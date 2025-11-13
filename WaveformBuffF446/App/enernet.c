/* ----------------------------------- Includes ------------------------------------ */
#include "enernet.h"
#include "Support/supportFunctions.h"
#include <stdbool.h>

/* ---------------------------------- Declaração ----------------------------------- */
StateMachineData_t  d_StateMachineApp;   		//contem todas as informações básicas sobre a aplicação
StatisticalValues_t d_StatisticalValues;		//guarda os valores estátisticos das medições feitas
int 				firstExecution = 1;			//variável para computar quando o programa faz a sua primeira execução

/* ------------------------------------ Código ------------------------------------- */
void App_loop(void)
{
	MainBuild_InitiateApp();
}

//==================================================================================================================================================
//
//                                      	 Função de iniciação das primeiras variáveis do programa
//
//==================================================================================================================================================

void MainBuild_InitiateApp(void)
{
	//PARTE 1: Configurações iniciais da aplicação
	if (firstExecution)
	{
		firstExecution = 0;
		d_StateMachineApp.State = STATE_APP_INICIA_VARIAVEIS_E_CARREGAMENTOS;
	}

	//Chamada da máquina de estados

	stateMachine();

	//Registradores de estado da aplicacao:
	//d_StateMachineApp.AppStatusRegister0 = 0;
	//d_StateMachineApp.AppStatusRegister1 = (d_StateMachineApp.AppStatusRegister1 | 16); //SETA BIT STATE_APP_RUNNING_NORMAL
	//d_StateMachineApp.AppStatusRegister1 = (d_StateMachineApp.AppStatusRegister1 & 31); //DESLIGA OUTROS BITS DE INDICACAO DE CONEXAO
	//d_StateMachineApp.AppStatusRegister2 = 0;
	return;
}

//==================================================================================================================================================
//
//                                      			Máquina de estados do programa
//                                    			atualmente sem tratação de erros implementada
//
//==================================================================================================================================================

bool stateMachine(void)
{
	switch(d_StateMachineApp.State)
	{
//-----------------------------------------------------------------------------------------------------------------------------------------------
		case STATE_APP_INICIA_VARIAVEIS_E_CARREGAMENTOS:

			//d_StateMachineApp.AppStatusRegister0 = 0; //zera outros estados
			//d_StateMachineApp.AppStatusRegister1 = (d_StateMachineApp.AppStatusRegister1 & 254);//limpa estado bit 0
			//d_StateMachineApp.AppStatusRegister1 = (d_StateMachineApp.AppStatusRegister1 | 2);//indica o atual estado

			//INICIALIZACOES DAs VARIAVEIS DA APLICACAO EM GERAL:
			AppMainBuild_General_InitiateVarStateMachine(&d_StateMachineApp);

			//INICIALIZACAO DAS ROTINAS MATEMATICAS DA APLICACAO USADAS PARA PROCESSAR MEDIDAS:
			d_StatisticalValues.contadorMedidasFeitas_pctNotificacao = 0;
			d_StatisticalValues.contadorMedidasFeitas_pct24h = 0;

			// =========================================================================================================================
			//
			// 	Aqui pode ser implementado o carregamento de dados da memória para a aplicação caso seja necessário
			//
			// =========================================================================================================================

			d_StateMachineApp.State = STATE_APP_INICIA_CONEXOES; //vai alterar o estado incial da máquina para o próximo estado
			d_StateMachineApp.contTempoEntreLeiturasSeg = d_StateMachineApp.freqCalculoSeg; //igualando o tempo para leitura dos valores do ADE9000
		break;
//-----------------------------------------------------------------------------------------------------------------------------------------------
		case STATE_APP_INICIA_CONEXOES:
			//d_StateMachineApp.AppStatusRegister0 = 1; //indica o atual estado
			//d_StateMachineApp.AppStatusRegister1 = (d_StateMachineApp.AppStatusRegister1 & 252);//indica o atual estado

			//INICIAR CONEXAO COM HARDWARE DE TERCEIROS: futuramente incluir aqui uma funcao de identificacao de qual hardware esta conectado a ele
			AppMainBuild_AppEnernet_HardwareInitiate(&d_StateMachineApp); //inicialização do hardware ADE

			//if(d_StateMachineApp.ConnectedElectricMeterInterface) //se conexao com hardware principal de medicao esta ok  (1= SUCESSO  0=FALHA).
				//d_StateMachineApp.AppStatusRegister2 = d_StateMachineApp.AppStatusRegister2 | 1; //seta o bit 0 de indicacao da conexao fisica com o hardware de terceiros
			//else
				//d_StateMachineApp.AppStatusRegister2 = d_StateMachineApp.AppStatusRegister2 & 254; //nao foi conectado com sucesso

			d_StateMachineApp.State = STATE_APP_EM_EXECUCAO; //vai para execução, esperando que tenha dado certo a conexão com o hardware de terceiros

		break;
//-----------------------------------------------------------------------------------------------------------------------------------------------
		case STATE_APP_EM_EXECUCAO:
			//d_StateMachineApp.AppStatusRegister0 = 8; //indica o atual estado
			//d_StateMachineApp.AppStatusRegister1 = (d_StateMachineApp.AppStatusRegister1 & 252);//zera outros estados (dois primeiros bits)

			AppMainBuild_General_AppRunning(&d_StateMachineApp);  //uma das principais funcoes da aplicacao esta aqui .... 99% do tempo ela fica rodando

			#if APP_DEBUG_LOG_USING_USB_APP
				printf("\n(%0*d:%0*d:%0*d [%dd])>>", 2 , d_StateMachineApp.tempoAppEmExecucaoHoras, 2, d_StateMachineApp.tempoAppEmExecucaoMin, 2, d_StateMachineApp.tempoAppEmExecucaoSeg, d_StateMachineApp.tempoAppEmExecucaoDias);
			#endif
		break;
//-----------------------------------------------------------------------------------------------------------------------------------------------
		case STATE_APP_FALHA_REGISTRO_APP:
			d_StateMachineApp.State = STATE_APP_INICIA_CONEXOES;
		break;
//------------------------------------------------------------------------------------------------------------------------------------------------
		case STATE_APP_FALHA_CONEXAO_HARD_TERCEIROS:
			d_StateMachineApp.State = STATE_APP_INICIA_CONEXOES;
		break;
//------------------------------------------------------------------------------------------------------------------------------------------------
		case STATE_APP_DESABILITADA:
			d_StateMachineApp.State = STATE_APP_INICIA_CONEXOES;
		break;
//------------------------------------------------------------------------------------------------------------------------------------------------
		case STATE_APP_FALHA_GERAL_E_CONTINGENCIA:   //usado em casos mais drasticos quando os outros estados de falha nao conseguem resolver
			d_StateMachineApp.State = STATE_APP_INICIA_CONEXOES;
		break;
//------------------------------------------------------------------------------------------------------------------------------------------------
		default:
			//d_StateMachineApp.AppStatusRegister0 = 1; //indica o atual estado
			//d_StateMachineApp.AppStatusRegister1 = (d_StateMachineApp.AppStatusRegister1 & 252);//zera outros estados (dois primeiros bits)
			d_StateMachineApp.State = STATE_APP_INICIA_CONEXOES;
		break;
	}
	return true;
}
