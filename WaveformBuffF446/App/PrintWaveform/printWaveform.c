#include "printWaveform.h"

extern bool flagDMATransferPageComplete;
extern SpiDMAReadState_t spiReadState;
extern osMessageQueueId_t xWaveformQueueHandle;

lumen_packet_t *currentPacket;

void mainPrint_Waveform()
{
	bool teste = false;
	while(1)
	{
		while(lumen_available() > 0 )
		{
			currentPacket = lumen_get_first_packet();

			if(currentPacket != NULL)
			{
				switch(currentPacket->address)
				{
					case address_getCurrentValues:
						//getWaveformPrint(address_getCurrentValues);
						break;
					case address_getVoltageValues:
						teste = currentPacket->data._bool;
						//getWaveformPrint(address_getVoltageValues);
						break;
					default:
						break;
				}
			}
		}

		if(teste)
		{
			ADE9000_Trigger_Detector();
		}
		osDelay(1);
	}
}


void getWaveformPrint(int selected)
{
	static uint16_t dma_rx_buffer[TOTAL_16BIT_WORDS];

	//uint16_t baseAddr = ADDR_WFB_BASE;
	for(int i = 0;i < 2; i++)
	{
		uint16_t baseAddr = ADDR_WFB_BASE + (i * PAGE_WORDS);

		if(spiReadState == SPI_STATE_IDLE)
		{
			spiReadState = SPI_STATE_READING_PAGE;
			ADE9000SPI_BurstRead_DMA(baseAddr, TOTAL_16BIT_WORDS, dma_rx_buffer);

			while(flagDMATransferPageComplete == false){osDelay(1);}
		}

		if(flagDMATransferPageComplete) //função chamada quando o DMA termina de receber toda a página do buffer, processa os dados e coloca na fila, para outra task enviar eles para onde for preciso
		{
			flagDMATransferPageComplete = false;

			int32_t temp = 0x00000000;
			for (int i = 0; i < PAGE_WORDS; i++)
			{
				// O ADE9000 envia o MSB (High) primeiro, depois o LSB (Low)
				uint16_t high = dma_rx_buffer[i * 2];
				uint16_t low  = dma_rx_buffer[i * 2 + 1];

				temp = ((int32_t)high << 16) | low;
				// Combina as duas palavras de 16 bits em uma de 32 bits
				getRealValues(temp,i % 8,selected);
			}

			lumen_packet_t resetFlag = {selected,kBool};
			resetFlag.data._bool = false;
			lumen_write_packet(&resetFlag);
		}
	}
}



void getRealValues(int32_t raw32, int word_in_sample, int selected)
{
	if(word_in_sample <= 5)
	{
		WaveformSample_t sampleToSend;
		osStatus_t status;
		double real_value_f = 0.0;
		double pin_voltage = ((double)raw32 / DSP_FS_DECIMAL) * PIN_FS_VOLTAGE;

		if(selected == address_getCurrentValues)
		{
			if((word_in_sample % 2) == 0)//valores de corrente
			{
				real_value_f = pin_voltage * FATOR_DIVISOR_CORRENTE;
				sampleToSend.channel = word_in_sample;
				sampleToSend.value = (int32_t)(real_value_f);
				status = osMessageQueuePut(xWaveformQueueHandle, &sampleToSend,0U,0U);
			}
		}

		if(selected == address_getVoltageValues )
		{
			if((word_in_sample % 2) != 0)//valores de tensao
			{
				real_value_f = pin_voltage * FATOR_DIVISOR_TENSAO;
				sampleToSend.channel = word_in_sample;
				sampleToSend.value = (int32_t)(real_value_f);
				status = osMessageQueuePut(xWaveformQueueHandle, &sampleToSend,0U,0U);
			}
		}

		if(status != osOK)
		{
				//tratar quando a fila estiver cheia aq, houve erro para inserir na fila e os dados tao sendo descartados
		}
	}

	return;
}

