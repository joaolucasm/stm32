/*  ---------------------------------- Includes ------------------------------------- */
#include "calibration.h"

extern UART_HandleTypeDef huart2;
/* ------------------------------------ Código ------------------------------------- */

//==================================================================================================================================================
//
//		 													Função utilizada para calcular o valor RMS de corrente
//
//==================================================================================================================================================
void calibracaoGanhoRMS(calibrationRegisters_t *medicao)
{
	//------------------------------------------------------------------------------------------------------------------------------------
	//Geração dos valores dos registradores: AIGAIN, BIGAIN, CIGAIN, AVGAIN, BVGAIN e CVGAIN
	//------------------------------------------------------------------------------------------------------------------------------------

	int32_t AIRMS_MESURED = ADE9000SPI_Read32(ADDR_AIRMS);
	int32_t BIRMS_MESURED = ADE9000SPI_Read32(ADDR_BIRMS);
	int32_t CIRMS_MESURED = ADE9000SPI_Read32(ADDR_CIRMS);
	int32_t AVRMS_MESURED = ADE9000SPI_Read32(ADDR_AVRMS);
	int32_t BVRMS_MESURED = ADE9000SPI_Read32(ADDR_BVRMS);
	int32_t CVRMS_MESURED = ADE9000SPI_Read32(ADDR_CVRMS);

	int32_t IRMS_EXPECTED = (int32_t)(I_FSP_Nominal * FULLSCALERMSCODES);

	int32_t VRMS_EXPECTED = (int32_t)(V_FSP_Nominal * FULLSCALERMSCODES);

	double GAIN;

	//Calculo do AIGAIN
	GAIN = (double)IRMS_EXPECTED / (double)AIRMS_MESURED;
	medicao->NEW_AIGAIN = (int32_t)((GAIN - 1) * pow(2.0,27.0));

	//Calculo do BIGAIN
	GAIN = (double)IRMS_EXPECTED / (double)BIRMS_MESURED;
	medicao->NEW_BIGAIN = (GAIN - 1) * pow(2.0,27.0);

	//Calculo do CIGAIN
	GAIN = (double)IRMS_EXPECTED / (double)CIRMS_MESURED;
	medicao->NEW_CIGAIN = (GAIN - 1) * pow(2.0,27.0);

	//Calculo do AVGAIN
	GAIN = (double)VRMS_EXPECTED / (double)AVRMS_MESURED;
	medicao->NEW_AVGAIN = (GAIN - 1) * pow(2.0,27.0);

	//Calculo do BVGAIN
	GAIN = (double)VRMS_EXPECTED / (double)BVRMS_MESURED;
	medicao->NEW_BVGAIN = (GAIN - 1) * pow(2.0,27.0);

	//Calculo do CVGAIN
	GAIN = (double)VRMS_EXPECTED / (double)CVRMS_MESURED;
	medicao->NEW_CVGAIN = (GAIN - 1) * pow(2.0,27.0);

}

//==================================================================================================================================================
//
//		 													Função utilizada para calcular o valor
//																de offset de RMS da tensão
//
//==================================================================================================================================================
void calibracaoOffsetRMS(calibrationRegisters_t *medicao)
{
	//------------------------------------------------------------------------------------------------------------------------------------
	//Geração dos valores dos registradores: AIRMSOS, BIRMSOS, CIRMSOS, AVRMSOS, BVRMSOS e CVRMSOS
	//------------------------------------------------------------------------------------------------------------------------------------

	int32_t AIRMS_MESURED = ADE9000SPI_Read32(ADDR_AIRMS);
	int32_t BIRMS_MESURED = ADE9000SPI_Read32(ADDR_BIRMS);
	int32_t CIRMS_MESURED = ADE9000SPI_Read32(ADDR_CIRMS);
	int32_t AVRMS_MESURED = ADE9000SPI_Read32(ADDR_AVRMS);
	int32_t BVRMS_MESURED = ADE9000SPI_Read32(ADDR_BVRMS);
	int32_t CVRMS_MESURED = ADE9000SPI_Read32(ADDR_CVRMS);

	int32_t IRMS_EXPECTED = I_FSP_Offset * FULLSCALERMSCODES;

	int32_t VRMS_EXPECTED = V_FSP_Offset * FULLSCALERMSCODES;

	//Calculo do registrador AIRMSOS:
	medicao->NEW_AIRMSOS = (pow(IRMS_EXPECTED,2.0) - pow(AIRMS_MESURED,2.0)) / pow(2.0,15.0);

	//Calculo do registrador BIRMSOS:
	medicao->NEW_BIRMSOS = (pow(IRMS_EXPECTED,2.0) - pow(BIRMS_MESURED,2.0)) / pow(2.0,15.0);

	//Calculo do registrador CIRMSOS:
	medicao->NEW_CIRMSOS = (pow(IRMS_EXPECTED,2.0) - pow(CIRMS_MESURED,2.0)) / pow(2.0,15.0);

	//Calculo do registrador AVRMSOS:
	medicao->NEW_AVRMSOS = (pow(VRMS_EXPECTED,2.0) - pow(AVRMS_MESURED,2.0)) / pow(2.0,15.0);

	//Calculo do registrador BVRMSOS:
	medicao->NEW_BVRMSOS = (pow(VRMS_EXPECTED,2.0) - pow(BVRMS_MESURED,2.0)) / pow(2.0,15.0);

	//Calculo do registrador CVRMSOS:
	medicao->NEW_CVRMSOS = (pow(VRMS_EXPECTED,2.0) - pow(CVRMS_MESURED,2.0)) / pow(2.0,15.0);

}

