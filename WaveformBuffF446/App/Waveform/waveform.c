#include "waveform.h"

volatile bool flagBufferCheio = false;
volatile bool flagAconteceuDistorcao = false; //Flag para indicar que aconteceu alguma distorção na forma de onda
volatile bool flagDMATransferPageComplete = false; //Flag para quando uma transferência completa de página for feita via DMA
volatile bool flagDMATransferBufferComplete = false; //Flag para quando uma transferência completa do buffer for feita via DMA

uint16_t dma_rx_buffer[TOTAL_16BIT_BUFFER] = {0};

extern osMutexId_t spiMutexHandle;
extern osMessageQueueId_t xWaveformQueueHandle;
volatile SpiDMAReadState_t spiReadState = SPI_STATE_IDLE;
volatile static DetectorState_t detectorState = STATE_INIT;


void ADE9000_Trigger_Detector()
{
	uint16_t triggerAddress = 0x0000;
	uint16_t variationEvent = 0x0000;
	switch(detectorState)
	{
		case STATE_INIT:
			ADE9000_Init_WFB();
			detectorState = STATE_IDLE;
		break;
		case STATE_IDLE:
			if(flagAconteceuDistorcao)
			{
				flagAconteceuDistorcao = false;
				detectorState = STATE_CHECK_STATUS;
			}
		break;
		case STATE_CHECK_STATUS:
			uint32_t status1 = ADE9000SPI_Read32(ADDR_STATUS1);

			if(status1 & STATUS1_DIPA_BIT){variationEvent |= (1 << 0);} //DIP de tensao na fase A
			if(status1 & STATUS1_DIPB_BIT){variationEvent |= (1 << 1);} //DIP de tensao na fase B
			if(status1 & STATUS1_DIPC_BIT){variationEvent |= (1 << 2);} //DIP de tensao na fase C
			if(status1 & STATUS1_SWELLA_BIT){variationEvent |= (1 << 3);} //SWELL de tensao na fase A
			if(status1 & STATUS1_SWELLB_BIT){variationEvent |= (1 << 4);} //SWELL de tensao na fase B
			if(status1 & STATUS1_SWELLC_BIT){variationEvent |= (1 << 5);} //SWELL de tensao na fase C
			if(status1 & STATUS1_OI_BIT) //Aconteceu um overcurrent
			{
				uint16_t oistatus = ADE9000SPI_Read16(ADDR_OISTATUS);
				if(oistatus & OISTATUS_IA_BIT){variationEvent |= (1 << 6);}
				if(oistatus & OISTATUS_IB_BIT){variationEvent |= (1 << 7);}
				if(oistatus & OISTATUS_IC_BIT){variationEvent |= (1 << 8);}
			}

			uint16_t wfb_tgr_stat = ADE9000SPI_Read16(ADDR_WFB_TRG_STAT);
			triggerAddress = (wfb_tgr_stat & 0x07F8); //Operação de máscara apenas para filtrar os bits de WFB_TRG_ADDR

			ADE9000SPI_Write32(ADDR_STATUS1,0xFFFFFFFF);//Dentro de status 1 eu reseto todos

			osMutexAcquire(spiMutexHandle, osWaitForever);

			spiReadState = SPI_STATE_READING_FULL_BUFFER;
			ADE9000SPI_BurstRead_DMA(ADDR_WFB_BASE, TOTAL_16BIT_BUFFER, dma_rx_buffer);
			detectorState = STATE_WAITING_FOR_DMA;
		break;
		case STATE_WAITING_FOR_DMA:
			if(flagDMATransferBufferComplete)
			{
				osMutexRelease(spiMutexHandle);
				flagDMATransferBufferComplete = false;
				detectorState = STATE_PROCESSING;
			}
			break;
		case STATE_PROCESSING:
			Analisar_Forma_Onda_Capturada(variationEvent,triggerAddress);

			ADE9000SPI_Write16(ADDR_WFB_CFG, ADE9000_WFB_CFG_STOP);

			ADE9000SPI_Write16(ADDR_WFB_CFG, ADE9000_WFB_CFG_START);

			detectorState = STATE_IDLE;
			break;
	}
}

