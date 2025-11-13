/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADE9000API_H
#define __ADE9000API_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ADE9000MAP.h"
#include "ADE9000SPI.h"
#include "ADE9000CAL.h"

/* Structs --------------------------------------------------------------------*/
typedef struct ActivePowerRegsValues
{
	int32_t ActivePowerReg_A;
	int32_t ActivePowerReg_B;
	int32_t ActivePowerReg_C;
	float 	ActivePowerValue_A;
	float 	ActivePowerValue_B;
	float 	ActivePowerValue_C;
} ActivePowerRegsValues;

typedef struct ReactivePowerRegsValues
{
	int32_t ReactivePowerReg_A;
	int32_t ReactivePowerReg_B;
	int32_t ReactivePowerReg_C;
	float	ReactivePowerValue_A;
	float 	ReactivePowerValue_B;
	float 	ReactivePowerValue_C;
} ReactivePowerRegsValues;

typedef struct ApparentPowerRegsValues
{
	int32_t ApparentPowerReg_A;
	int32_t ApparentPowerReg_B;
	int32_t ApparentPowerReg_C;
	float 	ApparentPowerValue_A;
	float 	ApparentPowerValue_B;
	float 	ApparentPowerValue_C;
} ApparentPowerRegsValues;

typedef struct ActiveEnergyRegsValues
{
	int32_t ActiveEnergyReg_A;
	int32_t ActiveEnergyReg_B;
	int32_t ActiveEnergyReg_C;
	float 	ActiveEnergyValue_A;
	float 	ActiveEnergyValue_B;
	float 	ActiveEnergyValue_C;
} ActiveEnergyRegsValues;

typedef struct ReactiveEnergyRegsValues
{
	int32_t ReactiveEnergyReg_A;
	int32_t ReactiveEnergyReg_B;
	int32_t ReactiveEnergyReg_C;
	float 	ReactiveEnergyValue_A;
	float 	ReactiveEnergyValue_B;
	float 	ReactiveEnergyValue_C;
} ReactiveEnergyRegsValues;

typedef struct ApparentEnergyRegsValues
{
	int32_t ApparentEnergyReg_A;
	int32_t ApparentEnergyReg_B;
	int32_t ApparentEnergyReg_C;
	float 	ApparentEnergyValue_A;
	float 	ApparentEnergyValue_B;
	float 	ApparentEnergyValue_C;
} ApparentEnergyRegsValues;

typedef struct VoltageRMSRegsValues
{
	int32_t VoltageRMSReg_A;
	int32_t VoltageRMSReg_B;
	int32_t VoltageRMSReg_C;
	float 	VoltageRMSValue_A;
	float 	VoltageRMSValue_B;
	float	VoltageRMSValue_C;
} VoltageRMSRegsValues;

typedef struct CurrentRMSRegsValues
{
	int32_t CurrentRMSReg_A;
	int32_t CurrentRMSReg_B;
	int32_t CurrentRMSReg_C;
	int32_t CurrentRMSReg_N;
	float 	CurrentRMSValue_A;
	float 	CurrentRMSValue_B;
	float 	CurrentRMSValue_C;
	float 	CurrentRMSValue_N;
} CurrentRMSRegsValues;

typedef struct FundActivePowerRegsValues
{
	int32_t FundActivePowerReg_A;
	int32_t FundActivePowerReg_B;
	int32_t FundActivePowerReg_C;
	float 	FundActivePowerValue_A;
	float 	FundActivePowerValue_B;
	float 	FundActivePowerValue_C;
} FundActivePowerRegsValues;

typedef struct FundReactivePowerRegsValues
{
	int32_t FundReactivePowerReg_A;
	int32_t FundReactivePowerReg_B;
	int32_t FundReactivePowerReg_C;
	float 	FundReactivePowerValue_A;
	float 	FundReactivePowerValue_B;
	float 	FundReactivePowerValue_C;
} FundReactivePowerRegsValues;

typedef struct FundApparentPowerRegsValues
{
	int32_t FundApparentPowerReg_A;
	int32_t FundApparentPowerReg_B;
	int32_t FundApparentPowerReg_C;
	float 	FundApparentPowerValue_A;
	float 	FundApparentPowerValue_B;
	float 	FundApparentPowerValue_C;
} FundApparentPowerRegsValues;

typedef struct FundActiveEnergyRegsValues
{
	int32_t FundActiveEnergyReg_A;
	int32_t FundActiveEnergyReg_B;
	int32_t FundActiveEnergyReg_C;
	float 	FundActiveEnergyValue_A;
	float 	FundActiveEnergyValue_B;
	float 	FundActiveEnergyValue_C;
} FundActiveEnergyRegsValues;

