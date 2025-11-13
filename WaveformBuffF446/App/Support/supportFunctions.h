/* --------------------------------- Includes ---------------------------------- */
#include "enernet.h"
#include "app_config.h"
#include <stdbool.h>
#include "waveform.h"

/*  -------------------------------- Defines ----------------------------------- */
void AppMainBuild_General_InitiateVarStateMachine(StateMachineData_t *d_StateMachineApp_cp);
void AppMainBuild_AppEnernet_HardwareInitiate(StateMachineData_t *d_StateMachineApp_cp);
bool AppMainBuild_General_TakeMeasurement(StateMachineData_t *d_StateMachineApp_cp);
int  AppMainBuild_AppEnernet_TakeElectricalMeasurement(StateMachineData_t *d_StateMachineApp_cp);
void AppMainBuild_Print(StateMachineData_t *d_StateMachineApp_cp);
void AppMainBuild_Print_Display(ADE9000_EnergyMeasurements *data);
