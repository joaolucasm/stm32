/* Includes ------------------------------------------------------------------*/
#include "ADE9000API.h"

void ADE9000API_ReadActivePowerRegsValues(ActivePowerRegsValues *Data)
{
	Data->ActivePowerReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AWATT);
	Data->ActivePowerReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BWATT);
	Data->ActivePowerReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CWATT);
	Data->ActivePowerValue_A = (float)Data->ActivePowerReg_A * POWER_CONVERSION_CONSTANT;
	Data->ActivePowerValue_B = (float)Data->ActivePowerReg_B * POWER_CONVERSION_CONSTANT;
	Data->ActivePowerValue_C = (float)Data->ActivePowerReg_C * POWER_CONVERSION_CONSTANT;
}


void ADE9000API_ReadReactivePowerRegsValues(ReactivePowerRegsValues *Data)
{
	Data->ReactivePowerReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AVAR);
	Data->ReactivePowerReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BVAR);
	Data->ReactivePowerReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CVAR);
	Data->ReactivePowerValue_A = (float)Data->ReactivePowerReg_A * POWER_CONVERSION_CONSTANT;
	Data->ReactivePowerValue_B = (float)Data->ReactivePowerReg_B * POWER_CONVERSION_CONSTANT;
	Data->ReactivePowerValue_C = (float)Data->ReactivePowerReg_C * POWER_CONVERSION_CONSTANT;
}


void ADE9000API_ReadApparentPowerRegsValues(ApparentPowerRegsValues *Data)
{
	Data->ApparentPowerReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AVA);
	Data->ApparentPowerReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BVA);
	Data->ApparentPowerReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CVA);
	Data->ApparentPowerValue_A = (float)Data->ApparentPowerReg_A * POWER_CONVERSION_CONSTANT;
	Data->ApparentPowerValue_B = (float)Data->ApparentPowerReg_B * POWER_CONVERSION_CONSTANT;
	Data->ApparentPowerValue_C = (float)Data->ApparentPowerReg_C * POWER_CONVERSION_CONSTANT;
}


void ADE9000API_ReadActiveEnergyRegsValues(ActiveEnergyRegsValues *Data)
{
	Data->ActiveEnergyReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AWATTHR_HI);
	Data->ActiveEnergyReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BWATTHR_HI);
	Data->ActiveEnergyReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CWATTHR_HI);
	Data->ActiveEnergyValue_A = (float)Data->ActiveEnergyReg_A * ENERGY_CONVERSION_CONSTANT;
	Data->ActiveEnergyValue_B = (float)Data->ActiveEnergyReg_B * ENERGY_CONVERSION_CONSTANT;
	Data->ActiveEnergyValue_C = (float)Data->ActiveEnergyReg_C * ENERGY_CONVERSION_CONSTANT;
}


void ADE9000API_ReadReactiveEnergyRegsValues(ReactiveEnergyRegsValues *Data)
{
	Data->ReactiveEnergyReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AVARHR_HI);
	Data->ReactiveEnergyReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BVARHR_HI);
	Data->ReactiveEnergyReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CVARHR_HI);
	Data->ReactiveEnergyValue_A = (float)Data->ReactiveEnergyReg_A * ENERGY_CONVERSION_CONSTANT;
	Data->ReactiveEnergyValue_B = (float)Data->ReactiveEnergyReg_B * ENERGY_CONVERSION_CONSTANT;
	Data->ReactiveEnergyValue_C = (float)Data->ReactiveEnergyReg_C * ENERGY_CONVERSION_CONSTANT;

}


void ADE9000API_ReadApparentEnergyRegsValues(ApparentEnergyRegsValues *Data)
{
	Data->ApparentEnergyReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AVAHR_HI);
	Data->ApparentEnergyReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BVAHR_HI);
	Data->ApparentEnergyReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CVAHR_HI);
	Data->ApparentEnergyValue_A = (float)Data->ApparentEnergyReg_A * ENERGY_CONVERSION_CONSTANT;
	Data->ApparentEnergyValue_B = (float)Data->ApparentEnergyReg_B * ENERGY_CONVERSION_CONSTANT;
	Data->ApparentEnergyValue_C = (float)Data->ApparentEnergyReg_C * ENERGY_CONVERSION_CONSTANT;
}


