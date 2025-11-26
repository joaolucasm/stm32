#include "waveform.h"

volatile bool flagBufferCheio = false;
volatile bool flagAconteceuDistorcao = false; //Flag para indicar que aconteceu alguma distorção na forma de onda
volatile bool flagDMATransferPageComplete = false; //Flag para quando uma transferência completa de página for feita via DMA
volatile bool flagDMATransferBufferComplete = false; //Flag para quando uma transferência completa do buffer for feita via DMA

//uint16_t dma_rx_buffer[TOTAL_16BIT_BUFFER] = {0};
//ade_wfb dma_rx_buffer[TOTAL_SAMPLES];
uint16_t dma_rx_buffer[512];

extern osMutexId_t spiMutexHandle;
extern osMessageQueueId_t xWaveformQueueHandle;
volatile SpiDMAReadState_t spiReadState = SPI_STATE_IDLE;
volatile static DetectorState_t detectorState = STATE_INIT;
uint16_t variationEvent = 0x0000;

void ADE9000_Trigger_Detector()
{


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
			ADE9000SPI_Write16(ADDR_WFB_CFG, 0x13AB);

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

			osMutexAcquire(spiMutexHandle, osWaitForever);

			spiReadState = SPI_STATE_READING_FULL_BUFFER;
			//ADE9000SPI_BurstRead_DMA(ADDR_WFB_BASE, TOTAL_16BIT_BUFFER,(uint16_t*) dma_rx_buffer);
			ADE9000SPI_BurstRead_DMA(ADDR_WFB_BASE,512,dma_rx_buffer);
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
			//Analisar_Forma_Onda_Capturada(variationEvent,triggerAddress);
			//Aqui é onde deve ser tratado os dados do buffer para ser enviado via UART
			uint16_t wfb_tgr_stat = ADE9000SPI_Read16(ADDR_WFB_TRG_STAT);
			uint16_t triggerAddress = (wfb_tgr_stat & 0x07FF); //Operação de máscara apenas para filtrar os bits de WFB_TRG_ADDR

			ADE9000SPI_Write32(ADDR_STATUS1,0xFFFFFFFF);//Dentro de status 1 eu reseto todos

			//separacaoCanais(triggerAddress);
			Processar_Buffer_VA_Unico(triggerAddress);

			memset(dma_rx_buffer, 0,sizeof(dma_rx_buffer));

			ADE9000SPI_Write16(ADDR_WFB_CFG, ADE9000_WFB_CFG_STOP);
			ADE9000SPI_Write16(ADDR_WFB_CFG, ADE9000_WFB_CFG_START);

			detectorState = STATE_IDLE;
			break;
	}
}

void separacaoCanais(uint16_t triggerAddress)
{
	WaveformSample_t sampleToSend;
	int32_t raw_val;

//	uint16_t trigger_offset = triggerAddress & 0x07FF;
//	uint16_t stop_offset = (trigger_offset + 1024) % 2048;
//	uint16_t stop_index = stop_offset / 8; // Divide por 8 (tamanho da sua struct em words)
//	uint16_t start_index = (stop_index + 1) % TOTAL_SAMPLES;


	for(int i = 0; i < TOTAL_SAMPLES; i++)
	{
		//int idx = (start_index + i) % TOTAL_SAMPLES;

//		raw_val = ADE_GET_VALUE(dma_rx_buffer[i].IA);
//		sampleToSend.value = (int32_t)((float) raw_val * FACTOR_I);
//		sampleToSend.channel = 0;
//		//Envia para a fila
//		osMessageQueuePut(xWaveformQueueHandle,&sampleToSend,0U,0U);
//
//		raw_val = ADE_GET_VALUE(dma_rx_buffer[i].VA);
//		sampleToSend.value = (int32_t)((float) raw_val * FACTOR_V);
//		sampleToSend.channel = 1;
//		//Envia para a fila
//		osMessageQueuePut(xWaveformQueueHandle,&sampleToSend,0U,0U);
//
//		raw_val = ADE_GET_VALUE(dma_rx_buffer[i].IB);
//		sampleToSend.value = (int32_t)((float) raw_val * FACTOR_I);
//		sampleToSend.channel = 2;
//		//Envia para a fila
//		osMessageQueuePut(xWaveformQueueHandle,&sampleToSend,0U,0U);
//
//		raw_val = ADE_GET_VALUE(dma_rx_buffer[i].VB);
//		sampleToSend.value = (int32_t)((float) raw_val * FACTOR_V);
//		sampleToSend.channel = 3;
//		//Envia para a fila
//		osMessageQueuePut(xWaveformQueueHandle,&sampleToSend,0U,0U);
//
//		raw_val = ADE_GET_VALUE(dma_rx_buffer[i].IC);
//		sampleToSend.value = (int32_t)((float) raw_val * FACTOR_I);
//		sampleToSend.channel = 4;
//		//Envia para a fila
//		osMessageQueuePut(xWaveformQueueHandle,&sampleToSend,0U,0U);
//
//		raw_val = ADE_GET_VALUE(dma_rx_buffer[i].VC);
//		sampleToSend.value = (int32_t)((float) raw_val * FACTOR_V);
//		sampleToSend.channel = 5;
//		//Envia para a fila
//		osMessageQueuePut(xWaveformQueueHandle,&sampleToSend,0U,0U);

		osDelay(1);
	}
}

