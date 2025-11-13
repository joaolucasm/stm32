#include "supportFunctions.h"
#include "app_config.h"
#include "ADE9000.h"
#include <math.h>
#include <string.h>
#include "LumenProtocol.h"

//=======================================================================================================================================================
	//--------------------------------------- Usada apenas para iniciar algumas variaveis da maq de estados da app --------------------------------------
	//OBS:
	//
	//PARAMETROS:
	//===================================================================================================================================================
void AppMainBuild_General_InitiateVarStateMachine(StateMachineData_t *d_StateMachineApp_cp)
{
	d_StateMachineApp_cp->StatusRunning = STATE_APP_RUNNING_NORMAL;
	d_StateMachineApp_cp->contTempoNotificacaoEnvMsgMin = 0;
	d_StateMachineApp_cp->contTempoEntreLeiturasSeg = 0;
	d_StateMachineApp_cp->tempoAppEmExecucaoDias = 0;
	d_StateMachineApp_cp->tempoAppEmExecucaoSeg = 0;
	d_StateMachineApp_cp->tempoAppEmExecucaoMin = 0;
	d_StateMachineApp_cp->tempoAppEmExecucaoHoras = 0;
	d_StateMachineApp_cp->contTempoLifeTimeMin = 0;
	d_StateMachineApp_cp->contTempoEmFalhaMinu = 0;
	d_StateMachineApp_cp->QtdTentativasReConexao = 0;
	d_StateMachineApp_cp->ConnectedElectricMeterInterface = false;
	d_StateMachineApp_cp->habExecucaoApp = false;
	d_StateMachineApp_cp->freqCalculoSeg = TIM_LEITURA;
	d_StateMachineApp_cp->habStaticCalcApp = true;
	return;
}
//====================================================================================================================================================
//----------------------------------------- Inicializa o hardware de medicao  -----------------------------------------------
	// PARAMETROS:
	//               devices_to_initiate:
	//                       0 = inicializa somente ADE (medidas eletricas)
	//                       1 = inicializa somente ADS (temperratura)
	//                       2 = inicializa todos (ADE e ADS)
	// OBS:
	//===================================================================================================================================================
void AppMainBuild_AppEnernet_HardwareInitiate(StateMachineData_t *d_StateMachineApp_cp)
{
	if(!APP_LIBS_MODE_MOC)
	{
		ADE9000_Init();
		d_StateMachineApp_cp->ConnectedElectricMeterInterface = true;
		d_StateMachineApp_cp->habExecucaoApp = true;
	}
	else
	{

		//d_StateMachineApp_cp->ConnectedElectricMeterInterface = true;
	}
	return;
}
//==================================================================================================================================================
//
//		 										Função para gerenciar a conexão e medidas elétricas
//
//==================================================================================================================================================

bool AppMainBuild_General_TakeMeasurement(StateMachineData_t *d_StateMachineApp_cp)
{
		int resp2 = 0;
		static uint8_t contTentativasConexaoHardTerceiros = 0;

		if(d_StateMachineApp_cp->ConnectedElectricMeterInterface)
		{
			if(APP_LIBS_MODE_MOC)
			{
				//resp2 = AppMainBuild_AppEnernet_GenerateSyntheticData(d_StateMachineApp_cp, d_AlarmsFactorTest_cp, SYNTHETIC_DATA_TYPE); //so para testes da app sem se preocupar com medidas verdadeiras
			}
			else
			{
				if(d_StateMachineApp_cp->ConnectedElectricMeterInterface)
					resp2 = AppMainBuild_AppEnernet_TakeElectricalMeasurement(d_StateMachineApp_cp);
			}
				//processa os acumuladores de energia:
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.AcumuladorEnergiaAparenteTotal_kva = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.AcumuladorEnergiaAparenteTotal_kva + ((d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ApparentEnergy.ApparentEnergyValue_A + d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ApparentEnergy.ApparentEnergyValue_B + d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ApparentEnergy.ApparentEnergyValue_C)/1000.0f);  //converte em Kilo
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.AcumuladorEnergiaAtivaTotal_kw = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.AcumuladorEnergiaAtivaTotal_kw         + ((d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActiveEnergy.ActiveEnergyValue_A     + d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActiveEnergy.ActiveEnergyValue_B     + d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActiveEnergy.ActiveEnergyValue_C)/1000.0f);  //converte em Kilo
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.AcumuladorEnergiaReativaTotal_kvar = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.AcumuladorEnergiaReativaTotal_kvar + ((d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ReactiveEnergy.ReactiveEnergyValue_A + d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ReactiveEnergy.ReactiveEnergyValue_B + d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ReactiveEnergy.ReactiveEnergyValue_C)/1000.0f);  //converte em Kilo
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.AcumuladorEnergiaAtiva24h = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.AcumuladorEnergiaAtiva24h + ((d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActiveEnergy.ActiveEnergyValue_A + d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActiveEnergy.ActiveEnergyValue_B + d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActiveEnergy.ActiveEnergyValue_C)/1000.0f);  //converte em Kilo
				//lembrando que as variavies acumuladoras de 24h e notificacao sao zeradas pelas funcoes de processamento estatistico toda vez que mandamos pacotes delas.
		}

		if(resp2>0) //medidas realizadas com sucesso
		{

			d_StateMachineApp_cp->novaLeituraMedidas = true;
			contTentativasConexaoHardTerceiros = 0;

			return true; //deu certo a leitura
		}
		else //nao foi possivel fazer medidas com o hardware de terceiros depois de algumas tentativas
		{

			#if APP_DEBUG_LOG_USING_USB_APP
				  printf("\n# Falha ao tentar ler medidas do hard. terceiro!. Tentando reconetar\n");
			#endif
			d_StateMachineApp_cp->novaLeituraMedidas = false;
			contTentativasConexaoHardTerceiros++;

			//PARTE 1: INICIAR CONEXAO COM HARDWARE DE TERCEIROS:
			AppMainBuild_AppEnernet_HardwareInitiate(d_StateMachineApp_cp); //inicializa hardware: ADE

			//as funcoes anteriores atualizam a variavel "d_StateMachineApp_cp->ConnectedElectricMeterInterface"
			if(d_StateMachineApp_cp->ConnectedElectricMeterInterface) //se conexao com hardware principal de medicao esta ok  (1= SUCESSO  0=FALHA).
			{
				#if APP_DEBUG_LOG_USING_USB_APP
					  printf("\n# Conseguiu reconectar !!! :)\n");
				#endif
				d_StateMachineApp_cp->AppStatusRegister2 = d_StateMachineApp_cp->AppStatusRegister2 | 1; //seta o bit 0 de indicacao da conexao fisica com o hardware de terceiros
			}
			else
			{
				d_StateMachineApp_cp->AppStatusRegister2 = d_StateMachineApp_cp->AppStatusRegister2 & 254; //nao foi conectado com sucesso
				#if APP_DEBUG_LOG_USING_USB_APP
					  printf("\n# NAO Conseguiu reconectar! :(\n");
				#endif
			}

			//PARTE 2: JA EXTRAPOLAMOS AS TENTATIVAS DE RECONEXAO?
			if(contTentativasConexaoHardTerceiros > QTD_TENTATIVAS_RECONEXAO_HARD_TERCEIROS) //Espera mais alguns ciclos antes de enviar alarme.
			{
				contTentativasConexaoHardTerceiros = 0;
				d_StateMachineApp_cp->State = STATE_APP_FALHA_CONEXAO_HARD_TERCEIROS;
				#if APP_DEBUG_LOG_USING_USB_APP
					  printf("\n# Tentamos reconectar X vezes. Mas nao deu. Agora vamos para o estado de falha de conexao com hard terceiros\n");
				#endif
			}

			return false; //deu errado a leitura
		}
		return false; //deu errado a leitura (se chegou aqui eh pq nada funcionou)
}