void ADE9000API_ReadVoltageRMSRegsValues(VoltageRMSRegsValues *Data)
{
	Data->VoltageRMSReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AVRMS);
	Data->VoltageRMSReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BVRMS);
	Data->VoltageRMSReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CVRMS);
	Data->VoltageRMSValue_A = (float)Data->VoltageRMSReg_A * VOLTAGE_CONVERSION_CONSTANT;
	Data->VoltageRMSValue_B = (float)Data->VoltageRMSReg_B * VOLTAGE_CONVERSION_CONSTANT;
	Data->VoltageRMSValue_C = (float)Data->VoltageRMSReg_C * VOLTAGE_CONVERSION_CONSTANT;
}


void ADE9000API_ReadCurrentRMSRegsValues(CurrentRMSRegsValues *Data)
{
	Data->CurrentRMSReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AIRMS);
	Data->CurrentRMSReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BIRMS);
	Data->CurrentRMSReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CIRMS);
	Data->CurrentRMSReg_N = (int32_t) ADE9000SPI_Read32(ADDR_NIRMS);
	Data->CurrentRMSValue_A = (float)Data->CurrentRMSReg_A * CURRENT_CONVERSION_CONSTANT;
	Data->CurrentRMSValue_B = (float)Data->CurrentRMSReg_B * CURRENT_CONVERSION_CONSTANT;
	Data->CurrentRMSValue_C = (float)Data->CurrentRMSReg_C * CURRENT_CONVERSION_CONSTANT;
	Data->CurrentRMSValue_N = (float)Data->CurrentRMSReg_N * CURRENT_CONVERSION_CONSTANT;
}


void ADE9000API_ReadFundActivePowerRegsValues(FundActivePowerRegsValues *Data)
{
	Data->FundActivePowerReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AFWATT);
	Data->FundActivePowerReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BFWATT);
	Data->FundActivePowerReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CFWATT);
	Data->FundActivePowerValue_A = (float)Data->FundActivePowerReg_A * POWER_CONVERSION_CONSTANT;
	Data->FundActivePowerValue_B = (float)Data->FundActivePowerReg_B * POWER_CONVERSION_CONSTANT;
	Data->FundActivePowerValue_C = (float)Data->FundActivePowerReg_C * POWER_CONVERSION_CONSTANT;
}


void ADE9000API_ReadFundReactivePowerRegsValues(FundReactivePowerRegsValues *Data)
{
	Data->FundReactivePowerReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AFVAR);
	Data->FundReactivePowerReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BFVAR);
	Data->FundReactivePowerReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CFVAR);
	Data->FundReactivePowerValue_A = (float)Data->FundReactivePowerReg_A * POWER_CONVERSION_CONSTANT;
	Data->FundReactivePowerValue_B = (float)Data->FundReactivePowerReg_B * POWER_CONVERSION_CONSTANT;
	Data->FundReactivePowerValue_C = (float)Data->FundReactivePowerReg_C * POWER_CONVERSION_CONSTANT;
}


void ADE9000API_ReadFundApparentPowerRegsValues(FundApparentPowerRegsValues *Data)
{
	Data->FundApparentPowerReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AFVA);
	Data->FundApparentPowerReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BFVA);
	Data->FundApparentPowerReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CFVA);
	Data->FundApparentPowerValue_A = (float)Data->FundApparentPowerReg_A * POWER_CONVERSION_CONSTANT;
	Data->FundApparentPowerValue_B = (float)Data->FundApparentPowerReg_B * POWER_CONVERSION_CONSTANT;
	Data->FundApparentPowerValue_C = (float)Data->FundApparentPowerReg_C * POWER_CONVERSION_CONSTANT;
}


void ADE9000API_ReadFundActiveEnergyRegsValues(FundActiveEnergyRegsValues *Data)
{
	Data->FundActiveEnergyReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AFWATTHR_HI);
	Data->FundActiveEnergyReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BFWATTHR_HI);
	Data->FundActiveEnergyReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CFWATTHR_HI);
	Data->FundActiveEnergyValue_A = (float)Data->FundActiveEnergyReg_A * ENERGY_CONVERSION_CONSTANT;
	Data->FundActiveEnergyValue_B = (float)Data->FundActiveEnergyReg_B * ENERGY_CONVERSION_CONSTANT;
	Data->FundActiveEnergyValue_C = (float)Data->FundActiveEnergyReg_C * ENERGY_CONVERSION_CONSTANT;
}


