/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADE9000_H
#define __ADE9000_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ADE9000API.h"
/* Defines -------------------------------------------------------------------*/

// Configur��o do ADE9000
#define ADE9000_PGA_GAIN 					0x0000    	    /*Ganho em todos os canais = 1*/
#define ADE9000_CONFIG0 					0x00000080		/*Desabilita o integrador, mas ativando o bit para o cálculo de ciclo de rms1/2*/
#define ADE9000_CONFIG1						0x0002			/*Sa�da do pino CF3/ZX � o cruzamento por zero*/
#define ADE9000_CONFIG2						0x0C00			/*Frequ�ncia do filtro passa alta = 1,25Hz*/
#define ADE9000_CONFIG3						0xE000			/*Habilita detec��o de sobre corrente em todos canais, menos em IN*/
#define ADE9000_ACCMODE 					0x0100			/*Opera��o a 60 Hz, 3 fases e 4 fios, acumula��o de energia com sinal (energia pode ser negativa)*/
#define ADE9000_TEMP_CFG 					0x0000			/*Sensor de temperatura habilitado*/
#define ADE9000_ZX_LP_SEL 					0x001E			/*Periodo e cruzamento obtidos pelos sinais combinados VA, VB e VC*/
#define ADE9000_MASK0 						0x00030000		/*Habilita interrupcao de PAGE_FULL e WFB_TRIG_IRQ*/
#define ADE9000_MASK1						0x00000000		/*Não habilito nenhuma interrupção para ocorrer em IRQ1, controlando tudo apenas por IRQ0*/
#define ADE9000_EVENT_MASK 					0x00000000		/*Eventos desabilitados*/
#define ADE9000_VLEVEL						0x001637EB		/*Assume-se que Vnom = 1/2 da escala cheia*/
#define ADE9000_EGY_TIME 					0x0001 			/*Acumula 1 amostras, 1/8000 segundo de acumula��o*/
#define ADE9000_EP_CFG 						0xC021			/*Habilita o acumulador de energia, sempre que a interrup��o EGYRDY � acionada o registrador interno de energia adiciona-se ao registrador do usu�rio, o modo sem carga � acionado com 4096 samples, tempo m�ximo de acumula��o � 106 seg*/
#define ADE9000_WFB_CFG_CONFIGURATION 		0x03A0			/*Modo burst da waveform habilitada em todos os canais, captura desabilitada, fixed data rate data, continuous filling(10), Waveforms Processed by the DSP source, IN disable*/
#define ADE9000_WFB_CFG_STOP 				0x03A0			/*Mesma coisa que o configuration, apenas para poder resetar a captura de dados do WFB*/
#define ADE9000_WFB_CFG_START 				0x03B0			/*Habilitando a leitura de preenchimento do waveform Buffer*/
#define ADE9000_WFB_TRG_CFG					0x0007			/*Habilita o trigger do buffer para quando ocorrer SWELL, DIP ou OI em qualquer fase*/
#define ADE9000_WFB_PQ_IRQEN				0x8000			/*Habilita a interrupcao quando a ultima página do buffer encher*/
#define ADE9000_16BIT_BLANK					0x0000

// Limites do modulo de qualidade
#define DIP_LVL					0x00007694					/*N�vel inferior de tens�o que caracteriza DIP (21V rms)*/
#define DIP_CYC					0x0020						/*N�mero m�nimo de ciclos para caracterizar DIP*/
#define SWELL_LVL				0x0000DC37					/*N�vel superior de tens�o que caracteriza SWELL (39V rms)*/
#define SWELL_CYC				0x0020						/*N�mero m�nimo de ciclos para caracterizar SWELL*/
#define OI_LVL                  0x00121519					/*N�vel de corrente que caracteriza sobre corrente (0,050A rms)*/
#define ACT_NL_LVL				0x6778						/*N�vel inferior de pot�ncia at�va que caracteriza opera��o sem carga*/
#define REACT_NL_LVL			0x6778						/*N�vel inferior de pot�ncia reativa que caracteriza opera��o sem carga*/
#define APP_NL_LVL				0x6778						/*N�vel inferior de pot�ncia aparente que caracteriza opera��o sem carga*/

// Constantes
#define ADE90xx_FDSP 			8000   						/*ADE9000 FDSP: 8000sps*/
#define ADE9000_RUN_ON 			0x0001						/*DSP ON*/


/* Structs --------------------------------------------------------------------*/
typedef struct ADE9000_EnergyMeasurements
{
	ActivePowerRegsValues			ActivePower;
	ReactivePowerRegsValues			ReactivePower;
	ApparentPowerRegsValues			ApparentPower;
	ActiveEnergyRegsValues			ActiveEnergy;
	ReactiveEnergyRegsValues		ReactiveEnergy;
	ApparentEnergyRegsValues		ApparentEnergy;
	VoltageRMSRegsValues			VoltageRMS;
	CurrentRMSRegsValues			CurrentRMS;
	FundActivePowerRegsValues		FundActivePower;
	FundReactivePowerRegsValues		FundReactivePower;
	FundApparentPowerRegsValues		FundApparentPower;
	FundActiveEnergyRegsValues		FundActiveEnergy;
	FundReactiveEnergyRegsValues	FundReactiveEnergy;
	FundApparentEnergyRegsValues	FundApparentEnergy;
	FundVoltageRMSRegsValues		FundVoltageRMS;
	FundCurrentRMSRegsValues		FundCurrentRMS;
	VoltageTHDRegsValues			VoltageTHD;
	CurrentTHDRegsValues			CurrentTHD;
	PowerFactorRegsValues			PowerFactor;
	PeriodRegsValues				Period;
	AngleRegsValues					Angle;
	PowerQuality					Quality;
} ADE9000_EnergyMeasurements;

/* Function prototypes ----------------------------------------------------------*/
void ADE9000_Init(void);
void ADE9000_Measurements(ADE9000_EnergyMeasurements *Data);

#ifdef __cplusplus
}
#endif

#endif /*__ ADE9000_H */