//==================================================================================================================================================
//
//		 										Função que pega as medidas elétricas chamando as funções do ADE
//
//==================================================================================================================================================

int AppMainBuild_AppEnernet_TakeElectricalMeasurement(StateMachineData_t *d_StateMachineApp_cp)
{
	if(d_StateMachineApp_cp->ConnectedElectricMeterInterface) //esta conectado?
	{
		//printf("valor lido do ADDR_RUN: %"PRIu16"\n\n\r",ADE9000SPI_Read16(ADDR_RUN));
		if(ADE9000SPI_Read16(ADDR_RUN))  //ADE apto a fazer medidas?
		{
			ADE9000_Measurements(&d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements); //pega medidas

			//vamos tratar algumas medidas:
			//Tratamento 1: trata o caso do angulo entre A-B e B-C que pode vir proximo de 240 ... ai tenho q fazer o complemento que gera os 120.
			if(d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VA_VB > 200)
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VA_VB = 360 - d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VA_VB;
			if(d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VB_VC > 200)
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VB_VC = 360 - d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VB_VC;
			if((d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VA_VC < 180) && (d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VA_VC > 20))
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VA_VC = 360 - d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VA_VC;

			//tratamento 2: se eu nao tiver quase nada de corrente em uma fase, o FP fica distorcido.
			if(d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActivePower.ActivePowerValue_A < 20) //se for potencia menor que 20w
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.PowerFactor.PowerFactorValue_A = 100;
			if(d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActivePower.ActivePowerValue_B < 20) //se for potencia menor que 20w
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.PowerFactor.PowerFactorValue_B = 100;
			if(d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActivePower.ActivePowerValue_C < 20) //se for potencia menor que 20w
				d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.PowerFactor.PowerFactorValue_C = 100;

			//AppMainBuild_Print(d_StateMachineApp_cp); //Chamando função para printar os valores no console!
			AppMainBuild_Print_Display(&d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements); //Chamando função para printar os valores no console!
		}
		return 1;
	}
	return 0; //se chegar aqui, eh pq nao leu entao retorna erro - zero
}

//==================================================================================================================================================
//
//		 										Função para calcular os valores estátisticos das medições elétricas
//
//==================================================================================================================================================

