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
#define DSP_FS_DECIMAL 						74532013.0
#define PIN_FS_VOLTAGE						1.0
#define FATOR_DIVISOR_TENSAO				801.0
#define FATOR_DIVISOR_CORRENTE				(1.0/2.55)										/*Fator de conversão do sensor de corrente (1 / R_burden)*/
#define SCALE_TO_MILLI_UNITS				1000.0											/*Fator de enviar para a fila int32_t (mV e mA em vez de V e A)*/

#define STATUS1_OI_BIT						(1 << 17)										/*Bit de overcurrent no registrador status1*/
#define OISTATUS_IA_BIT						(1 << 0)										/*Bit de indicando a fase A no registrador oistatus*/
#define OISTATUS_IB_BIT						(1 << 1)										/*Bit de indicando a fase B no registrador oistatus*/
#define OISTATUS_IC_BIT						(1 << 2)										/*Bit de indicando a fase C no registrador oistatus*/
#define STATUS1_SWELLA_BIT					(1 << 20)										/*Bit de SWELL no canal A no registrador status1*/
#define STATUS1_SWELLB_BIT					(1 << 21)										/*Bit de SWELL no canal B no registrador status1*/
#define STATUS1_SWELLC_BIT					(1 << 22)										/*Bit de SWELL no canal C no registrador status1*/
#define STATUS1_DIPA_BIT					(1 << 23)										/*Bit de DIP no canal A no registrador status1*/
#define STATUS1_DIPB_BIT					(1 << 24)										/*Bit de DIP no canal B no registrador status1*/
#define STATUS1_DIPC_BIT					(1 << 25)										/*Bit de DIP no canal C no registrador status1*/
#define STATUS1_ZXTOVA_BIT 					(1 << 6)										/*Bit de zero crossing timeout no canal A no registrador status1*/
#define STATUS1_ZXTOVB_BIT 					(1 << 7)										/*Bit de zero crossing timeout no canal B no registrador status1*/
#define STATUS1_ZXTOVC_BIT 					(1 << 8)										/*Bit de zero crossing timeout no canal C no registrador status1*/


#define STATUS0_PAGE_FULL					(1 << 17)										/*Bit de status0 que avisa se uma página está cheia*/
#define STATUS0_WFB_TRIG_IRQ				(1 << 16)										/*Bit de status0 que avisa a parada do buffer*/

#define ADDR_WFB_BASE						0x00000800		 								/*Start of Waveform Buffer*/
#define ADDR_WFB_TOP						0x00000FFF										/*End of Waveform Buffer*/

#define DMA_SAMPLE_SETS						128                         					/*Leitura de 64 amostras do ADE por vez*/
#define DMA_WORDS_PER_SAMPLE				14												/*Canais para leitura 7 canais * High/Low parts*/
#define DMA_BUFFER_SIZE_16_BITS				(DMA_SAMPLE_SETS * DMA_WORDS_PER_SAMPLE * 2) 	/*Tamanho do buffer que vai comportar os dados que serão trazidos do ADE, multiplicado por 2 pois será de 16 bits*/

#define DECIMATION_FACTOR					8												/*Para pegar 500ms de onda com 20Kbytes, onde faremos 8000Hz / 8 = 1000Hz*/
#define DECIMATION_FACTOR_PRINT				4

// RAM Usada: 700 * 28 bytes = 19.600 bytes (~19.1 KB)
// Tempo Total: 700 / 1333.33 = ~525 ms
#define HISTORY_BUFFER_SIZE					700
#define HISTORY_CHANNELS					7
#define PRINT_BUFFER_SIZE					100

#define FLAG_CAPTURE_COMPLETE				0x01											/*Flag para acordar a task*/
#define MONITORING_PERIOD_MS				1000

//#define FACTOR_I (float)((PIN_FS_VOLTAGE * FATOR_DIVISOR_CORRENTE * SCALE_TO_MILLI_UNITS) / DSP_FS_DECIMAL)
//#define FACTOR_V (float)((PIN_FS_VOLTAGE *  FATOR_DIVISOR_TENSAO  * SCALE_TO_MILLI_UNITS) / DSP_FS_DECIMAL)

#define FACTOR_I (float)((PIN_FS_VOLTAGE * FATOR_DIVISOR_CORRENTE) / DSP_FS_DECIMAL)
#define FACTOR_V (float)((PIN_FS_VOLTAGE * FATOR_DIVISOR_TENSAO) / DSP_FS_DECIMAL)

#define FACTOR_I_DECIMADO  			(float)	(FACTOR_I / DECIMATION_FACTOR)
#define FACTOR_V_DECIMADO  			(float)	(FACTOR_V / DECIMATION_FACTOR)
#define FACTOR_I_DECIMADO_PRINT  	(float)	(FACTOR_I / DECIMATION_FACTOR_PRINT)
#define FACTOR_V_DECIMADO_PRINT  	(float)	(FACTOR_V / DECIMATION_FACTOR_PRINT)


#define ADE_GET_VALUE(ade_union) \
	( (int32_t)( ((uint32_t)(ade_union).parts.High << 16) | (ade_union).parts.Low ) )
//================================================================================================================================================
//
//												Definições das estruturas de dados utilizadas
//
//================================================================================================================================================

typedef enum{
	WFB_STATE_INIT,							//Iniciação do ADE9000 e do DMA
	WFB_STATE_STREAMING,					//Estado passivo: DMA fica preenchendo o buffer, aguardando um trigger
	WFB_STATE_POST_TRIGGER,					//(Gerenciado na ISR) Capturando o final da onda
	WFB_STATE_PROCESSING,					//DMA parado. Extraindo dados e enviando para a fila
	WFB_STATE_RESTART						//Limpa buffers e reinicia o processo
}WaveformState_t;

typedef struct{
	int32_t value;
	uint8_t channel;
}WaveformSample_t;

#pragma pack(push,1)
typedef union {
    struct {
        uint16_t High;
        uint16_t Low;
    } parts;
    int32_t val;
} ade_wfb_sample;

typedef struct{
	ade_wfb_sample IA;
	ade_wfb_sample VA;
	ade_wfb_sample IB;
	ade_wfb_sample VB;
	ade_wfb_sample IC;
	ade_wfb_sample VC;
	ade_wfb_sample IN;
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

//================================================================================================================================================
//
//													Definições das funções utilizadas
//
//================================================================================================================================================
void Init_WFB(void);
void Restart_WFB(void);
void Waveform_Main(void);
void Waveform_Process_Chunk(uint16_t start_index,uint16_t count);
uint16_t Analise_Interrupcao(void);
void Reset_Internal_State(void);
void mainPrint_Waveform(void);
void UartTransmit(void);

#endif
