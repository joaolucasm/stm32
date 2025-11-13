#include "waveform.h"

volatile bool flagBufferCheio = false;
volatile bool flagAconteceuDistorcao = false; //Flag para indicar que aconteceu alguma distorção na forma de onda
volatile bool flagDMATransferPageComplete = false; //Flag para quando uma transferência completa de página for feita via DMA
volatile bool flagDMATransferBufferComplete = false; //Flag para quando uma transferência completa do buffer for feita via DMA

uint16_t trigger_address = 0x0000;
uint16_t dma_rx_buffer[TOTAL_16BIT_BUFFER] = {0};
uint16_t variationEvent = 0x0000;

extern osMutexId_t spiMutexHandle;
extern osMessageQueueId_t xWaveformQueueHandle;
volatile SpiDMAReadState_t spiReadState = SPI_STATE_IDLE;
volatile static DetectorState_t detectorState = STATE_IDLE;


void ADE9000_Trigger_Detector()
{
	switch(detectorState)
	{
		case STATE_IDLE:
			if(flagAconteceuDistorcao)
			{
				flagAconteceuDistorcao = false;
				detectorState = STATE_CHECK_STATUS;
			}
		break;
		case STATE_CHECK_STATUS:

			uint32_t status1 = ADE9000SPI_Read32(ADDR_STATUS1);

			if(status1 & STATUS1_DIPA_BIT){variationEvent |= (1 << 1);printf("DIP no canal A\n\r");} //DIP de tensao na fase A
			if(status1 & STATUS1_DIPB_BIT){variationEvent |= (1 << 2);printf("DIP no canal B\n\r");} //DIP de tensao na fase B
			if(status1 & STATUS1_DIPC_BIT){variationEvent |= (1 << 3);printf("DIP no canal C\n\r");} //DIP de tensao na fase C
			if(status1 & STATUS1_SWELLA_BIT){variationEvent |= (1 << 4);printf("SWELL no canal A\n\r");} //SWELL de tensao na fase A
			if(status1 & STATUS1_SWELLB_BIT){variationEvent |= (1 << 5);printf("SWELL no canal B\n\r");} //SWELL de tensao na fase B
			if(status1 & STATUS1_SWELLC_BIT){variationEvent |= (1 << 6);printf("SWELL no canal C\n\r");} //SWELL de tensao na fase C
			if(status1 & STATUS1_OI_BIT) //Aconteceu um overcurrent
			{
				uint16_t oistatus = ADE9000SPI_Read16(ADDR_OISTATUS);
				if(oistatus & OISTATUS_IA_BIT){variationEvent |= (1 << 7);printf("OI no canal A\n\r");}
				if(oistatus & OISTATUS_IB_BIT){variationEvent |= (1 << 8);printf("OI no canal B\n\r");}
				if(oistatus & OISTATUS_IC_BIT){variationEvent |= (1 << 9);printf("OI no canal C\n\r");}
			}

			uint16_t wfb_tgr_stat = ADE9000SPI_Read16(ADDR_WFB_TRG_STAT);
			trigger_address = (wfb_tgr_stat & 0x7FF); //Operação de máscara apenas para filtrar os bits de WFB_TRG_ADDR

			ADE9000SPI_Write32(ADDR_STATUS1,0xFFFFFFFF);//Dentro de status 1 eu reseto todos

			osMutexAcquire(spiMutexHandle, osWaitForever);
			spiReadState = SPI_STATE_READING_FULL_BUFFER;
			ADE9000SPI_BurstRead_DMA(ADDR_WFB_BASE, TOTAL_16BIT_BUFFER, dma_rx_buffer);
			detectorState = STATE_WAITING_FOR_DMA;

			//detectorState = STATE_IDLE;
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
			Analisar_Forma_Onda_Capturada(variationEvent,trigger_address);

			//ADE9000SPI_Write16(ADDR_WFB_CFG, ADE9000_WFB_CFG_STOP);

			//ADE9000SPI_Write16(ADDR_WFB_CFG, ADE9000_WFB_CFG_START);

			detectorState = STATE_IDLE;
			break;
	}
}

void ADE9000_Init_Trigger_Detector()
{
	ADE9000SPI_Write16(ADDR_WFB_CFG,ADE9000_WFB_CFG_CONFIGURATION);

	ADE9000SPI_Write16(ADDR_WFB_TRG_CFG,ADE9000_16BIT_BLANK);

	ADE9000SPI_Write16(ADDR_WFB_PG_IRQEN,ADE9000_WFB_PQ_IRQEN);

	ADE9000SPI_Write16(ADDR_WFB_CFG,ADE9000_WFB_CFG_START); //SOMENTE nesse ponto do código é inicado a captura de valores pelo WFB

	while(flagBufferCheio != true){osDelay(1);} //Aqui é esperado a interrupção setada anteriormente, avisando que o buffer está cheio

	flagBufferCheio = false;

	//ADE9000SPI_Write32(ADDR_STATUS0,STATUS0_PAGE_FULL);
	ADE9000SPI_Write16(ADDR_WFB_PG_IRQEN, ADE9000_16BIT_BLANK);

	ADE9000SPI_Write16(ADDR_WFB_TRG_CFG, ADE9000_WFB_TRG_CFG); //Aqui habilita todas as interrupções que realmente interessam
}

void Analisar_Forma_Onda_Capturada(uint16_t eventos,uint16_t trigger_addr_capturado)
{
	for(int i = 0;i < BUFFER_TAM; i++)
	{
		if((i%8) == 1) // O filtro "if" SÓ deixa passar i = 1, 9, 17, 25, 33... (canal VA)
		{
			getRealValues2(Sample_Linear_Remontado(i,trigger_addr_capturado), i%8);
			//osDelay(1);
		}
	}
}

int32_t Sample_Linear_Remontado(uint16_t index_linear, uint16_t trigger_addr_capturado)
{
	uint16_t indice_raw = (index_linear + trigger_addr_capturado - 1024 + 2048) % 2048;

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
		double pin_voltage = ((double)raw32 / ADE9000_DSP_FS_DECIMAL) * ADE9000_PIN_FS_VOLTAGE;

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
	uint32_t bits_to_clear = 0x00000000;

	if(GPIO_Pin == IRQ0_ADE_Pin)
	{
		uint32_t status0 = ADE9000SPI_Read32(ADDR_STATUS0);

		if((status0 & STATUS0_PAGE_FULL))
		{
			flagBufferCheio = true;
			bits_to_clear |= STATUS0_PAGE_FULL;
		}

		if(status0 & STATUS0_WFB_TRIG_IRQ)
		{
			flagAconteceuDistorcao = true;
			bits_to_clear |= STATUS0_WFB_TRIG_IRQ;
		}

		if(bits_to_clear > 0)
			ADE9000SPI_Write32(ADDR_STATUS0, bits_to_clear);
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
