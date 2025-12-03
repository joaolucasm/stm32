#include "waveform.h"

extern osMessageQueueId_t xWaveformQueueHandle;

//================================================================================================================================================
//
//									Função executada por outra task para envio separdado dos dados para onde for necessário
//
//================================================================================================================================================
void UartTransmit(void)
{
	WaveformSample_t receivedSample;
	osStatus_t status;

	while(1)
	{
		status = osMessageQueueGet(xWaveformQueueHandle,&receivedSample,NULL,osWaitForever);
		if(status == osOK)
		{
			switch(receivedSample.channel)
			{
				case 0:
					lumen_packet_t currentPhaseA = {address_currentPhaseA,kS32};
					currentPhaseA.data._s32 = receivedSample.value;
					lumen_write_packet(&currentPhaseA);

					//printf("%d\n",(int)receivedSample.value);
					break;
				case 1:
					lumen_packet_t voltagePhaseA = {address_voltagePhaseA,kS32};
					voltagePhaseA.data._s32 = receivedSample.value;
					lumen_write_packet(&voltagePhaseA);

					//printf("%d\n",(int)receivedSample.value);
					break;
				case 2:
					lumen_packet_t currentPhaseB = {address_currentPhaseB,kS32};
					currentPhaseB.data._s32 = receivedSample.value;
					lumen_write_packet(&currentPhaseB);

					//printf("%d\n",(int)receivedSample.value);
					break;
				case 3:
					lumen_packet_t voltagePhaseB = {address_voltagePhaseB,kS32};
					voltagePhaseB.data._s32 = receivedSample.value;
					lumen_write_packet(&voltagePhaseB);

					//printf("%d\n",(int)receivedSample.value);
					break;
				case 4:
					lumen_packet_t currentPhaseC = {address_currentPhaseC,kS32};
					currentPhaseC.data._s32 = receivedSample.value;
					lumen_write_packet(&currentPhaseC);

					//printf("%d\n",(int)receivedSample.value);
					break;
				case 5:
					lumen_packet_t voltagePhaseC = {address_voltagePhaseC,kS32};
					voltagePhaseC.data._s32 = receivedSample.value;
					lumen_write_packet(&voltagePhaseC);

					//printf("%d\n",(int)receivedSample.value);
					break;
				default:
					break;
			}
		}
	}
}