void ADE9000API_ReadFundReactiveEnergyRegsValues(FundReactiveEnergyRegsValues *Data)
{
	Data->FundReactiveEnergyReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AFVARHR_HI);
	Data->FundReactiveEnergyReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BFVARHR_HI);
	Data->FundReactiveEnergyReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CFVARHR_HI);
	Data->FundReactiveEnergyValue_A = (float)Data->FundReactiveEnergyReg_A * ENERGY_CONVERSION_CONSTANT;
	Data->FundReactiveEnergyValue_B = (float)Data->FundReactiveEnergyReg_B * ENERGY_CONVERSION_CONSTANT;
	Data->FundReactiveEnergyValue_C = (float)Data->FundReactiveEnergyReg_C * ENERGY_CONVERSION_CONSTANT;
}


void ADE9000API_ReadFundApparentEnergyRegsValues(FundApparentEnergyRegsValues *Data)
{
	Data->FundApparentEnergyReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AFVAHR_HI);
	Data->FundApparentEnergyReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BFVAHR_HI);
	Data->FundApparentEnergyReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CFVAHR_HI);
	Data->FundApparentEnergyValue_A = (float)Data->FundApparentEnergyReg_A * ENERGY_CONVERSION_CONSTANT;
	Data->FundApparentEnergyValue_B = (float)Data->FundApparentEnergyReg_B * ENERGY_CONVERSION_CONSTANT;
	Data->FundApparentEnergyValue_C = (float)Data->FundApparentEnergyReg_C * ENERGY_CONVERSION_CONSTANT;
}


void ADE9000API_ReadFundVoltageRMSRegsValues(FundVoltageRMSRegsValues *Data)
{
	Data->FundVoltageRMSReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AVFRMS);
	Data->FundVoltageRMSReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BVFRMS);
	Data->FundVoltageRMSReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CVFRMS);
	Data->FundVoltageRMSValue_A = (float)Data->FundVoltageRMSReg_A * VOLTAGE_CONVERSION_CONSTANT;
	Data->FundVoltageRMSValue_B = (float)Data->FundVoltageRMSReg_B * VOLTAGE_CONVERSION_CONSTANT;
	Data->FundVoltageRMSValue_C = (float)Data->FundVoltageRMSReg_C * VOLTAGE_CONVERSION_CONSTANT;
}


void ADE9000API_ReadFundCurrentRMSRegsValues(FundCurrentRMSRegsValues *Data)
{
	Data->FundCurrentRMSReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AIFRMS);
	Data->FundCurrentRMSReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BIFRMS);
	Data->FundCurrentRMSReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CIFRMS);
	Data->FundCurrentRMSValue_A = (float)Data->FundCurrentRMSReg_A * CURRENT_CONVERSION_CONSTANT;
	Data->FundCurrentRMSValue_B = (float)Data->FundCurrentRMSReg_B * CURRENT_CONVERSION_CONSTANT;
	Data->FundCurrentRMSValue_C = (float)Data->FundCurrentRMSReg_C * CURRENT_CONVERSION_CONSTANT;
}


void ADE9000API_ReadHalfVoltageRMSRegsValues(HalfVoltageRMSRegsValues *Data)
{
	Data->HalfVoltageRMSReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AVRMSONE);
	Data->HalfVoltageRMSReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BVRMSONE);
	Data->HalfVoltageRMSReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CVRMSONE);
	Data->HalfVoltageRMSValue_A = (float)Data->HalfVoltageRMSReg_A * VOLTAGE_CONVERSION_CONSTANT;
	Data->HalfVoltageRMSValue_A = (float)Data->HalfVoltageRMSReg_A * VOLTAGE_CONVERSION_CONSTANT;
	Data->HalfVoltageRMSValue_A = (float)Data->HalfVoltageRMSReg_A * VOLTAGE_CONVERSION_CONSTANT;
}


