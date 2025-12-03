#include "waveform.h"

extern osMutexId_t spiMutexHandle;
extern osMessageQueueId_t xWaveformQueueHandle;
extern SPI_HandleTypeDef hspi1;
extern osThreadId_t enernetTaskHandle;

volatile bool flagAconteceuDistorcao = false; //Flag para indicar que aconteceu alguma distorção na forma de onda
volatile bool capturandoDepoisTrigger = false;
volatile uint32_t amostrasCapturaPosTrigger = 0;
volatile uint16_t history_head = 0;
volatile uint16_t print_head = 0;
volatile static WaveformState_t currentState = WFB_STATE_INIT;

static AdeRawSample_t* raw_struct_array;
static int32_t acc_IA = 0, acc_VA = 0;
static int32_t acc_IB = 0, acc_VB = 0;
static int32_t acc_IC = 0, acc_VC = 0;
static int32_t acc_IN = 0;
static uint8_t decim_counter = 0;

uint16_t dma_rx_buffer[DMA_BUFFER_SIZE_16_BITS] = {0};
//ProcessedSample_t history_buffer[HISTORY_BUFFER_SIZE] = {0};
static ProcessedSample_t local_snapshot[PRINT_BUFFER_SIZE] = {0};
ProcessedSample_t print_buffer[PRINT_BUFFER_SIZE] = {0};
uint16_t variationEvent = 0x0000;

static void Waveform_Extract_Send(void)
{
//	WaveformSample_t sampleMsg;
//	uint16_t idx;
//	uint16_t oldest_index = (history_head + 1) % HISTORY_BUFFER_SIZE;
//
//	for(int i = 0; i < HISTORY_BUFFER_SIZE; i++)
//	{
//		idx = (oldest_index + i) % HISTORY_BUFFER_SIZE;
//
//		sampleMsg.channel = 1;
//		sampleMsg.value = history_buffer[idx].VA;
//
//		osMessageQueuePut(xWaveformQueueHandle,&sampleMsg,0U,10);
//	}
}

void Waveform_Main(void)
{
	for(;;)
	{
		switch(currentState)
		{
			case WFB_STATE_INIT:
				//osMutexAcquire(spiMutexHandle, osWaitForever);
				Init_WFB();
				currentState = WFB_STATE_STREAMING;
				break;
			case WFB_STATE_STREAMING:
				uint32_t flags = osThreadFlagsWait(FLAG_CAPTURE_COMPLETE, osFlagsWaitAny, osWaitForever);

				if(flags & FLAG_CAPTURE_COMPLETE)
					currentState = 	WFB_STATE_PROCESSING;
				break;
			case WFB_STATE_PROCESSING:
				Analise_Interrupcao();
				//osMutexRelease(spiMutexHandle); //Libera o MUTEX
				Waveform_Extract_Send();
				currentState = WFB_STATE_RESTART;
				break;
			case WFB_STATE_RESTART:
				//osMutexAcquire(spiMutexHandle, osWaitForever);
				Restart_WFB();
				currentState = WFB_STATE_STREAMING;
				break;
			default:
				currentState = WFB_STATE_INIT;
				break;
		}
		osDelay(1);
	}
}