//==================================================================================================================================================
//
//		 													Função utilizada para a calibração das Fases
//
//==================================================================================================================================================
void calibracaoFase(calibrationRegisters_t *medicao)
{
	//------------------------------------------------------------------------------------------------------------------------------------
	//Geração dos valores dos registradores: APHCAL0, BPHCAL0 e CPHCAL0
	//OBS IMPORTANTE: Aplique os valores de I nominal e V nominal, com um fator de potência de atraso de 0.5.
	//------------------------------------------------------------------------------------------------------------------------------------

	int32_t AWATTHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_AWATTHR_HI);
	int32_t BWATTHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_BWATTHR_HI);
	int32_t CWATTHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_CWATTHR_HI);

	int32_t AVARHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_AVARHR_HI);
	int32_t BVARHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_BVARHR_HI);
	int32_t CVARHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_CVARHR_HI);


	double fp = 0.5;

	double w = 2 * M_PI * Freq_line / Freq_dsp;

	//Erro na fase A:
	double APHASE_ERROR = -atan((AWATTHR_HI_MEASURED * sin(acos(fp)) - (AVARHR_HI_MEASURED * cos(acos(fp)))) / (AWATTHR_HI_MEASURED * cos(acos(fp)) + (AVARHR_HI_MEASURED * sin(acos(fp)))));

	//Erro na fase B:
	double BPHASE_ERROR = -atan((BWATTHR_HI_MEASURED * sin(acos(fp)) - (BVARHR_HI_MEASURED * cos(acos(fp)))) / (BWATTHR_HI_MEASURED * cos(acos(fp)) + (BVARHR_HI_MEASURED * sin(acos(fp)))));

	//Erro na fase C:
	double CPHASE_ERROR = -atan((CWATTHR_HI_MEASURED * sin(acos(fp)) - (CVARHR_HI_MEASURED * cos(acos(fp)))) / (CWATTHR_HI_MEASURED * cos(acos(fp)) + (CVARHR_HI_MEASURED * sin(acos(fp)))));


	//Calculo do registrador APHCAL0:
	medicao->NEW_APHCAL0 = ((sin(APHASE_ERROR - w) + sin(w)) / sin(2 * w - APHASE_ERROR)) * pow (2.0,27.0);

	//Calculo do registrador BPHCAL0:
	medicao->NEW_BPHCAL0 = ((sin(BPHASE_ERROR - w) + sin(w)) / sin(2 * w - BPHASE_ERROR)) * pow (2.0,27.0);

	//Calculo do registrador CPHCAL0:
	medicao->NEW_CPHCAL0 = ((sin(CPHASE_ERROR - w) + sin(w)) / sin(2 * w - CPHASE_ERROR)) * pow (2.0,27.0);

}

//==================================================================================================================================================
//
//		 													Função utilizada para a calibração de potência
//
//==================================================================================================================================================
void calibracaoPotencia(calibrationRegisters_t *medicao)
{
	//------------------------------------------------------------------------------------------------------------------------------------
	//Geração dos valores dos registradores: APGAIN, BPGAIN e CPGAIN
	//OBS IMPORTANTE: Aplique os valores de I nominal e V nominal, com um fator de potência de 1.
	//------------------------------------------------------------------------------------------------------------------------------------

	int32_t AWATTHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_AWATTHR_HI);
	int32_t BWATTHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_BWATTHR_HI);
	int32_t CWATTHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_CWATTHR_HI);

	int32_t WATTHR_HI_EXPECTED = TEMPOPOT * I_FSP_Nominal * V_FSP_Nominal * FULLSCALEPOWERCODES * Freq_dsp * pow(2.0,-13.0);

	//Calculo do registrador APGAIN:
	medicao->NEW_APGAIN = ((WATTHR_HI_EXPECTED - AWATTHR_HI_MEASURED) / AWATTHR_HI_MEASURED) * pow(2.0,27.0);

	//Calculo do registrador BPGAIN:
	medicao->NEW_BPGAIN = ((WATTHR_HI_EXPECTED - BWATTHR_HI_MEASURED) / BWATTHR_HI_MEASURED) * pow(2.0,27.0);

	//Calculo do registrador APGAIN:
	medicao->NEW_CPGAIN = ((WATTHR_HI_EXPECTED - CWATTHR_HI_MEASURED) / CWATTHR_HI_MEASURED) * pow(2.0,27.0);
}