void ADE9000API_ReadHalfCurrentRMSRegsValues(HalfCurrentRMSRegsValues *Data)
{
	Data->HalfCurrentRMSReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AIRMSONE);
	Data->HalfCurrentRMSReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BIRMSONE);
	Data->HalfCurrentRMSReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CIRMSONE);
	Data->HalfCurrentRMSReg_N = (int32_t) ADE9000SPI_Read32(ADDR_NIRMSONE);
	Data->HalfCurrentRMSValue_A = (float)Data->HalfCurrentRMSReg_A * CURRENT_CONVERSION_CONSTANT;
	Data->HalfCurrentRMSValue_B = (float)Data->HalfCurrentRMSReg_B * CURRENT_CONVERSION_CONSTANT;
	Data->HalfCurrentRMSValue_C = (float)Data->HalfCurrentRMSReg_C * CURRENT_CONVERSION_CONSTANT;
	Data->HalfCurrentRMSValue_N = (float)Data->HalfCurrentRMSReg_N * CURRENT_CONVERSION_CONSTANT;
}


void ADE9000API_ReadTen12VoltageRMSRegsValues(Ten12VoltageRMSRegsValues *Data)
{
	Data->Ten12VoltageRMSReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AVRMS1012);
	Data->Ten12VoltageRMSReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BVRMS1012);
	Data->Ten12VoltageRMSReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CVRMS1012);
	Data->Ten12VoltageRMSValue_A = (float)Data->Ten12VoltageRMSReg_A * VOLTAGE_CONVERSION_CONSTANT;
	Data->Ten12VoltageRMSValue_B = (float)Data->Ten12VoltageRMSReg_B * VOLTAGE_CONVERSION_CONSTANT;
	Data->Ten12VoltageRMSValue_C = (float)Data->Ten12VoltageRMSReg_C * VOLTAGE_CONVERSION_CONSTANT;
}


void ADE9000API_ReadTen12CurrentRMSRegsValues(Ten12CurrentRMSRegsValues *Data)
{
	Data->Ten12CurrentRMSReg_A = (int32_t) ADE9000SPI_Read32(ADDR_AIRMS1012);
	Data->Ten12CurrentRMSReg_B = (int32_t) ADE9000SPI_Read32(ADDR_BIRMS1012);
	Data->Ten12CurrentRMSReg_C = (int32_t) ADE9000SPI_Read32(ADDR_CIRMS1012);
	Data->Ten12CurrentRMSReg_N = (int32_t) ADE9000SPI_Read32(ADDR_NIRMS1012);
	Data->Ten12CurrentRMSValue_A = (float)Data->Ten12CurrentRMSReg_A * CURRENT_CONVERSION_CONSTANT;
	Data->Ten12CurrentRMSValue_B = (float)Data->Ten12CurrentRMSReg_B * CURRENT_CONVERSION_CONSTANT;
	Data->Ten12CurrentRMSValue_C = (float)Data->Ten12CurrentRMSReg_C * CURRENT_CONVERSION_CONSTANT;
	Data->Ten12CurrentRMSValue_N = (float)Data->Ten12CurrentRMSReg_N * CURRENT_CONVERSION_CONSTANT;
}


void ADE9000API_ReadVoltageTHDRegsnValues(VoltageTHDRegsValues *Data)
{
	Data->VoltageTHDReg_A = (int32_t)ADE9000SPI_Read32(ADDR_AVTHD);
	Data->VoltageTHDValue_A = (float)Data->VoltageTHDReg_A * 100 / (float)134217728;

	Data->VoltageTHDReg_B = (int32_t)ADE9000SPI_Read32(ADDR_BVTHD);
	Data->VoltageTHDValue_B = (float)Data->VoltageTHDReg_B * 100 / (float)134217728;

	Data->VoltageTHDReg_C = (int32_t)ADE9000SPI_Read32(ADDR_CVTHD);
	Data->VoltageTHDValue_C = (float)Data->VoltageTHDReg_C * 100 / (float)134217728;
}