uint16_t Analise_Interrupcao(void)
{
	uint16_t variationEvent = 0x0000;
	uint32_t status1 = ADE9000SPI_Read32_Unsafe(ADDR_STATUS1);

	if(status1 & STATUS1_DIPA_BIT){variationEvent |= (1 << 0);} //DIP de tensao na fase A
	if(status1 & STATUS1_DIPB_BIT){variationEvent |= (1 << 1);} //DIP de tensao na fase B
	if(status1 & STATUS1_DIPC_BIT){variationEvent |= (1 << 2);} //DIP de tensao na fase C
	if(status1 & STATUS1_SWELLA_BIT){variationEvent |= (1 << 3);} //SWELL de tensao na fase A
	if(status1 & STATUS1_SWELLB_BIT){variationEvent |= (1 << 4);} //SWELL de tensao na fase B
	if(status1 & STATUS1_SWELLC_BIT){variationEvent |= (1 << 5);} //SWELL de tensao na fase C
	if(status1 & STATUS1_OI_BIT) //Aconteceu um overcurrent
	{
		uint16_t oistatus = ADE9000SPI_Read16(ADDR_OISTATUS);
		if(oistatus & OISTATUS_IA_BIT){variationEvent |= (1 << 6);}//OI na fase A
		if(oistatus & OISTATUS_IB_BIT){variationEvent |= (1 << 7);}//OI na fase B
		if(oistatus & OISTATUS_IC_BIT){variationEvent |= (1 << 8);}//OI na fase C
	}
	if(status1 & STATUS1_ZXTOVA_BIT){variationEvent |= (1 << 9);}//Zero crossing timeout na fase A
	if(status1 & STATUS1_ZXTOVB_BIT){variationEvent |= (1 << 10);}//Zero crossing timeout na fase B
	if(status1 & STATUS1_ZXTOVC_BIT){variationEvent |= (1 << 11);}//Zero crossing timeout na fase C

	ADE9000SPI_Write32(ADDR_STATUS0,0xFFFFFFFF);
	ADE9000SPI_Write32(ADDR_STATUS1,0xFFFFFFFF);

	return variationEvent;
}

void Init_WFB(void)
{
	ADE9000SPI_Write16(ADDR_WFB_CFG,ADE9000_WFB_CFG_CONFIGURATION); //Configurando o WFB
	ADE9000SPI_Write16(ADDR_WFB_CFG,ADE9000_WFB_CFG_START); //SOMENTE nesse ponto do código é iniciado a captura de valores pelo WFB

	ADE9000SPI_Write32(ADDR_STATUS0,0xFFFFFFFF);
	ADE9000SPI_Write16(ADDR_WFB_TRG_CFG,ADE9000_WFB_TRG_CFG);

	raw_struct_array = (AdeRawSample_t*)dma_rx_buffer;

	ADE9000SPI_BurstRead_DMA_Start(ADDR_WFB_BASE, DMA_BUFFER_SIZE_16_BITS, dma_rx_buffer);
}

void Restart_WFB(void)
{
	Reset_Internal_State();
	ADE9000SPI_Write32(ADDR_STATUS0,0xFFFFFFFF);
	ADE9000SPI_Write16(ADDR_WFB_TRG_CFG,ADE9000_WFB_TRG_CFG);

	ADE9000SPI_BurstRead_DMA_Start(ADDR_WFB_BASE, DMA_BUFFER_SIZE_16_BITS, dma_rx_buffer);
}

void Waveform_Process_Chunk_Print(uint16_t start_index,uint16_t count)
{
	for(int i = 0;i < count; i++)
	{
		AdeRawSample_t* current_sample = &raw_struct_array[start_index + i];

		acc_IA += ADE_GET_VALUE(current_sample->IA);
		acc_VA += ADE_GET_VALUE(current_sample->VA);
		acc_IB += ADE_GET_VALUE(current_sample->IB);
		acc_VB += ADE_GET_VALUE(current_sample->VB);
		acc_IC += ADE_GET_VALUE(current_sample->IC);
		acc_VC += ADE_GET_VALUE(current_sample->VC);
		acc_IN += ADE_GET_VALUE(current_sample->IN);

		decim_counter++;

		if(decim_counter >= DECIMATION_FACTOR_PRINT)
		{
			print_buffer[print_head].IA = (int32_t)((float)acc_IA * FACTOR_I_DECIMADO_PRINT);
			print_buffer[print_head].VA = (int32_t)((float)acc_VA * FACTOR_V_DECIMADO_PRINT);
			print_buffer[print_head].IB = (int32_t)((float)acc_IB * FACTOR_I_DECIMADO_PRINT);
			print_buffer[print_head].VB = (int32_t)((float)acc_VB * FACTOR_V_DECIMADO_PRINT);
			print_buffer[print_head].IC = (int32_t)((float)acc_IC * FACTOR_I_DECIMADO_PRINT);
			print_buffer[print_head].VC = (int32_t)((float)acc_VC * FACTOR_V_DECIMADO_PRINT);
			print_buffer[print_head].IN = (int32_t)((float)acc_IN * FACTOR_I_DECIMADO_PRINT);

			print_head++;

			if(print_head >= PRINT_BUFFER_SIZE)
			{
				print_head = 0;
			}

			acc_IA = 0; acc_VA = 0;
			acc_IB = 0; acc_VB = 0;
			acc_IC = 0; acc_VC = 0;
			acc_IN = 0;

			decim_counter = 0;
		}
	}
}



