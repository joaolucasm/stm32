#ifndef WAVEFORM_H
#define WAVEFORM_H

//================================================================================================================================================
//
//																	Includes
//
//================================================================================================================================================
#include <stdio.h>
#include <stdint.h>
#include "LumenProtocol.h"
#include "calibration.h"
#include "ADE9000.h"
#include "main.h"

//================================================================================================================================================
//
//																	Defines
//
//================================================================================================================================================

#define PAGE_WORDS     						128     					/*Tamanho da página dentro do waveform buffer*/
#define TOTAL_16BIT_WORDS					(PAGE_WORDS * 2)			/*Quantidade de meias palavras dentro de uma página*/
#define TOTAL_SAMPLES						256
#define OFFSET_PALAVRAS						1024
#define NUM_PAGES							16							/*Quantidade de páginas dentro do waveform buffer*/
#define BUFFER_TAM							2048						/*Tamanho total do waveform buffer*/
#define TOTAL_16BIT_BUFFER					(BUFFER_TAM * 2)			/*Tamanho total do buffer em 16 bits*/
#define DSP_FS_DECIMAL 						74532013.0
#define PIN_FS_VOLTAGE						1.0
#define FATOR_DIVISOR_TENSAO				801.0
#define FATOR_DIVISOR_CORRENTE				(1.0/2.55)					/*Fator de conversão do sensor de corrente (1 / R_burden)*/
#define SCALE_TO_MILLI_UNITS				1000.0						/*Fator de enviar para a fila int32_t (mV e mA em vez de V e A)*/

#define STATUS1_OI_BIT						(1 << 17)					/*Bit de overcurrent no registrador status1*/
#define OISTATUS_IA_BIT						(1 << 0)					/*Bit de indicando a fase A no registrador oistatus*/
#define OISTATUS_IB_BIT						(1 << 1)					/*Bit de indicando a fase B no registrador oistatus*/
#define OISTATUS_IC_BIT						(1 << 2)					/*Bit de indicando a fase C no registrador oistatus*/
#define STATUS1_SWELLA_BIT					(1 << 20)					/*Bit de SWELL no channel A no registrador status1*/
#define STATUS1_SWELLB_BIT					(1 << 21)					/*Bit de SWELL no channel B no registrador status1*/
#define STATUS1_SWELLC_BIT					(1 << 22)					/*Bit de SWELL no channel C no registrador status1*/
#define STATUS1_DIPA_BIT					(1 << 23)					/*Bit de DIP no channel A no registrador status1*/
#define STATUS1_DIPB_BIT					(1 << 24)					/*Bit de DIP no channel B no registrador status1*/
#define STATUS1_DIPC_BIT					(1 << 25)					/*Bit de DIP no channel C no registrador status1*/
#define STATUS0_PAGE_FULL					(1 << 17)					/*Bit de status0 que avisa se uma página está cheia*/
#define STATUS0_WFB_TRIG_IRQ				(1 << 16)					/*Bit de status0 que avisa a parada do buffer*/

#define ADDR_WFB_BASE						0x00000800		 			/*Start of Waveform Buffer*/
#define ADDR_WFB_TOP						0x00000FFF					/*End of Waveform Buffer*/

//================================================================================================================================================
//
//												Definições das estruturas de dados utilizadas
//
//================================================================================================================================================

typedef enum{
	SPI_STATE_IDLE,							//SPI_DMA ocioso
	SPI_STATE_READING_PAGE,					//SPI está ocupado lendo uma página do buffer
	SPI_STATE_READING_FULL_BUFFER			//SPI está ocuádo lendo o buffer inteiro
}SpiDMAReadState_t;

typedef enum {
	STATE_IDLE,								//1. Esperando por um gatilho
	STATE_WAITING_FOR_DMA,					//2. Gatilho recebido, esperando DMA
	STATE_PROCESSING,						//3. DMA completo, processando dados
	STATE_CHECK_STATUS,						//4. Recebeu uma interrupção, tratando o que causou ela
	STATE_INIT
}DetectorState_t;

typedef struct{
	int32_t value;
	uint8_t channel;
}WaveformSample_t;

//================================================================================================================================================
//
//													Definições das funções utilizadas
//
//================================================================================================================================================
void ADE9000_Init_WFB();
void ADE9000_Trigger_Detector();
void Analisar_Forma_Onda_Capturada(uint16_t eventos,uint16_t trigger_addr_capturado);
void getRealValues2(int32_t raw32, int word_in_sample);
int32_t Sample_Linear_Remontado(uint16_t index_linear, uint16_t trigger_addr_capturado);

void UartTransmit(void);

#endif