void AppMainBuild_AppEnernet_CalculateStatisticalValues(StateMachineData_t *d_StateMachineApp_cp, StatisticalValues_t *d_StatisticalValues_cp)
	{
		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		//PARTE 1:  pega os valores instantaneos e joga em variaveis temporarias. Uso para efeitos de padronizacao entre aplicacoes.
		uint16_t correnteInstantanea [NUM_FASES_SEP];   //quando [0][x] equivale a fonte e quando [1][x] equivale a carga
		uint16_t tensaoInstantanea   [NUM_FASES_SEP];   //quando [0][x] equivale a fonte e quando [1][x] equivale a carga
		uint16_t anguloTensaoEntreFasesInstantanea [NUM_FASES_SEP];
		float fatorPotenciaInstantaneo [NUM_FASES_SEP];
		float potenciaAtivaInstantaneo_kw [NUM_FASES_SEP];
		float potenciaReativaInstantaneo_kvar [NUM_FASES_SEP];
		float potenciaAparenteInstantaneo_kvar[NUM_FASES_SEP];
		float CurrentTHD[NUM_FASES_SEP];
		float VoltageTHD[NUM_FASES_SEP];
		float frequenciaRede[NUM_FASES_SEP];
		int8_t desequilibrioCorrenteInstantaneo = 0;
		int8_t desequilibrioTensaoInstantaneo = 0;
		float temporario; //usado para calculos auxiliares

		//PARTE 1.1: atribui variaveis locais
		tensaoInstantanea[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageRMS.VoltageRMSValue_A;
		tensaoInstantanea[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageRMS.VoltageRMSValue_B;
		tensaoInstantanea[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageRMS.VoltageRMSValue_C;
		correnteInstantanea[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentRMS.CurrentRMSValue_A;
		correnteInstantanea[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentRMS.CurrentRMSValue_B;
		correnteInstantanea[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentRMS.CurrentRMSValue_C;
		anguloTensaoEntreFasesInstantanea[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VA_VB;
		anguloTensaoEntreFasesInstantanea[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VB_VC;
		anguloTensaoEntreFasesInstantanea[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Angle.AngleValue_VA_VC;
		fatorPotenciaInstantaneo[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.PowerFactor.PowerFactorValue_A;
		fatorPotenciaInstantaneo[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.PowerFactor.PowerFactorValue_B;
		fatorPotenciaInstantaneo[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.PowerFactor.PowerFactorValue_C;
		potenciaAtivaInstantaneo_kw[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActivePower.ActivePowerValue_A;
		potenciaAtivaInstantaneo_kw[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActivePower.ActivePowerValue_B;
		potenciaAtivaInstantaneo_kw[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActivePower.ActivePowerValue_C;
		potenciaReativaInstantaneo_kvar[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ReactivePower.ReactivePowerValue_A;
		potenciaReativaInstantaneo_kvar[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ReactivePower.ReactivePowerValue_B;
		potenciaReativaInstantaneo_kvar[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ReactivePower.ReactivePowerValue_C;
		potenciaAparenteInstantaneo_kvar[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ApparentPower.ApparentPowerValue_A;
		potenciaAparenteInstantaneo_kvar[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ApparentPower.ApparentPowerValue_B;
		potenciaAparenteInstantaneo_kvar[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ApparentPower.ApparentPowerValue_C;
		CurrentTHD[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentTHD.CurrentTHDValue_A;
		CurrentTHD[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentTHD.CurrentTHDValue_B;
		CurrentTHD[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentTHD.CurrentTHDValue_C;
		VoltageTHD[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageTHD.VoltageTHDValue_A;
		VoltageTHD[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageTHD.VoltageTHDValue_B;
		VoltageTHD[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageTHD.VoltageTHDValue_C;
		frequenciaRede[0] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Period.FrequencyValue_A;
		frequenciaRede[1] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Period.FrequencyValue_B;
		frequenciaRede[2] = d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.Period.FrequencyValue_C;

		//PASSO 1.2: inicializa dados estatisticos do pacote de notificacao
		if(d_StateMachineApp_cp->habStaticCalcApp && (d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao == 0)) // Alguma funcao, ao enviar os dados, zera este contador. Isto significa que temos que zerar os dados estatiscos para o novo pacote que esta comecando agora com esta chamada
		{
			for(int i =0; i < NUM_FASES_SEP; i++) //calcula para as 3 fases!
			{
				d_StatisticalValues_cp->tensaoRMSmedia_pctNotificacao[i] = tensaoInstantanea[i];
				d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pctNotificacao[i] = anguloTensaoEntreFasesInstantanea[i];
				d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pctNotificacao[i] = 0.0;
				d_StatisticalValues_cp->correnteRMSmedia_pctNotificacao[i] = correnteInstantanea[i];
				d_StatisticalValues_cp->fatorPotenciaMedio_pctNotificacao[i] = fatorPotenciaInstantaneo[i];
				d_StatisticalValues_cp->potReativaMedia_pctNotificacao[i] = potenciaReativaInstantaneo_kvar[i];
				d_StatisticalValues_cp->potAtivaMedia_pctNotificacao[i] = potenciaAtivaInstantaneo_kw[i];
				d_StatisticalValues_cp->potAparenteMedia_pctNotificacao[i] = potenciaAparenteInstantaneo_kvar[i];
				d_StatisticalValues_cp->frequenciaRedeMedia_pctNotificacao[i] = frequenciaRede[i];
				d_StatisticalValues_cp->distorcaoHarmonicaCorrente_pctNotificacao[i] = CurrentTHD[i];
				d_StatisticalValues_cp->distorcaoHarmonicaTensao_pctNotificacao[i] = VoltageTHD[i];
				d_StatisticalValues_cp->desvioPadraoTensaoRMS_pctNotificacao[i] = 0.0;
				d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pctNotificacao[i] = 0.0;
				d_StatisticalValues_cp->desvioPadraoFatorPotencia_pctNotificacao[i] = 0.0;
				d_StatisticalValues_cp->desvioPadraoTensaoRMS_pctNotificacao[i] = 0.0;
				d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pctNotificacao[i] = 0.0;
				d_StatisticalValues_cp->desvioPadraoFatorPotencia_pctNotificacao[i] = 0.0;
			}
			d_StatisticalValues_cp->desequilibrioMedioTensaoEntreFases_pctNotificacao = 0.0;
			d_StatisticalValues_cp->desequilibrioMedioCorrenteEntreFases_pctNotificacao = 0.0;
			d_StatisticalValues_cp->desequilibrioCorrenteInstantaneo = 0;
			d_StatisticalValues_cp->desequilibrioTensaoInstantaneo = 0;
		}

		//PASSO 1.3: inicializa dados estatisticos do pacote de 24h  que sempre deve ser calculado (exceto qdo aplicacao nao esta habilitada)
		if(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h == 0)//alguma funcao, ao enviar os dados, zera este contador. Isto significa que temos que zerar os dados estatiscos para o novo pacote que esta comecando agora com esta chamada
		{
			for(int i =0; i < NUM_FASES_SEP; i++) //calcula para as 3 fases!
			{
				d_StatisticalValues_cp->tensaoRMSmedia_pct24h[i] = tensaoInstantanea[i];
				d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pct24h[i] = anguloTensaoEntreFasesInstantanea[i];
				d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pct24h[i] = 0.0;
				d_StatisticalValues_cp->correnteRMSmedia_pct24h[i] = correnteInstantanea[i];
				d_StatisticalValues_cp->fatorPotenciaMedio_pct24h[i] = 100.0;
				d_StatisticalValues_cp->desvioPadraoFatorPotencia_pct24h[i] = 0.0;
				d_StatisticalValues_cp->potReativaMedia_pct24h[i] = potenciaReativaInstantaneo_kvar[i];
				d_StatisticalValues_cp->desvioPadraoPotReativa_pct24h[i] = 0.0;
				d_StatisticalValues_cp->potAtivaMedia_pct24h[i] = potenciaAtivaInstantaneo_kw[i];
				d_StatisticalValues_cp->desvioPadraoPotAtiva_pct24h[i] = 0.0;
				d_StatisticalValues_cp->potAparenteMedia_pct24h[i] = potenciaAparenteInstantaneo_kvar[i];
				d_StatisticalValues_cp->frequenciaRedeMedia_pct24h[i] = frequenciaRede[i];
				d_StatisticalValues_cp->distorcaoHarmonicaCorrente_pct24h[i] = CurrentTHD[i];
				d_StatisticalValues_cp->distorcaoHarmonicaTensao_pct24h[i] = VoltageTHD[i];
				d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaCorrente_pct24h[i] = 0.0;
				d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaTensao_pct24h[i] = 0.0;
				d_StatisticalValues_cp->PicoTensaoNegativo_pct24h[i] = tensaoInstantanea[i]; //comecar com o valor atual
				d_StatisticalValues_cp->PicoTensaoPositivo_pct24h[i] = tensaoInstantanea[i]; //comecar com o valor atual
				d_StatisticalValues_cp->MenorFatorPotencia_pct24h[i] = 100; //comecar com o valor atual
				d_StatisticalValues_cp->desvioPadraoTensaoRMS_pct24h[i] = 0.0;
				d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pct24h[i] = 0.0;
				d_StatisticalValues_cp->desvioPadraoFatorPotencia_pct24h[i] = 0.0;
				d_StatisticalValues_cp->PicoCorrente_pct24h[i] = 0;
				d_StatisticalValues_cp->MenorFatorPotencia_pct24h[i] = 100.0;
				d_StatisticalValues_cp->desvioPadraoPotAparente_pct24h[i] = 0.0f;
			}
			d_StatisticalValues_cp->PicoPotenciaAparenteTotal_pct24h = 0;
			d_StatisticalValues_cp->desequilibrioMedioTensaoEntreFases_pct24h = 0.0;
			d_StatisticalValues_cp->desequilibrioMedioCorrenteEntreFases_pct24h = 0.0;
			d_StatisticalValues_cp->MaiorDesequilibrioTensao_pct24h = 0;
			d_StatisticalValues_cp->MaiorDesequilibrioCorrente_pct24h = 0;
			d_StateMachineApp_cp->EnernetInstantaneousMeasurements.AcumuladorEnergiaAtiva24h = 0.0f;  //zera o acumulador de energia deste pacote
		}


		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		//PARTE 2: calcula desequilibrio de tensao e corrente INSTANTENEOS
		if(NUM_FASES_SEP > 1) //se for trifasico
		{
				//2.1 => media do desequilibrio medio de v entre fases
				float beta, Xab, Xbc, Xca;
				Xab = (tensaoInstantanea[0]/10)*(tensaoInstantanea[0]/10); //elevado a 2. Dividi por 100 para nao estourar o valor
				Xbc = (tensaoInstantanea[1]/10)*(tensaoInstantanea[1]/10); //elevado a 2. Dividi por 100 para nao estourar o valor
				Xca = (tensaoInstantanea[2]/10)*(tensaoInstantanea[2]/10); //elevado a 2. Dividi por 100 para nao estourar o valor
				if((Xab==0) && (Xbc==0) && (Xca==0)) //condicao especial (caiu energia).
					desequilibrioTensaoInstantaneo = 0;
				else
				{
					beta = (Xab*Xab + Xbc*Xbc + Xca*Xca)/((Xab + Xbc + Xca)*(Xab + Xbc + Xca));
					desequilibrioTensaoInstantaneo = (uint8_t) round(100*sqrt(   (1-sqrt(3-6*beta)) / (1+sqrt(3-6*beta))   ));   //vai ficar entre 0 e 100 e por isto a medida eh percentual;
				}
				//2.2 => media do desequilibrio medio de I entre fases. Este calculo eh feito usando a formulacao matematica vista no "PRODIST, modulo 8 da ANEEL", mas especificamente na secao "5 DESEQUILÍBRIO DE TENSÃO"
				Xab = (correnteInstantanea[0]/2)*(correnteInstantanea[0]/2);
				Xbc = (correnteInstantanea[1]/2)*(correnteInstantanea[1]/2);
				Xca = (correnteInstantanea[2]/2)*(correnteInstantanea[2]/2);
				if((Xab==0) && (Xbc==0) && (Xca==0)) //condicao especial (caiu energia).
					desequilibrioCorrenteInstantaneo = 0;
				else
				{
					beta = (Xab*Xab + Xbc*Xbc + Xca*Xca)/((Xab + Xbc + Xca)*(Xab + Xbc + Xca));
					desequilibrioCorrenteInstantaneo = (uint8_t) round(100*sqrt(   (1-sqrt(3-6*beta)) / (1+sqrt(3-6*beta))   ));   //vai ficar entre 0 e 100 e por isto a medida eh percentual;
				}
		}
		d_StatisticalValues_cp->desequilibrioCorrenteInstantaneo = desequilibrioCorrenteInstantaneo;
		d_StatisticalValues_cp->desequilibrioTensaoInstantaneo = desequilibrioTensaoInstantaneo;


		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		//PARTE 3: processa os dados estatisticos do pacote de notificacao e de 24h. O algoritmo usado para calcular media e desvio padrao de forma computacionalmente eficiente eh o "Welford's online algorithm". Referencia: https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
		d_StatisticalValues_cp->contadorMedidasFeitas_pct24h++; //incrimenta numero de pacotes recebidos
		if(d_StateMachineApp_cp->habStaticCalcApp)
			d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao++; //incrimenta numero de pacotes recebidos. Notificacao eh optativo. 24h nao!

		//PARTE 3.1: calculos dos pacotes de notificacao
		if((d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao > 1) && d_StateMachineApp_cp->habStaticCalcApp)
		{
			for(int i = 0; i < NUM_FASES_SEP; i++) //calcula para as 3 fases!
			{
				temporario = (tensaoInstantanea[i] - d_StatisticalValues_cp->tensaoRMSmedia_pctNotificacao[i]); //tive que colocar este antes, porque se olharmos a formula de Welford, para calcaular a variancia/desvioPadrao, eu preciso da media anterior. Entao antes de calcular a nova, eu uso a antigo para este calculo temporario usado na estimacao da varianca
				d_StatisticalValues_cp->tensaoRMSmedia_pctNotificacao[i] = d_StatisticalValues_cp->tensaoRMSmedia_pctNotificacao[i]   +   ((tensaoInstantanea[i] - d_StatisticalValues_cp->tensaoRMSmedia_pctNotificacao[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
				d_StatisticalValues_cp->desvioPadraoTensaoRMS_pctNotificacao[i] = d_StatisticalValues_cp->desvioPadraoTensaoRMS_pctNotificacao[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao) -  (d_StatisticalValues_cp->desvioPadraoTensaoRMS_pctNotificacao[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao - 1));
				d_StatisticalValues_cp->desvioPadraoTensaoRMS_pctNotificacao[i] = sqrt(d_StatisticalValues_cp->desvioPadraoTensaoRMS_pctNotificacao[i] ); //tinhamos calculado a variancia. Agora calcula variencia passando para desvio padrao

				temporario = (correnteInstantanea[i] - d_StatisticalValues_cp->correnteRMSmedia_pctNotificacao[i]); //tive que colocar este antes, porque se olharmos a formula de Welford, para calcaular a variancia/desvioPadrao, eu preciso da media anterior. Entao antes de calcular a nova, eu uso a antigo para este calculo temporario usado na estimacao da varianca
				d_StatisticalValues_cp->correnteRMSmedia_pctNotificacao[i] = d_StatisticalValues_cp->correnteRMSmedia_pctNotificacao[i]   +   ((correnteInstantanea[i] - d_StatisticalValues_cp->correnteRMSmedia_pctNotificacao[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
				d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pctNotificacao[i] = d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pctNotificacao[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao) -  (d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pctNotificacao[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao - 1));
				d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pctNotificacao[i] = sqrt(d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pctNotificacao[i] ); //tinhamos calculado a variancia. Agora calcula variencia passando para desvio padrao

				temporario = (fatorPotenciaInstantaneo[i] - d_StatisticalValues_cp->fatorPotenciaMedio_pctNotificacao[i]); //tive que colocar este antes, porque se olharmos a formula de Welford, para calcaular a variancia/desvioPadrao, eu preciso da media anterior. Entao antes de calcular a nova, eu uso a antigo para este calculo temporario usado na estimacao da varianca
				d_StatisticalValues_cp->fatorPotenciaMedio_pctNotificacao[i] = d_StatisticalValues_cp->fatorPotenciaMedio_pctNotificacao[i]   +   ((fatorPotenciaInstantaneo[i] - d_StatisticalValues_cp->fatorPotenciaMedio_pctNotificacao[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
				d_StatisticalValues_cp->desvioPadraoFatorPotencia_pctNotificacao[i] = d_StatisticalValues_cp->desvioPadraoFatorPotencia_pctNotificacao[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao) -  (d_StatisticalValues_cp->desvioPadraoFatorPotencia_pctNotificacao[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao - 1));
				d_StatisticalValues_cp->desvioPadraoFatorPotencia_pctNotificacao[i] = sqrt(d_StatisticalValues_cp->desvioPadraoFatorPotencia_pctNotificacao[i] ); //tinhamos calculado a variancia. Agora calcula variencia passando para desvio padrao

				temporario = (anguloTensaoEntreFasesInstantanea[i] - d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pctNotificacao[i]);
				d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pctNotificacao[i] = d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pctNotificacao[i]   +   ((anguloTensaoEntreFasesInstantanea[i] - d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pctNotificacao[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
				d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pctNotificacao[i] = d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pctNotificacao[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao) -  (d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pctNotificacao[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao - 1));
				d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pctNotificacao[i] = sqrt(d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pctNotificacao[i] ); //tinhamos calculado a variancia. Agora calcula variencia passando para desvio padrao

				d_StatisticalValues_cp->potAtivaMedia_pctNotificacao[i]    = d_StatisticalValues_cp->potAtivaMedia_pctNotificacao[i]    +   ((potenciaAtivaInstantaneo_kw[i]      - d_StatisticalValues_cp->potAtivaMedia_pctNotificacao[i])   / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
				d_StatisticalValues_cp->potReativaMedia_pctNotificacao[i]  = d_StatisticalValues_cp->potReativaMedia_pctNotificacao[i]  +   ((potenciaReativaInstantaneo_kvar[i]  - d_StatisticalValues_cp->potReativaMedia_pctNotificacao[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
				d_StatisticalValues_cp->potAparenteMedia_pctNotificacao[i] = d_StatisticalValues_cp->potAparenteMedia_pctNotificacao[i] +   ((potenciaAparenteInstantaneo_kvar[i] - d_StatisticalValues_cp->potAparenteMedia_pctNotificacao[i])/ d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);


				d_StatisticalValues_cp->distorcaoHarmonicaTensao_pctNotificacao[i]   = d_StatisticalValues_cp->distorcaoHarmonicaTensao_pctNotificacao[i]    +   ((VoltageTHD[i] - d_StatisticalValues_cp->distorcaoHarmonicaTensao_pctNotificacao[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
				d_StatisticalValues_cp->distorcaoHarmonicaCorrente_pctNotificacao[i] = d_StatisticalValues_cp->distorcaoHarmonicaCorrente_pctNotificacao[i]  +   ((CurrentTHD[i] - d_StatisticalValues_cp->distorcaoHarmonicaCorrente_pctNotificacao[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);

				d_StatisticalValues_cp->frequenciaRedeMedia_pctNotificacao[i] = d_StatisticalValues_cp->frequenciaRedeMedia_pctNotificacao[i] +  ((frequenciaRede[i] - d_StatisticalValues_cp->frequenciaRedeMedia_pctNotificacao[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
			}
			d_StatisticalValues_cp->desequilibrioMedioTensaoEntreFases_pctNotificacao   = d_StatisticalValues_cp->desequilibrioMedioTensaoEntreFases_pctNotificacao    +   ((desequilibrioTensaoInstantaneo   - d_StatisticalValues_cp->desequilibrioMedioTensaoEntreFases_pctNotificacao) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
			d_StatisticalValues_cp->desequilibrioMedioCorrenteEntreFases_pctNotificacao = d_StatisticalValues_cp->desequilibrioMedioCorrenteEntreFases_pctNotificacao  +   ((desequilibrioCorrenteInstantaneo - d_StatisticalValues_cp->desequilibrioMedioCorrenteEntreFases_pctNotificacao) / d_StatisticalValues_cp->contadorMedidasFeitas_pctNotificacao);
			//ate aqui, calculamos a variancia. Agora temos que calcular o desvio padrao aplicando a raiz quadrada na variancia


		}

		//PARTE 3.2: calculos dos pacotes de 24h. O algoritmo usado para  calcular media e desvio padrao de forma computacionalmente eficiente eh o "Welford's online algorithm". Referencia: https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
		if(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h > 1)
		{
			for(int i =0; i < NUM_FASES_SEP; i++) //calcula para as 3 fases!
			{
				temporario = (tensaoInstantanea[i] - d_StatisticalValues_cp->tensaoRMSmedia_pct24h[i]); //tive que colocar este antes, porque se olharmos a formula de Welford, para calcaular a variancia/desvioPadrao, eu preciso da media anterior. Entao antes de calcular a nova, eu uso a antigo para este calculo temporario usado na estimacao da varianca
				d_StatisticalValues_cp->tensaoRMSmedia_pct24h[i] = d_StatisticalValues_cp->tensaoRMSmedia_pct24h[i]   +   ((tensaoInstantanea[i] - d_StatisticalValues_cp->tensaoRMSmedia_pct24h[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
				d_StatisticalValues_cp->desvioPadraoTensaoRMS_pct24h[i] = d_StatisticalValues_cp->desvioPadraoTensaoRMS_pct24h[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pct24h) -  (d_StatisticalValues_cp->desvioPadraoTensaoRMS_pct24h[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h - 1));
				d_StatisticalValues_cp->desvioPadraoTensaoRMS_pct24h[i] = sqrt(d_StatisticalValues_cp->desvioPadraoTensaoRMS_pct24h[i] ); //tinhamos calculado a variancia. Agora calcula variencia passando para desvio padrao

				temporario = (correnteInstantanea[i] - d_StatisticalValues_cp->correnteRMSmedia_pct24h[i]); //tive que colocar este antes, porque se olharmos a formula de Welford, para calcaular a variancia/desvioPadrao, eu preciso da media anterior. Entao antes de calcular a nova, eu uso a antigo para este calculo temporario usado na estimacao da varianca
				d_StatisticalValues_cp->correnteRMSmedia_pct24h[i] = d_StatisticalValues_cp->correnteRMSmedia_pct24h[i]   +   ((correnteInstantanea[i] - d_StatisticalValues_cp->correnteRMSmedia_pct24h[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
				d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pct24h[i] = d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pct24h[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pct24h) -  (d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pct24h[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h - 1));
				d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pct24h[i] = sqrt(d_StatisticalValues_cp->desvioPadraoCorrenteRMS_pct24h[i] );

				temporario = (fatorPotenciaInstantaneo[i] - d_StatisticalValues_cp->fatorPotenciaMedio_pct24h[i]); //tive que colocar este antes, porque se olharmos a formula de Welford, para calcaular a variancia/desvioPadrao, eu preciso da media anterior. Entao antes de calcular a nova, eu uso a antigo para este calculo temporario usado na estimacao da varianca
				d_StatisticalValues_cp->fatorPotenciaMedio_pct24h[i] = d_StatisticalValues_cp->fatorPotenciaMedio_pct24h[i]   +   ((fatorPotenciaInstantaneo[i] - d_StatisticalValues_cp->fatorPotenciaMedio_pct24h[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
				d_StatisticalValues_cp->desvioPadraoFatorPotencia_pct24h[i] = d_StatisticalValues_cp->desvioPadraoFatorPotencia_pct24h[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pct24h) -  (d_StatisticalValues_cp->desvioPadraoFatorPotencia_pct24h[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h - 1));
				d_StatisticalValues_cp->desvioPadraoFatorPotencia_pct24h[i] = sqrt(d_StatisticalValues_cp->desvioPadraoFatorPotencia_pct24h[i] );

				temporario = (potenciaAtivaInstantaneo_kw[i] - d_StatisticalValues_cp->potAtivaMedia_pct24h[i]); //tive que colocar este antes, porque se olharmos a formula de Welford, para calcaular a variancia/desvioPadrao, eu preciso da media anterior. Entao antes de calcular a nova, eu uso a antigo para este calculo temporario usado na estimacao da varianca
				d_StatisticalValues_cp->potAtivaMedia_pct24h[i]    = d_StatisticalValues_cp->potAtivaMedia_pct24h[i]    +   ((potenciaAtivaInstantaneo_kw[i]      - d_StatisticalValues_cp->potAtivaMedia_pct24h[i])   / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
				d_StatisticalValues_cp->desvioPadraoPotAtiva_pct24h[i] = d_StatisticalValues_cp->desvioPadraoPotAtiva_pct24h[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pct24h) -  (d_StatisticalValues_cp->desvioPadraoPotAtiva_pct24h[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h - 1));
				d_StatisticalValues_cp->desvioPadraoPotAtiva_pct24h[i] = sqrt(d_StatisticalValues_cp->desvioPadraoPotAtiva_pct24h[i] );

				temporario = (potenciaReativaInstantaneo_kvar[i] - d_StatisticalValues_cp->potReativaMedia_pct24h[i]); //tive que colocar este antes, porque se olharmos a formula de Welford, para calcaular a variancia/desvioPadrao, eu preciso da media anterior. Entao antes de calcular a nova, eu uso a antigo para este calculo temporario usado na estimacao da varianca
				d_StatisticalValues_cp->potReativaMedia_pct24h[i]    = d_StatisticalValues_cp->potReativaMedia_pct24h[i]    +   ((potenciaReativaInstantaneo_kvar[i]      - d_StatisticalValues_cp->potReativaMedia_pct24h[i])   / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
				d_StatisticalValues_cp->desvioPadraoPotReativa_pct24h[i] = d_StatisticalValues_cp->desvioPadraoPotReativa_pct24h[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pct24h) -  (d_StatisticalValues_cp->desvioPadraoPotReativa_pct24h[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h - 1));
				d_StatisticalValues_cp->desvioPadraoPotReativa_pct24h[i] = sqrt(d_StatisticalValues_cp->desvioPadraoPotReativa_pct24h[i] );

				temporario = (potenciaAparenteInstantaneo_kvar[i] - d_StatisticalValues_cp->potAparenteMedia_pct24h[i]); //tive que colocar este antes, porque se olharmos a formula de Welford, para calcaular a variancia/desvioPadrao, eu preciso da media anterior. Entao antes de calcular a nova, eu uso a antigo para este calculo temporario usado na estimacao da varianca
				d_StatisticalValues_cp->potAparenteMedia_pct24h[i]    = d_StatisticalValues_cp->potAparenteMedia_pct24h[i]    +   ((potenciaAparenteInstantaneo_kvar[i]      - d_StatisticalValues_cp->potAparenteMedia_pct24h[i])   / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
				d_StatisticalValues_cp->desvioPadraoPotAparente_pct24h[i] = d_StatisticalValues_cp->desvioPadraoPotAparente_pct24h[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pct24h) -  (d_StatisticalValues_cp->desvioPadraoPotAparente_pct24h[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h - 1));
				d_StatisticalValues_cp->desvioPadraoPotAparente_pct24h[i] = sqrt(d_StatisticalValues_cp->desvioPadraoPotAparente_pct24h[i] );

				temporario = (anguloTensaoEntreFasesInstantanea[i] - d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pct24h[i]);
				d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pct24h[i] = d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pct24h[i]   +   ((anguloTensaoEntreFasesInstantanea[i] - d_StatisticalValues_cp->anguloMedioTensaoEntreFases_pct24h[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
				d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pct24h[i] = d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pct24h[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pct24h) -  (d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pct24h[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h - 1));
				d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pct24h[i] = sqrt(d_StatisticalValues_cp->desvioPadraoAnguloMedioTensaoEntreFases_pct24h[i] );

				temporario = (VoltageTHD[i] - d_StatisticalValues_cp->distorcaoHarmonicaTensao_pct24h[i]);
				d_StatisticalValues_cp->distorcaoHarmonicaTensao_pct24h[i]   = d_StatisticalValues_cp->distorcaoHarmonicaTensao_pct24h[i]    +   ((VoltageTHD[i] - d_StatisticalValues_cp->distorcaoHarmonicaTensao_pct24h[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
				d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaTensao_pct24h[i] = d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaTensao_pct24h[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pct24h) -  (d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaTensao_pct24h[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h - 1));
				d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaTensao_pct24h[i] = sqrt(d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaTensao_pct24h[i] );

				temporario = (CurrentTHD[i] - d_StatisticalValues_cp->distorcaoHarmonicaCorrente_pct24h[i]);
				d_StatisticalValues_cp->distorcaoHarmonicaCorrente_pct24h[i] = d_StatisticalValues_cp->distorcaoHarmonicaCorrente_pct24h[i]  +   ((CurrentTHD[i] - d_StatisticalValues_cp->distorcaoHarmonicaCorrente_pct24h[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
				d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaCorrente_pct24h[i] = d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaCorrente_pct24h[i]   +  ((temporario*temporario)/d_StatisticalValues_cp->contadorMedidasFeitas_pct24h) -  (d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaCorrente_pct24h[i]/(d_StatisticalValues_cp->contadorMedidasFeitas_pct24h - 1));
				d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaCorrente_pct24h[i] = sqrt(d_StatisticalValues_cp->desvioPadraoDistorcaoHarmonicaCorrente_pct24h[i] );

				d_StatisticalValues_cp->frequenciaRedeMedia_pct24h[i] = d_StatisticalValues_cp->frequenciaRedeMedia_pct24h[i] +  ((frequenciaRede[i] - d_StatisticalValues_cp->frequenciaRedeMedia_pct24h[i]) / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
			}
			d_StatisticalValues_cp->desequilibrioMedioTensaoEntreFases_pct24h   = d_StatisticalValues_cp->desequilibrioMedioTensaoEntreFases_pct24h    +   ((desequilibrioTensaoInstantaneo   - d_StatisticalValues_cp->desequilibrioMedioTensaoEntreFases_pct24h) / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
			d_StatisticalValues_cp->desequilibrioMedioCorrenteEntreFases_pct24h = d_StatisticalValues_cp->desequilibrioMedioCorrenteEntreFases_pct24h  +   ((desequilibrioCorrenteInstantaneo - d_StatisticalValues_cp->desequilibrioMedioCorrenteEntreFases_pct24h) / d_StatisticalValues_cp->contadorMedidasFeitas_pct24h);
		}

		//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		//PARTE 4: Estima picos
		for(int i =0; i < NUM_FASES_SEP; i++)
		{
				if(tensaoInstantanea[i] > d_StatisticalValues_cp->PicoTensaoPositivo_pct24h[i]) //verifica se eh maior pico de tensao (positiva)
					d_StatisticalValues_cp->PicoTensaoPositivo_pct24h[i] = tensaoInstantanea[i];
				if(tensaoInstantanea[i] < d_StatisticalValues_cp->PicoTensaoNegativo_pct24h[i]) //verifica se eh menor pico de tensao (negativo)
					d_StatisticalValues_cp->PicoTensaoNegativo_pct24h[i] = tensaoInstantanea[i];
				if(correnteInstantanea[i] > d_StatisticalValues_cp->PicoCorrente_pct24h[i]) //verifica se eh menor pico de tensao (negativo)
					d_StatisticalValues_cp->PicoCorrente_pct24h[i] = correnteInstantanea[i];
				if(fatorPotenciaInstantaneo[i] < d_StatisticalValues_cp->MenorFatorPotencia_pct24h[i]) //verifica se eh menor fator de potencia
					d_StatisticalValues_cp->MenorFatorPotencia_pct24h[i] = fatorPotenciaInstantaneo[i];
		}
		if(desequilibrioTensaoInstantaneo  > d_StatisticalValues_cp->MaiorDesequilibrioTensao_pct24h) //verifica se eh maior desequilibrio de tensao
			d_StatisticalValues_cp->MaiorDesequilibrioTensao_pct24h = desequilibrioTensaoInstantaneo;
		if(desequilibrioCorrenteInstantaneo > d_StatisticalValues_cp->MaiorDesequilibrioCorrente_pct24h) //verifica se eh maior desequilibrio de corrente
			d_StatisticalValues_cp->MaiorDesequilibrioCorrente_pct24h = desequilibrioCorrenteInstantaneo;

		int potencialAparenteTotal = potenciaAparenteInstantaneo_kvar[0] + potenciaAparenteInstantaneo_kvar[1] + potenciaAparenteInstantaneo_kvar[2];
		if(potencialAparenteTotal > d_StatisticalValues_cp->PicoPotenciaAparenteTotal_pct24h) //verifica se eh maior desequilibrio de corrente
			d_StatisticalValues_cp->PicoPotenciaAparenteTotal_pct24h = potencialAparenteTotal;

		return;
}

//==================================================================================================================================================
//
//		 										Principal funcao onde se tudo estiver funcionando bem
//												a maquina de estados fica so aqui fazendo suas tarefas
//
//==================================================================================================================================================

void AppMainBuild_General_AppRunning(StateMachineData_t *d_StateMachineApp_cp)
{
	if(d_StateMachineApp_cp->habExecucaoApp)//se as funcoes do aplicativo estao habilitadas
	{
		//PASSO 1: faz medicoes
		if(d_StateMachineApp_cp->contTempoEntreLeiturasSeg >= d_StateMachineApp_cp->freqCalculoSeg) //verifica se o tempo decorrido ja estourou para fazer medidas
		{
			d_StateMachineApp_cp->contTempoEntreLeiturasSeg = 0; //reinicia contagem de tempo entre uma leitura e outra
			//PASSO 3.1: faz medicao no hardware de terceiros
			d_StateMachineApp_cp->novaLeituraMedidas = AppMainBuild_General_TakeMeasurement(d_StateMachineApp_cp);
			//PASSO 3.2: atualiza estatisticas
			if(d_StateMachineApp_cp->novaLeituraMedidas) //se existe nova medida
			{
				//AppMainBuild_AppEnernet_CalculateStatisticalValues(d_StateMachineApp_cp, &d_StatisticalValues);  //pega os dados atuais recem-medidos e atualiza as estatiscas contidas em "medidasEstatisticas" para que estas estatisticas possam ser enviadas nos pacotes 50 e 51. Tambem alimenta com dados a estrutura de dados "d_AppSG_AlertasEAlarmes" que tem as medidas paras as funcoes de alerta e alarme
			}
		}
	}

	osDelay(1000);
	d_StateMachineApp_cp->contTempoEntreLeiturasSeg++;
}


//==================================================================================================================================================
//
//		 										Função para printar todos os valores que estão sendo medidos
//																para verificação dos dados
//
//==================================================================================================================================================
void AppMainBuild_Print(StateMachineData_t *d_StateMachineApp_cp)
{
	printf("######################################################\r\n\n");

	printf("Tensao RMS A: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageRMS.VoltageRMSValue_A);
	printf("Tensao RMS B: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageRMS.VoltageRMSValue_B);
	printf("Tensao RMS C: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageRMS.VoltageRMSValue_C);
	printf("------------------------------------\r\n");

	printf("Corrente RMS A: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentRMS.CurrentRMSValue_A);
	printf("Corrente RMS B: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentRMS.CurrentRMSValue_B);
	printf("Corrente RMS C: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentRMS.CurrentRMSValue_C);
	printf("------------------------------------\r\n");

	printf("Potencia ativa A: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActivePower.ActivePowerValue_A);
	printf("Potencia ativa B: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActivePower.ActivePowerValue_B);
	printf("Potencia ativa C: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ActivePower.ActivePowerValue_C);
	printf("------------------------------------\r\n");

	printf("Potencia aparente A: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ApparentPower.ApparentPowerValue_A);
	printf("Potencia aparente B: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ApparentPower.ApparentPowerValue_B);
	printf("Potencia aparente C: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.ApparentPower.ApparentPowerValue_C);
	printf("------------------------------------\r\n");

	printf("Tensao THD A: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageTHD.VoltageTHDValue_A);
	printf("Tensao THD B: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageTHD.VoltageTHDValue_B);
	printf("Tensao THD C: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.VoltageTHD.VoltageTHDValue_C);
	printf("------------------------------------\r\n");

	printf("Corrente THD A: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentTHD.CurrentTHDValue_A);
	printf("Corrente THD B: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentTHD.CurrentTHDValue_B);
	printf("Corrente THD C: %.3f\r\n",d_StateMachineApp_cp->EnernetInstantaneousMeasurements.EnergyMeasurements.CurrentTHD.CurrentTHDValue_C);

	printf("\n######################################################\r\n\n\n\n\n\n\n\n");
}

void AppMainBuild_Print_Display(ADE9000_EnergyMeasurements *data)
{
	lumen_packet_t correnteRMSA = {address_correnteARMS, kDouble};
	lumen_packet_t correnteRMSB = {address_correnteBRMS, kDouble};
	lumen_packet_t correnteRMSC = {address_correnteCRMS, kDouble};
	lumen_packet_t correnteTHDA = {address_correnteATHD, kDouble};
	lumen_packet_t correnteTHDB = {address_correnteBTHD, kDouble};
	lumen_packet_t correnteTHDC = {address_correnteCTHD, kDouble};
	lumen_packet_t tensaoRMSA = {address_tensaoARMS, kDouble};
	lumen_packet_t tensaoRMSB = {address_tensaoBRMS, kDouble};
	lumen_packet_t tensaoRMSC = {address_tensaoCRMS, kDouble};
	lumen_packet_t tensaoTHDA = {address_tensaoATHD, kDouble};
	lumen_packet_t tensaoTHDB = {address_tensaoBTHD, kDouble};
	lumen_packet_t tensaoTHDC = {address_tensaoCTHD, kDouble};

	correnteRMSA.data._double = (double) data->CurrentRMS.CurrentRMSValue_A;
	correnteRMSB.data._double = (double) data->CurrentRMS.CurrentRMSValue_B;
	correnteRMSC.data._double = (double) data->CurrentRMS.CurrentRMSValue_C;
	correnteTHDA.data._double = (double) data->CurrentTHD.CurrentTHDValue_A;
	correnteTHDB.data._double = (double) data->CurrentTHD.CurrentTHDValue_B;
	correnteTHDC.data._double = (double) data->CurrentTHD.CurrentTHDValue_C;
	tensaoRMSA.data._double = (double) data->VoltageRMS.VoltageRMSValue_A;
	tensaoRMSB.data._double = (double) data->VoltageRMS.VoltageRMSValue_B;
	tensaoRMSC.data._double = (double) data->VoltageRMS.VoltageRMSValue_C;
	tensaoTHDA.data._double = (double) data->VoltageTHD.VoltageTHDValue_A;
	tensaoTHDB.data._double = (double) data->VoltageTHD.VoltageTHDValue_B;
	tensaoTHDC.data._double = (double) data->VoltageTHD.VoltageTHDValue_C;

	lumen_write_packet(&correnteRMSA);
	lumen_write_packet(&correnteRMSB);
	lumen_write_packet(&correnteRMSC);
	lumen_write_packet(&correnteTHDA);
	lumen_write_packet(&correnteTHDB);
	lumen_write_packet(&correnteTHDC);
	lumen_write_packet(&tensaoRMSA);
	lumen_write_packet(&tensaoRMSB);
	lumen_write_packet(&tensaoRMSC);
	lumen_write_packet(&tensaoTHDA);
	lumen_write_packet(&tensaoTHDB);
	lumen_write_packet(&tensaoTHDC);
}