//void Waveform_Process_Chunk(uint16_t start_index,uint16_t count)
//{
//	for(int i = 0;i < count; i++)
//	{
//		AdeRawSample_t* current_sample = &raw_struct_array[start_index + i];
//
//		acc_IA += ADE_GET_VALUE(current_sample->IA);
//		acc_VA += ADE_GET_VALUE(current_sample->VA);
//		acc_IB += ADE_GET_VALUE(current_sample->IB);
//		acc_VB += ADE_GET_VALUE(current_sample->VB);
//		acc_IC += ADE_GET_VALUE(current_sample->IC);
//		acc_VC += ADE_GET_VALUE(current_sample->VC);
//		acc_IN += ADE_GET_VALUE(current_sample->IN);
//
//		decim_counter++;
//
//		if(decim_counter >= DECIMATION_FACTOR)
//		{
//			history_buffer[history_head].IA = (int32_t)((float)acc_IA * FACTOR_I_DECIMADO);
//			history_buffer[history_head].VA = (int32_t)((float)acc_VA * FACTOR_V_DECIMADO);
//			history_buffer[history_head].IB = (int32_t)((float)acc_IB * FACTOR_I_DECIMADO);
//			history_buffer[history_head].VB = (int32_t)((float)acc_VB * FACTOR_V_DECIMADO);
//			history_buffer[history_head].IC = (int32_t)((float)acc_IC * FACTOR_I_DECIMADO);
//			history_buffer[history_head].VC = (int32_t)((float)acc_VC * FACTOR_V_DECIMADO);
//			history_buffer[history_head].IN = (int32_t)((float)acc_IN * FACTOR_I_DECIMADO);
//
//			history_head++;
//
//			if(history_head >= HISTORY_BUFFER_SIZE)
//			{
//				history_head = 0;
//			}
//
//			if(capturandoDepoisTrigger)
//			{
//				if(amostrasCapturaPosTrigger > 0)
//				{
//					amostrasCapturaPosTrigger--;
//				}
//				else
//				{
//					HAL_SPI_DMAStop(&hspi1); //Para o DMA
//					HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET); //Levanta o pino de CS de novo
//
//					ADE9000SPI_Write16(ADDR_WFB_TRG_CFG,0x0000);
//					osThreadFlagsSet(enernetTaskHandle,FLAG_CAPTURE_COMPLETE);
//					capturandoDepoisTrigger = false;
//				}
//			}
//
//			acc_IA = 0; acc_VA = 0;
//			acc_IB = 0; acc_VB = 0;
//			acc_IC = 0; acc_VC = 0;
//			acc_IN = 0;
//
//			decim_counter = 0;
//		}
//	}
//}

void Get_Print_Buffer_Snapshot(ProcessedSample_t *destination, uint16_t *head_position)
{
    // 1. (Opcional mas recomendado) Desabilita interrupção do DMA brevemente
    // para garantir que o 'print_head' não mude durante a cópia.
    // Como memcpy é muito rápido (< 50us), isso não afeta o streaming do ADE.
    //__disable_irq();

    // 2. Copia o buffer inteiro para o destino
    memcpy(destination, print_buffer, sizeof(print_buffer));

    // 3. Salva a posição da cabeça para saber onde começa o "mais antigo"
    *head_position = print_head;

    // 4. Reabilita interrupções
   //__enable_irq();
}

