#include "printWaveform.h"

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

		}
		osDelay(1);
	}
}

