#ifndef WAVEFORM_H
#define WAVEFORM_H

//================================================================================================================================================
//
//																	Includes
//
//================================================================================================================================================
#include <stdio.h>
#include <stdint.h>
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

// 1. Buffer de Transporte (DMA)
#define DMA_SAMPLE_SETS						64                          /*Leitura de 64 amostras do ADE por vez*/
#define DMA_CHANNELS_SPI					8							/*Canais para leitura (IA,VA,IB,VB,IC,VC,IN,Spare)*/
#define DMA_BUFFER_SIZE_16_BITS				(DMA_SAMPLE_SETS * DMA_CHANNELS_SPI * 2) /*Tamanho do buffer que vai comportar os dados que serão trazidos do ADE, multiplicado por 2 pois será de 16 bits*/

// 2. Decimação
#define DECIMATION_FACTOR					6							/*Para pegar 500ms de onda com 20Kbytes, onde faremos 8000Hz / 6 = 1333.33Hz*/

// 3. Buffer Circular de Histórico (RAM STM32)
// RAM Usada: 700 * 28 bytes = 19.600 bytes (~19.1 KB)
// Tempo Total: 700 / 1333.33 = ~525 ms
#define HISTORY_BUFFER_SIZE					700
#define HISTORY_CHANNELS					7							/*IA,VA,IB,VB,IC,VC,IN*/



#define FACTOR_I (float)((PIN_FS_VOLTAGE * FATOR_DIVISOR_CORRENTE * SCALE_TO_MILLI_UNITS) / DSP_FS_DECIMAL)
#define FACTOR_V (float)((PIN_FS_VOLTAGE *  FATOR_DIVISOR_TENSAO  * SCALE_TO_MILLI_UNITS) / DSP_FS_DECIMAL)

#define ADE_GET_VALUE(ade_union) \
	( (int32_t)( ((uint32_t)(ade_union).parts.High << 16) | (ade_union).parts.Low ) )
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

#pragma pack(push,1)
typedef struct{
	int32_t IA;
	int32_t VA;
	int32_t IB;
	int32_t VB;
	int32_t IC;
	int32_t VC;
	int32_t IN;
	int32_t Spare;
}AdeRawSample_t;
#pragma pack(pop)

typedef struct{
	int32_t IA;
	int32_t VA;
	int32_t IB;
	int32_t VB;
	int32_t IC;
	int32_t VC;
	int32_t IN;
}ProcessedSample_t;

extern uint16_t dma_rx_buffer[DMA_BUFFER_SIZE_16_BITS];

extern ProcessedSample_t history_buffer[HISTORY_BUFFER_SIZE];

extern volatile uint16_t history_head;

extern volatile bool flag_dma_half;
extern volatile bool flag_dma_cplt;

//================================================================================================================================================
//
//													Definições das funções utilizadas
//
//================================================================================================================================================
void ADE9000_Init_WFB();
void ADE9000_Trigger_Detector();
void Waveform_Process_Loop(void);



void UartTransmit(void);

#endif