void Reset_Internal_State(void)
{
	amostrasCapturaPosTrigger = 0;
	acc_IA = 0; acc_VA = 0;
	acc_IB = 0; acc_VB = 0;
	acc_IC = 0; acc_VC = 0;
	acc_IN = 0;
	decim_counter = 0;

	memset(dma_rx_buffer, 0, sizeof(dma_rx_buffer));
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
		if(!capturandoDepoisTrigger && currentState == WFB_STATE_STREAMING)
		{
			capturandoDepoisTrigger = true;
			amostrasCapturaPosTrigger = HISTORY_BUFFER_SIZE / 2;
		}
	}

	if(GPIO_Pin == IRQ1_ADE_Pin)
	{
		if(!capturandoDepoisTrigger && currentState == WFB_STATE_STREAMING)
		{
			capturandoDepoisTrigger = true;
			amostrasCapturaPosTrigger = HISTORY_BUFFER_SIZE / 2;
		}
	}
}

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        //Waveform_Process_Chunk(0, DMA_SAMPLE_SETS);
    	Waveform_Process_Chunk_Print(0, DMA_SAMPLE_SETS);
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        //Waveform_Process_Chunk(DMA_SAMPLE_SETS, DMA_SAMPLE_SETS);
    	Waveform_Process_Chunk_Print(DMA_SAMPLE_SETS, DMA_SAMPLE_SETS);

        HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);

        ADE9000SPI_BurstRead_DMA_Start(ADDR_WFB_BASE, DMA_BUFFER_SIZE_16_BITS, dma_rx_buffer);
    }
}
//================================================================================================================================================
//
//														Função para a Task Print Waveform
//
//================================================================================================================================================
void mainPrint_Waveform()
{
	lumen_packet_t *currentPacket;
	while(1)
	{
		while(lumen_available() > 0 )
		{
			currentPacket = lumen_get_first_packet();
			uint16_t snapshot_head;

			if(currentPacket != NULL)
			{
				switch(currentPacket->address)
				{
					case address_getCurrentValues:
						if(currentPacket->data._bool == true)
						{
							WaveformSample_t sampleMsg1;
							HAL_SPI_DMAStop(&hspi1); //Para o DMA
							HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);

							for(int i = 0; i < PRINT_BUFFER_SIZE; i++)
							{
								sampleMsg1.channel = 0; //COLOCANDO NA FILA O CANAL IA
								sampleMsg1.value = print_buffer[i].IA;
								osMessageQueuePut(xWaveformQueueHandle,&sampleMsg1,0U,10);

								sampleMsg1.channel = 2; //COLOCANDO NA FILA O CANAL IB
								sampleMsg1.value = print_buffer[i].IB;
								osMessageQueuePut(xWaveformQueueHandle,&sampleMsg1,0U,10);

								sampleMsg1.channel = 3; //COLOCANDO NA FILA O CANAL IC
								sampleMsg1.value = print_buffer[i].IC;
								osMessageQueuePut(xWaveformQueueHandle,&sampleMsg1,0U,10);
							}

							Restart_WFB();
						}
						break;
					case address_getVoltageValues:
						if(currentPacket->data._bool == true)
						{

							HAL_SPI_DMAStop(&hspi1); //Para o DMA
							HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);
							Get_Print_Buffer_Snapshot(local_snapshot, &snapshot_head);
							WaveformSample_t sampleMsg2;
							uint16_t idx;
							for(int i = 0; i < PRINT_BUFFER_SIZE; i++)
							{
								idx = (snapshot_head + i) % PRINT_BUFFER_SIZE;
								sampleMsg2.channel = 1; //COLOCANDO NA FILA O CANAL VA
								sampleMsg2.value = local_snapshot[idx].VA;
								osMessageQueuePut(xWaveformQueueHandle,&sampleMsg2,0U,10);

								sampleMsg2.channel = 3; //COLOCANDO NA FILA O CANAL VB
								sampleMsg2.value = local_snapshot[idx].VB;
								osMessageQueuePut(xWaveformQueueHandle,&sampleMsg2,0U,10);

								sampleMsg2.channel = 5; //COLOCANDO NA FILA O CANAL VC
								sampleMsg2.value = local_snapshot[idx].VC;
								osMessageQueuePut(xWaveformQueueHandle,&sampleMsg2,0U,10);
							}
							Restart_WFB();
						}
						break;
					default:
						break;
				}
			}
		}
		osDelay(1);
	}
}