typedef struct FundReactiveEnergyRegsValues
{
	int32_t FundReactiveEnergyReg_A;
	int32_t FundReactiveEnergyReg_B;
	int32_t FundReactiveEnergyReg_C;
	float 	FundReactiveEnergyValue_A;
	float 	FundReactiveEnergyValue_B;
	float 	FundReactiveEnergyValue_C;
} FundReactiveEnergyRegsValues;

typedef struct FundApparentEnergyRegsValues
{
	int32_t FundApparentEnergyReg_A;
	int32_t FundApparentEnergyReg_B;
	int32_t FundApparentEnergyReg_C;
	float 	FundApparentEnergyValue_A;
	float 	FundApparentEnergyValue_B;
	float 	FundApparentEnergyValue_C;
} FundApparentEnergyRegsValues;

typedef struct FundVoltageRMSRegsValues
{
	int32_t FundVoltageRMSReg_A;
	int32_t FundVoltageRMSReg_B;
	int32_t FundVoltageRMSReg_C;
	float 	FundVoltageRMSValue_A;
	float 	FundVoltageRMSValue_B;
	float 	FundVoltageRMSValue_C;
} FundVoltageRMSRegsValues;

typedef struct FundCurrentRMSRegsValues
{
	int32_t FundCurrentRMSReg_A;
	int32_t FundCurrentRMSReg_B;
	int32_t FundCurrentRMSReg_C;
	float 	FundCurrentRMSValue_A;
	float 	FundCurrentRMSValue_B;
	float 	FundCurrentRMSValue_C;
} FundCurrentRMSRegsValues;

typedef struct HalfVoltageRMSRegsValues
{
	int32_t HalfVoltageRMSReg_A;
	int32_t HalfVoltageRMSReg_B;
	int32_t HalfVoltageRMSReg_C;
	float 	HalfVoltageRMSValue_A;
	float 	HalfVoltageRMSValue_B;
	float 	HalfVoltageRMSValue_C;
} HalfVoltageRMSRegsValues;

typedef struct HalfCurrentRMSRegsValues
{
	int32_t HalfCurrentRMSReg_A;
	int32_t HalfCurrentRMSReg_B;
	int32_t HalfCurrentRMSReg_C;
	int32_t HalfCurrentRMSReg_N;
	float 	HalfCurrentRMSValue_A;
	float 	HalfCurrentRMSValue_B;
	float 	HalfCurrentRMSValue_C;
	float 	HalfCurrentRMSValue_N;
} HalfCurrentRMSRegsValues;

typedef struct Ten12VoltageRMSRegsValues
{
	int32_t Ten12VoltageRMSReg_A;
	int32_t Ten12VoltageRMSReg_B;
	int32_t Ten12VoltageRMSReg_C;
	float 	Ten12VoltageRMSValue_A;
	float 	Ten12VoltageRMSValue_B;
	float 	Ten12VoltageRMSValue_C;
} Ten12VoltageRMSRegsValues;

typedef struct Ten12CurrentRMSRegsValues
{
	int32_t Ten12CurrentRMSReg_A;
	int32_t Ten12CurrentRMSReg_B;
	int32_t Ten12CurrentRMSReg_C;
	int32_t Ten12CurrentRMSReg_N;
	float 	Ten12CurrentRMSValue_A;
	float 	Ten12CurrentRMSValue_B;
	float 	Ten12CurrentRMSValue_C;
	float 	Ten12CurrentRMSValue_N;
} Ten12CurrentRMSRegsValues;

typedef struct VoltageTHDRegsValues
{
	int32_t VoltageTHDReg_A;
	int32_t VoltageTHDReg_B;
	int32_t VoltageTHDReg_C;
	float 	VoltageTHDValue_A;
	float 	VoltageTHDValue_B;
	float 	VoltageTHDValue_C;
} VoltageTHDRegsValues;

typedef struct CurrentTHDRegsValues
{
	int32_t CurrentTHDReg_A;
	int32_t CurrentTHDReg_B;
	int32_t CurrentTHDReg_C;
	float 	CurrentTHDValue_A;
	float 	CurrentTHDValue_B;
	float 	CurrentTHDValue_C;
} CurrentTHDRegsValues;


typedef struct PowerFactorRegsValues
{
	int32_t PowerFactorReg_A;
	int32_t PowerFactorReg_B;
	int32_t PowerFactorReg_C;
	float 	PowerFactorValue_A;
	float 	PowerFactorValue_B;
	float 	PowerFactorValue_C;
} PowerFactorRegsValues;

typedef struct PeriodRegsValues
{
	int32_t PeriodReg_A;
	int32_t PeriodReg_B;
	int32_t PeriodReg_C;
	float 	FrequencyValue_A;
	float	FrequencyValue_B;
	float	FrequencyValue_C;
} PeriodRegsValues;