void Processar_Buffer_VA_Unico(uint16_t triggerAddress)
{
    WaveformSample_t sampleToSend;
    int32_t raw_val;

    // 2. CALCULAR O PONTO DE PARTIDA (Start Index)
    // O Trigger Address retornado pelo ADE é baseado no mapa de memória COMPLETO (0x800-0xFFF).
    // O ADE grava internamente 8 canais, independente do que você lê no Burst.
    // Portanto, a matemática para achar o índice temporal (0-255) é a mesma.

    uint16_t trigger_offset = triggerAddress & 0x07FF;          // Offset em palavras (0-2047)
    uint16_t stop_addr_global = (trigger_offset + 1024) % 2048; // Onde o ADE parou de gravar
    uint16_t stop_sample_idx = stop_addr_global / 8;            // Índice da amostra (0-255)

    // A amostra mais antiga é a próxima após a parada
    uint16_t start_sample_idx = (stop_sample_idx + 1) % TOTAL_SAMPLES; // TOTAL_SAMPLES = 256

    // 3. REMONTAGEM E ENVIO
    for(int i = 0; i < TOTAL_SAMPLES; i++)
    {
        // Qual amostra queremos pegar agora (ordem cronológica)?
        int current_physical_idx = (start_sample_idx + i) % TOTAL_SAMPLES;

        // Como estamos lendo SÓ VA, o buffer tem uma estrutura simples:
        // [VA0_H, VA0_L, VA1_H, VA1_L, VA2_H, VA2_L...]
        // Cada amostra ocupa 2 posições de 16 bits.

        // Pegamos as partes Alta e Baixa usando o índice físico
        uint16_t val_high = dma_rx_buffer[current_physical_idx * 2];
        uint16_t val_low  = dma_rx_buffer[current_physical_idx * 2 + 1];

        // Reconstrói o int32 (Big Endian do ADE para Little Endian do STM32)
        raw_val = (int32_t)( ((uint32_t)val_high << 16) | val_low );

        // Converte e envia
        sampleToSend.value = (int32_t)((float)raw_val * FACTOR_V);
        sampleToSend.channel = 3; // Canal 1 = VA

        osMessageQueuePut(xWaveformQueueHandle, &sampleToSend, 0U, 0U);

        // Pequeno delay para não saturar a UART se necessário
        // osDelay(1);
    }
}


void ADE9000_Init_WFB()
{
	ADE9000SPI_Write32(ADDR_STATUS0,0xFFFFFFFF);
	ADE9000SPI_Write16(ADDR_WFB_CFG,0x0000);
	ADE9000SPI_Write16(ADDR_WFB_PG_IRQEN,0x0000);
	ADE9000SPI_Write16(ADDR_WFB_CFG,ADE9000_WFB_CFG_CONFIGURATION); //Configurando o WFB
	ADE9000SPI_Write16(ADDR_WFB_PG_IRQEN,ADE9000_WFB_PQ_IRQEN); //Habilita interrupção para quando a página 7 (metade) do buffer encher
	ADE9000SPI_Write16(ADDR_WFB_CFG,ADE9000_WFB_CFG_START); //SOMENTE nesse ponto do código é iniciado a captura de valores pelo WFB

	while(!flagBufferCheio){osDelay(1);}//Aqui é esperado a interrupção setada anteriormente, avisando que o buffer está cheio
	flagBufferCheio = false;

	ADE9000SPI_Write16(ADDR_WFB_TRG_CFG, ADE9000_WFB_TRG_CFG);//Aqui habilita todas as interrupções que realmente interessam (OI/SWELL/DIP)
	ADE9000SPI_Write32(ADDR_STATUS0,0xFFFFFFFF);
	ADE9000SPI_Write16(ADDR_WFB_PG_IRQEN, 0x0000);
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
//================================================================================================================================================
//
//														Cemitério de funções para teste de outros métodos
//
//================================================================================================================================================
/*

int32_t Sample_Linear_Remontado(uint16_t index_linear, uint16_t trigger_addr_capturado)
{
//	uint16_t indice_raw = (index_linear + trigger_addr_capturado - 1024 + 1 + 2048) % 2049;
//
//	uint16_t high_indice = indice_raw * 2;
//	uint16_t low_indice = high_indice + 1;
//
//	return ((int32_t) dma_rx_buffer[high_indice] << 16) | dma_rx_buffer[low_indice];

	uint16_t last_filled_address;

	if((trigger_addr_capturado + 1024) > 0xFFF)
		last_filled_address = trigger_addr_capturado - 1024;
	else
		last_filled_address = trigger_addr_capturado + 1024;

	uint16_t start_index = (last_filled_address + 1) % 2048;
	uint16_t indice_raw = (start_index + index_linear) % 2048;

	uint16_t high_indice = indice_raw * 2;
	uint16_t low_indice = high_indice + 1;

	return ((int32_t) dma_rx_buffer[high_indice] << 16) | dma_rx_buffer[low_indice];
}

void Analisar_Forma_Onda_Capturada(uint16_t eventos,uint16_t trigger_addr_capturado)
{
	for(int i = 0;i < 512; i++)
	{
		getRealValues2(Sample_Linear_Remontado(i,trigger_addr_capturado), 1);
		osDelay(1);
	}
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

		else//valores de tensao
			real_value_f = pin_voltage * FATOR_DIVISOR_TENSAO;

		sampleToSend.channel = word_in_sample;
		sampleToSend.value = (int32_t)(real_value_f * SCALE_TO_MILLI_UNITS);

		osMessageQueuePut(xWaveformQueueHandle, &sampleToSend,0U,0U);
	}
	return;
}

*/