void ADE9000_Init_WFB()
{
	ADE9000SPI_Write32(ADDR_STATUS0,0xFFFFFFFF);
	ADE9000SPI_Write16(ADDR_WFB_CFG,0x0000);
	ADE9000SPI_Write16(ADDR_WFB_PG_IRQEN,0x0000);

	osDelay(1);

	ADE9000SPI_Write16(ADDR_WFB_CFG,ADE9000_WFB_CFG_CONFIGURATION); //Configurando o WFB
	ADE9000SPI_Write16(ADDR_WFB_PG_IRQEN,ADE9000_WFB_PQ_IRQEN); //Habilita interrupção para quando a página 7 (metade) do buffer encher
	ADE9000SPI_Write16(ADDR_WFB_CFG,ADE9000_WFB_CFG_START); //SOMENTE nesse ponto do código é iniciado a captura de valores pelo WFB

	while(!flagBufferCheio)//Aqui é esperado a interrupção setada anteriormente, avisando que o buffer está cheio
		{osDelay(1);}

	flagBufferCheio = false;

	ADE9000SPI_Write16(ADDR_WFB_TRG_CFG, ADE9000_WFB_TRG_CFG);//Aqui habilita todas as interrupções que realmente interessam (OI/SWELL/DIP)

	osDelay(1);

	ADE9000SPI_Write32(ADDR_STATUS0,0xFFFFFFFF);
	ADE9000SPI_Write16(ADDR_WFB_PG_IRQEN, 0x0000);
}

void Analisar_Forma_Onda_Capturada(uint16_t eventos,uint16_t trigger_addr_capturado)
{
	for(int i = 0;i < BUFFER_TAM; i++)
	{
		if((i%8) == 1) // O filtro "if" SÓ deixa passar i = 1, 9, 17, 25, 33... (canal VA)
		{
			getRealValues2(Sample_Linear_Remontado(i,trigger_addr_capturado), i%8);
			osDelay(1);
		}
	}
}

int32_t Sample_Linear_Remontado(uint16_t index_linear, uint16_t trigger_addr_capturado)
{
	uint16_t indice_raw = (index_linear + trigger_addr_capturado - 1024 + 1 + 2048) % 2049;

	uint16_t high_indice = indice_raw * 2;
	uint16_t low_indice = high_indice + 1;

	return ((int32_t) dma_rx_buffer[high_indice] << 16) | dma_rx_buffer[low_indice];
}

void getRealValues2(int32_t raw32, int word_in_sample)
{
	if(word_in_sample <= 5)
	{
		WaveformSample_t sampleToSend;
		//osStatus_t status;
		double real_value_f = 0.0;
		double pin_voltage = ((double)raw32 / DSP_FS_DECIMAL) * PIN_FS_VOLTAGE;

		if((word_in_sample % 2) == 0)//valores de corrente
			real_value_f = pin_voltage * FATOR_DIVISOR_CORRENTE;

		if((word_in_sample % 2) != 0)//valores de tensao
			real_value_f = pin_voltage * FATOR_DIVISOR_TENSAO;

		sampleToSend.channel = word_in_sample;
		sampleToSend.value = (int32_t)(real_value_f * SCALE_TO_MILLI_UNITS);

		osMessageQueuePut(xWaveformQueueHandle, &sampleToSend,0U,0U);
	}
	return;
}

//================================================================================================================================================
//
//														Funções de callback de interrupções e do DMA
//
//================================================================================================================================================

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == IRQ0_ADE_Pin)
	{
		uint32_t status0 = ADE9000SPI_Read32(ADDR_STATUS0);

		if((status0 & STATUS0_PAGE_FULL))
		{
			flagBufferCheio = true;
			ADE9000SPI_Write32(ADDR_STATUS0, STATUS0_PAGE_FULL);
		}
		if(status0 & STATUS0_WFB_TRIG_IRQ)
		{
			flagAconteceuDistorcao = true;
			ADE9000SPI_Write32(ADDR_STATUS0, STATUS0_WFB_TRIG_IRQ);
		}
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);

        if(spiReadState == SPI_STATE_READING_PAGE)
        {
        	spiReadState = SPI_STATE_IDLE;
        	flagDMATransferPageComplete = true;
        }

        if(spiReadState == SPI_STATE_READING_FULL_BUFFER)
        {
        	spiReadState = SPI_STATE_IDLE;
        	flagDMATransferBufferComplete = true;
        }
    }
}