typedef struct AngleRegsValues
{
	int16_t AngleReg_VA_VB;
	int16_t AngleReg_VB_VC;
	int16_t AngleReg_VA_VC;
	int16_t AngleReg_VA_IA;
	int16_t AngleReg_VB_IB;
	int16_t AngleReg_VC_IC;
	int16_t AngleReg_IA_IB;
	int16_t AngleReg_IB_IC;
	int16_t AngleReg_IA_IC;
	float 	AngleValue_VA_VB;
	float 	AngleValue_VB_VC;
	float 	AngleValue_VA_VC;
	float 	AngleValue_VA_IA;
	float 	AngleValue_VB_IB;
	float 	AngleValue_VC_IC;
	float 	AngleValue_IA_IB;
	float 	AngleValue_IB_IC;
	float 	AngleValue_IA_IC;
} AngleRegsValues;


typedef struct PowerQuality
{
	/*DIP*/
	int16_t DIPA;
	int16_t DIPB;
	int16_t DIPC;
	/*SWELL*/
	int16_t SWELLA;
	int16_t SWELLB;
	int16_t SWELLC;
	/*Over Current*/
	int16_t OIA;
	int16_t OIB;
	int16_t OIC;
	int16_t OIN;
	/*Fundamental apparent no load*/
	int16_t AFVANL;
	int16_t BFVANL;
	int16_t CFVANL;
	/*Fundamental reactive no load*/
	int16_t AFVARNL;
	int16_t BFVARNL;
	int16_t CFVARNL;
	/*Fundamental active no load*/
	int16_t AFWATTNL;
	int16_t BFWATTNL;
	int16_t CFWATTNL;
	/*Total apparent no load*/
	int16_t AVANL;
	int16_t BVANL;
	int16_t CVANL;
	/*Total reactive no load*/
	int16_t AVARNL;
	int16_t BVARNL;
	int16_t CVARNL;
	/*Total active no load*/
	int16_t AWATTNL;
	int16_t BWATTNL;
	int16_t CWATTNL;
} PowerQuality;



/* Function prototypes ----------------------------------------------------------*/
void ADE9000API_ReadActivePowerRegsValues(ActivePowerRegsValues *Data);
void ADE9000API_ReadReactivePowerRegsValues(ReactivePowerRegsValues *Data);
void ADE9000API_ReadApparentPowerRegsValues(ApparentPowerRegsValues *Data);
void ADE9000API_ReadActiveEnergyRegsValues(ActiveEnergyRegsValues *Data);
void ADE9000API_ReadReactiveEnergyRegsValues(ReactiveEnergyRegsValues *Data);
void ADE9000API_ReadApparentEnergyRegsValues(ApparentEnergyRegsValues *Data);
void ADE9000API_ReadVoltageRMSRegsValues(VoltageRMSRegsValues *Data);
void ADE9000API_ReadCurrentRMSRegsValues(CurrentRMSRegsValues *Data);
void ADE9000API_ReadFundActivePowerRegsValues(FundActivePowerRegsValues *Data);
void ADE9000API_ReadFundReactivePowerRegsValues(FundReactivePowerRegsValues *Data);
void ADE9000API_ReadFundApparentPowerRegsValues(FundApparentPowerRegsValues *Data);
void ADE9000API_ReadFundActiveEnergyRegsValues(FundActiveEnergyRegsValues *Data);
void ADE9000API_ReadFundReactiveEnergyRegsValues(FundReactiveEnergyRegsValues *Data);
void ADE9000API_ReadFundApparentEnergyRegsValues(FundApparentEnergyRegsValues *Data);
void ADE9000API_ReadFundVoltageRMSRegsValues(FundVoltageRMSRegsValues *Data);
void ADE9000API_ReadFundCurrentRMSRegsValues(FundCurrentRMSRegsValues *Data);
void ADE9000API_ReadHalfVoltageRMSRegsValues(HalfVoltageRMSRegsValues *Data);
void ADE9000API_ReadHalfCurrentRMSRegsValues(HalfCurrentRMSRegsValues *Data);
void ADE9000API_ReadTen12VoltageRMSRegsValues(Ten12VoltageRMSRegsValues *Data);
void ADE9000API_ReadTen12CurrentRMSRegsValues(Ten12CurrentRMSRegsValues *Data);
void ADE9000API_ReadVoltageTHDRegsnValues(VoltageTHDRegsValues *Data);
void ADE9000API_ReadCurrentTHDRegsnValues(CurrentTHDRegsValues *Data);
void ADE9000API_ReadPowerFactorRegsnValues(PowerFactorRegsValues *Data);
void ADE9000API_ReadPeriodRegsnValues(PeriodRegsValues *Data);
void ADE9000API_ReadAngleRegsnValues(AngleRegsValues *Data);
void ADE9000API_ReadPowerQuality(PowerQuality *Data);

#ifdef __cplusplus
}
#endif

#endif /*__ ADE9000API_H */