void ADE9000API_ReadCurrentTHDRegsnValues(CurrentTHDRegsValues *Data)
{
	Data->CurrentTHDReg_A = (int32_t)ADE9000SPI_Read32(ADDR_AITHD);
	Data->CurrentTHDValue_A = (float)Data->CurrentTHDReg_A * 100 / (float)134217728;

	Data->CurrentTHDReg_B = (int32_t)ADE9000SPI_Read32(ADDR_BITHD);
	Data->CurrentTHDValue_B = (float)Data->CurrentTHDReg_B * 100 / (float)134217728;

	Data->CurrentTHDReg_C = (int32_t)ADE9000SPI_Read32(ADDR_CITHD);
	Data->CurrentTHDValue_C = (float)Data->CurrentTHDReg_C * 100 / (float)134217728;
}


void ADE9000API_ReadPowerFactorRegsnValues(PowerFactorRegsValues *Data)
{
	Data->PowerFactorReg_A = (int32_t)ADE9000SPI_Read32(ADDR_APF);
	Data->PowerFactorValue_A = (float)Data->PowerFactorReg_A * 100/ (float)134217728;

	Data->PowerFactorReg_B = (int32_t)ADE9000SPI_Read32(ADDR_BPF);
	Data->PowerFactorValue_B = (float)Data->PowerFactorReg_B * 100/ (float)134217728;

	Data->PowerFactorReg_C = (int32_t)ADE9000SPI_Read32(ADDR_CPF);
	Data->PowerFactorValue_C = (float)Data->PowerFactorReg_C * 100/ (float)134217728;
}


void ADE9000API_ReadPeriodRegsnValues(PeriodRegsValues *Data)
{
	Data->PeriodReg_A = (int32_t)ADE9000SPI_Read32(ADDR_APERIOD);
	Data->FrequencyValue_A = (float)(8000 * 65536) / (float)(Data->PeriodReg_A + 1);

	Data->PeriodReg_B = (int32_t)ADE9000SPI_Read32(ADDR_BPERIOD);
	Data->FrequencyValue_B = (float)(8000 * 65536) / (float)(Data->PeriodReg_B + 1);

	Data->PeriodReg_C = (int32_t)ADE9000SPI_Read32(ADDR_CPERIOD);
	Data->FrequencyValue_C = (float)(8000 * 65536) / (float)(Data->PeriodReg_C + 1);
}


void ADE9000API_ReadAngleRegsnValues(AngleRegsValues *Data)
{
	float mulConstant;

	uint16_t temp = ADE9000SPI_Read16(ADDR_ACCMODE); 						//L� registrador de configura��o da frequ�ncia.

	if((temp&0x0100) >= 0)
	{
		mulConstant = 0.02109375;  											// Constante de multiplica��o para sistema a 60 Hz.
	}
	else
	{
		mulConstant = 0.017578125; 											// Constante de multiplica��o para sistema a 50 Hz.
	}

	Data->AngleReg_VA_VB = (int16_t)ADE9000SPI_Read16(ADDR_ANGL_VA_VB);
	Data->AngleValue_VA_VB = Data->AngleReg_VA_VB * mulConstant;

	Data->AngleReg_VB_VC = (int16_t)ADE9000SPI_Read16(ADDR_ANGL_VB_VC);
	Data->AngleValue_VB_VC = Data->AngleReg_VB_VC * mulConstant;

	Data->AngleReg_VA_VC = (int16_t)ADE9000SPI_Read16(ADDR_ANGL_VA_VC);
	Data->AngleValue_VA_VC = Data->AngleReg_VA_VC * mulConstant;

	Data->AngleReg_VA_IA = (int16_t)ADE9000SPI_Read16(ADDR_ANGL_VA_IA);
	Data->AngleValue_VA_IA = Data->AngleReg_VA_IA * mulConstant;

	Data->AngleReg_VB_IB = (int16_t)ADE9000SPI_Read16(ADDR_ANGL_VB_IB);
	Data->AngleValue_VB_IB = Data->AngleReg_VB_IB * mulConstant;

	Data->AngleReg_VC_IC = (int16_t)ADE9000SPI_Read16(ADDR_ANGL_VC_IC);
	Data->AngleValue_VC_IC = Data->AngleReg_VC_IC * mulConstant;

	Data->AngleReg_IA_IB = (int16_t)ADE9000SPI_Read16(ADDR_ANGL_IA_IB);
	Data->AngleValue_IA_IB = Data->AngleReg_IA_IB * mulConstant;

	Data->AngleReg_IB_IC = (int16_t) ADE9000SPI_Read16(ADDR_ANGL_IB_IC);
	Data->AngleValue_IB_IC = Data->AngleReg_IB_IC * mulConstant;

	Data->AngleReg_IA_IC = (int16_t)ADE9000SPI_Read16(ADDR_ANGL_IA_IC);
	Data->AngleValue_IA_IC = Data->AngleReg_IA_IC * mulConstant;
}