//==================================================================================================================================================
//
//		 													Função utilizada para a calibração do Offset da potência ativa
//
//==================================================================================================================================================
void calibrarOffsetPotenciaAtiva(calibrationRegisters_t *medicao)
{
	//------------------------------------------------------------------------------------------------------------------------------------
	//Geração dos valores dos registradores: AWATTOS, BWATTOS e CWATTOS
	//OBS IMPORTANTE 1: Para fazer a calibração do Offset de corrente, é necessário uma baixa corrente nominal!!!!!!
	//OBS IMPORTANTE 2: Aplique os valores de I nominal e V nominal, com um fator de potência de 1.
	//------------------------------------------------------------------------------------------------------------------------------------
	int32_t AWATTHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_AWATTHR_HI);
	int32_t BWATTHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_BWATTHR_HI);
	int32_t CWATTHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_CWATTHR_HI);

	int32_t WATTHR_HI_EXPECTED = TEMPOPOTAT * I_FSP_Offset * V_FSP_Nominal * FULLSCALEPOWERCODES * Freq_dsp * pow(2.0,-13.0);

	//Calculo do registrador AWATTOS:
	medicao->NEW_AWATTOS = (WATTHR_HI_EXPECTED - AWATTHR_HI_MEASURED) / (TEMPOPOTAT * Freq_dsp * pow(2.0,-13.0));

	//Calculo do registrador BWATTOS:
	medicao->NEW_BWATTOS = (WATTHR_HI_EXPECTED - BWATTHR_HI_MEASURED) / (TEMPOPOTAT * Freq_dsp * pow(2.0,-13.0));

	//Calculo do registrador CWATTOS:
	medicao->NEW_CWATTOS = (WATTHR_HI_EXPECTED - CWATTHR_HI_MEASURED) / (TEMPOPOTAT * Freq_dsp * pow(2.0,-13.0));
}

//==================================================================================================================================================
//
//		 													Função utilizada para a calibração do Offset da potência reativa
//
//==================================================================================================================================================
void calibrarOffsetPotenciaReativa(calibrationRegisters_t *medicao)
{
	//------------------------------------------------------------------------------------------------------------------------------------
	//Geração dos valores dos registradores: AVAROS, BVAROS e CVAROS
	//OBS IMPORTANTE 1: Para fazer a calibração do Offset de corrente, é necessário uma baixa corrente nominal!!!!!!
	//OBS IMPORTANTE 2: Aplique os valores de I nominal e V nominal, com um fator de potência de 0.
	//------------------------------------------------------------------------------------------------------------------------------------

	int32_t AVARHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_AVARHR_HI);
	int32_t BVARHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_BVARHR_HI);
	int32_t CVARHR_HI_MEASURED = ADE9000SPI_Read32(ADDR_CVARHR_HI);

	double VARHR_EXPECTED = TEMPOPOTAT * I_FSP_Offset * V_FSP_Nominal * FULLSCALEPOWERCODES * Freq_dsp * pow(2.0,-13.0);

	//Calculo do registrador AVAROS:
	medicao->NEW_AVAROS = (VARHR_EXPECTED - AVARHR_HI_MEASURED) / (TEMPOPOTAT * Freq_dsp * pow(2.0,-13.0));

	//Calculo do registrador BVAROS:
	medicao->NEW_BVAROS = (VARHR_EXPECTED - BVARHR_HI_MEASURED) / (TEMPOPOTAT * Freq_dsp * pow(2.0,-13.0));

	//Calculo do registrador CVAROS:
	medicao->NEW_CVAROS = (VARHR_EXPECTED - CVARHR_HI_MEASURED) / (TEMPOPOTAT * Freq_dsp * pow(2.0,-13.0));
}

void configuracaoRegistradores(int valorAcumulacao,int EP_CFG,int EGY_TIME)
{
	ADE9000SPI_Write16(ADDR_EP_CFG,EP_CFG);
	ADE9000SPI_Write16(ADDR_EGY_TIME,EGY_TIME);
	HAL_Delay(valorAcumulacao);


}
