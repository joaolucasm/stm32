#ifndef CALIBRATION_H
#define CALIBRATION_H
/*  -------------------------------- Includes ----------------------------------- */
#include <stdint.h>
#include "ADE9000.h"
#include <math.h>
#include "stm32f4xx_hal.h"
#include <inttypes.h>


/*  -------------------------------- Defines ----------------------------------- */
#define FULLSCALERMSCODES       52702092
#define FULLSCALEPOWERCODES     20694066

#define BURDENRESISTOR          10.2f
#define VOLTAGEDIVIDERR1		800000
#define VOLTAGEDIVIDERR2		1000
#define TEMPOPOT				1
#define TEMPOPOTAT				10

#define V_offset 			    0.01
#define I_offset 				0.001
#define Freq_line 				60
#define Freq_dsp 				8000
#define I_Nominal				0.05
#define V_Nominal				100
#define TC_RATIO 				1
#define CTF 					((float)BURDENRESISTOR / (float)TC_RATIO) // 10.2 / 1 = 10.2
#define VTF 					((float)VOLTAGEDIVIDERR2 / (float)VOLTAGEDIVIDERR1) // 1000 / 800000 = 0.00125

#define I_ADC_Nominal 			(CTF * I_Nominal) // 10.2 * 0.05 = 0.51
#define V_ADC_Nominal 			(VTF * V_Nominal) // 0.00125 * 100 = 0.125
#define I_FSP_Nominal 			(I_ADC_Nominal / 0.707) // 0.51 / 0.707 = 0.72135785
#define V_FSP_Nominal			(V_ADC_Nominal / 0.707) // 0.125 / 0.707 = 0.17680339

#define I_ADC_Offset 			(CTF * I_offset )
#define V_ADC_Offset			(VTF * V_offset )
#define I_FSP_Offset 			(I_ADC_Offset / 0.707)
#define V_FSP_Offset 			(V_ADC_Offset / 0.707)

/*
typedef struct FSP{
	double CTF;
	double VTF;
	float I_FSP_Nominal;
	float V_FSP_Nominal;
	float I_FSP_Offset;
	float V_FSP_Offset;
	uint32_t validFlag;
}FSP_t;
*/


typedef struct calibrationRegisters{

	int32_t NEW_AIGAIN;
	int32_t NEW_BIGAIN;
	int32_t NEW_CIGAIN;
	int32_t NEW_AVGAIN;
	int32_t NEW_BVGAIN;
	int32_t NEW_CVGAIN;

	int32_t NEW_AIRMSOS;
	int32_t NEW_BIRMSOS;
	int32_t NEW_CIRMSOS;
	int32_t NEW_AVRMSOS;
	int32_t NEW_BVRMSOS;
	int32_t NEW_CVRMSOS;

	int32_t NEW_APHCAL0;
	int32_t NEW_BPHCAL0;
	int32_t NEW_CPHCAL0;

	int32_t NEW_APGAIN;
	int32_t NEW_BPGAIN;
	int32_t NEW_CPGAIN;

	int32_t NEW_AWATTOS;
	int32_t NEW_BWATTOS;
	int32_t NEW_CWATTOS;

	int32_t NEW_AVAROS;
	int32_t NEW_BVAROS;
	int32_t NEW_CVAROS;

	float CAL_IRMS_CC;
	float CAL_VRMS_CC;
	float CAL_POWER_CC;
	float CAL_ENERGY_CC;

}calibrationRegisters_t;


void calibracaoGanhoRMS(calibrationRegisters_t *medicao);
void calibracaoOffsetRMS(calibrationRegisters_t *medicao);
void calibracaoFase(calibrationRegisters_t *medicao);
void calibracaoPotencia(calibrationRegisters_t *medicao);
void calibrarOffsetPotenciaAtiva(calibrationRegisters_t *medicao);
void calibrarOffsetPotenciaReativa(calibrationRegisters_t *medicao);
void configuracaoRegistradores(int valorAcumulacao,int EP_CFG,int EGY_TIME);
void ADE9000_Calibracao(calibrationRegisters_t *medicao);

#endif