void ADE9000API_ReadPowerQuality(PowerQuality *Data)
{
	uint32_t STATUS1 = ADE9000SPI_Read32(ADDR_STATUS1);

	/*DIP and SWELL*/

	uint32_t Bit_SWELLA = 20;
	uint32_t Bit_SWELLB = 21;
	uint32_t Bit_SWELLC = 22;
	uint32_t Bit_DIPA = 23;
	uint32_t Bit_DIPB = 24;
	uint32_t Bit_DIPC = 25;

	Bit_SWELLA = 1 << Bit_SWELLA;
	Bit_SWELLB = 1 << Bit_SWELLB;
	Bit_SWELLC = 1 << Bit_SWELLC;
	Bit_DIPA = 1 << Bit_DIPA;
	Bit_DIPB = 1 << Bit_DIPB;
	Bit_DIPC = 1 << Bit_DIPC;

	if (Bit_SWELLA & STATUS1)
	{
		Data->SWELLA = 1;
		ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
	}
	if (Bit_SWELLB & STATUS1)
	{
		Data->SWELLB = 1;
		ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
	}
	if (Bit_SWELLC & STATUS1)
	{
		Data->SWELLC = 1;
		ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
	}
	if (Bit_DIPA & STATUS1)
	{
		Data->DIPA = 1;
		ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
	}
	if (Bit_DIPB & STATUS1)
	{
		Data->DIPB = 1;
		ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
	}
	if (Bit_DIPC & STATUS1)
	{
		Data->DIPC = 1;
		ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
	}

	/*Overcurrent*/

	uint32_t Bit_OI = 17;
	Bit_OI = 1 << Bit_OI;

	if (Bit_OI & STATUS1)
	{
		uint16_t OISTATUS = ADE9000SPI_Read16(ADDR_OISTATUS);

		uint16_t Bit_OIA = 0;
		uint16_t Bit_OIB = 1;
		uint16_t Bit_OIC = 2;
		uint16_t Bit_OIN = 3;

		Bit_OIA = 1 << Bit_OIA;
		Bit_OIB = 1 << Bit_OIB;
		Bit_OIC = 1 << Bit_OIC;
		Bit_OIN = 1 << Bit_OIN;

		if (Bit_OIA & OISTATUS)
		{
			Data->OIA = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_OIB & OISTATUS)
		{
			Data->OIB = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_OIC & OISTATUS)
		{
			Data->OIC = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_OIN & OISTATUS)
		{
			Data->OIN = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}
	}

	/*No load condition*/

	uint32_t Bit_VAFNOLOAD = 5;
	uint32_t Bit_RFNOLOAD = 4;
	uint32_t Bit_AFNOLOAD = 3;
	uint32_t Bit_VANLOAD = 2;
	uint32_t Bit_RNLOAD = 1;
	uint32_t Bit_ANLOAD = 0;

	Bit_VAFNOLOAD = 1 << Bit_VAFNOLOAD;
	Bit_RFNOLOAD = 1 << Bit_RFNOLOAD;
	Bit_AFNOLOAD = 1 << Bit_AFNOLOAD;
	Bit_VANLOAD = 1 << Bit_VANLOAD;
	Bit_RNLOAD = 1 << Bit_RNLOAD;
	Bit_ANLOAD = 1 << Bit_ANLOAD;

	if (Bit_VAFNOLOAD & STATUS1)
	{
		uint32_t PHNOLOAD = ADE9000SPI_Read32(ADDR_PHNOLOAD);

		uint32_t Bit_AFVANL = 5;
		uint32_t Bit_BFVANL = 11;
		uint32_t Bit_CFVANL = 17;

		Bit_AFVANL = 1 << Bit_AFVANL;
		Bit_BFVANL = 1 << Bit_BFVANL;
		Bit_CFVANL = 1 << Bit_CFVANL;

		if (Bit_AFVANL & PHNOLOAD)
		{
			Data->AFVANL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_BFVANL & PHNOLOAD)
		{
			Data->BFVANL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_CFVANL & PHNOLOAD)
		{
			Data->CFVANL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

	}
	if (Bit_RFNOLOAD & STATUS1)
	{
		uint32_t PHNOLOAD = ADE9000SPI_Read32(ADDR_PHNOLOAD);

		uint32_t Bit_AFVARNL = 4;
		uint32_t Bit_BFVARNL = 10;
		uint32_t Bit_CFVARNL = 16;

		Bit_AFVARNL = 1 << Bit_AFVARNL;
		Bit_BFVARNL = 1 << Bit_BFVARNL;
		Bit_CFVARNL = 1 << Bit_CFVARNL;

		if (Bit_AFVARNL & PHNOLOAD)
		{
			Data->AFVARNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_BFVARNL & PHNOLOAD)
		{
			Data->BFVARNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_CFVARNL & PHNOLOAD)
		{
			Data->CFVARNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}
	}
	if (Bit_AFNOLOAD & STATUS1)
	{
		uint32_t PHNOLOAD = ADE9000SPI_Read32(ADDR_PHNOLOAD);

		uint32_t Bit_AFWATTNL = 3;
		uint32_t Bit_BFWATTNL = 9;
		uint32_t Bit_CFWATTNL= 15;

		Bit_AFWATTNL = 1 << Bit_AFWATTNL;
		Bit_BFWATTNL = 1 << Bit_BFWATTNL;
		Bit_CFWATTNL = 1 << Bit_CFWATTNL;

		if (Bit_AFWATTNL & PHNOLOAD)
		{
			Data->AFWATTNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_BFWATTNL & PHNOLOAD)
		{
			Data->BFWATTNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_CFWATTNL & PHNOLOAD)
		{
			Data->CFWATTNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}
	}
	if (Bit_VANLOAD & STATUS1)
	{
		uint32_t PHNOLOAD = ADE9000SPI_Read32(ADDR_PHNOLOAD);

		uint32_t Bit_AVANL = 2;
		uint32_t Bit_BVANL = 8;
		uint32_t Bit_CVANL= 14;

		Bit_AVANL = 1 << Bit_AVANL;
		Bit_BVANL = 1 << Bit_BVANL;
		Bit_CVANL = 1 << Bit_CVANL;

		if (Bit_AVANL & PHNOLOAD)
		{
			Data->AVANL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_BVANL & PHNOLOAD)
		{
			Data->BVANL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_CVANL & PHNOLOAD)
		{
			Data->CVANL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}
	}
	if (Bit_RNLOAD & STATUS1)
	{
		uint32_t PHNOLOAD = ADE9000SPI_Read32(ADDR_PHNOLOAD);

		uint32_t Bit_AVARNL = 1;
		uint32_t Bit_BVARNL = 7;
		uint32_t Bit_CVARNL = 13;

		Bit_AVARNL = 1 << Bit_AVARNL;
		Bit_BVARNL = 1 << Bit_BVARNL;
		Bit_CVARNL = 1 << Bit_CVARNL;

		if (Bit_AVARNL & PHNOLOAD)
		{
			Data->AVARNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_BVARNL & PHNOLOAD)
		{
			Data->BVARNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_CVARNL & PHNOLOAD)
		{
			Data->CVARNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}
	}
	if (Bit_ANLOAD & STATUS1)
	{
		uint32_t PHNOLOAD = ADE9000SPI_Read32(ADDR_PHNOLOAD);

		uint32_t Bit_AWATTNL = 0;
		uint32_t Bit_BWATTNL = 6;
		uint32_t Bit_CWATTNL= 12;

		Bit_AWATTNL = 1 << Bit_AWATTNL;
		Bit_BWATTNL = 1 << Bit_BWATTNL;
		Bit_CWATTNL = 1 << Bit_CWATTNL;

		if (Bit_AWATTNL & PHNOLOAD)
		{
			Data->AWATTNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_BWATTNL & PHNOLOAD)
		{
			Data->BWATTNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}

		if (Bit_CWATTNL & PHNOLOAD)
		{
			Data->CWATTNL = 1;
			ADE9000SPI_Write32(ADDR_STATUS1, 0xFFFFFFFF);
		}
	}
}
