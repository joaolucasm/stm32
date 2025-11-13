#pragma once
/* --------------------------------- Includes ---------------------------------- */
#include "ADE9000.h"
#include "app_config.h"
#include <stdbool.h>
#include <stdint.h>
#include "calibration.h"

/*  -------------------------------- Defines ----------------------------------- */

enum StatusRunningAPP_e //lista de estados possiveis para a maquina de estados da aplicacao:
{
	STATE_APP_RUNNING_NORMAL = 0,            //PDR >> 90%
	STATE_APP_RUNNING_BAIXAS_RESTRICOES,     // 60 << PDR << 90
	STATE_APP_RUNNING_MODERADAS_RESTRICOES,  // 30 << PDR << 60
	STATE_APP_RUNNING_MUITAS_RESTRICOES,     // 0 << PDR << 30
};

enum StatesAPP_e //lista de estados possiveis para a maquina de estados da aplicacao:
{
	STATE_APP_INICIA_CONEXOES = 0,
	STATE_APP_FAZ_REGISTRY_SERV_APP,
	STATE_APP_VERIFICA_COD_ID,
	STATE_APP_EM_EXECUCAO,
	STATE_APP_DESABILITADA,
	STATE_APP_FALHA_GERAL_E_CONTINGENCIA,
	STATE_APP_FALHA_CONEXAO_HARD_TERCEIROS,
	STATE_APP_FALHA_REDE,
	STATE_APP_FALHA_REGISTRO_APP,
	STATE_APP_INICIA_VARIAVEIS_E_CARREGAMENTOS,
};

typedef struct EnernetMeasurements_s  //usada apenas para testar alarmes com dados sinteticos. Nao tem efeito funcional no programa
{
	ADE9000_EnergyMeasurements EnergyMeasurements;              		//estrutura de dados oferecida pela Analog Devices para o CI ADE9000 que contem as medidas eletricas
	float AcumuladorEnergiaAparenteTotal_kva;                   		//acumula a energia aparente desde quando eh ligado (ou resetado)
	float AcumuladorEnergiaAtivaTotal_kw;                       		//acumula a energia aparente desde quando eh ligado (ou resetado)
	float AcumuladorEnergiaReativaTotal_kvar;                   		//acumula a energia aparente desde quando eh ligado (ou resetado)
	float AcumuladorEnergiaAtiva24h;                          			//acumula a energia aparente para pacote de 24h
} EnernetMeasurements_t ;

typedef struct StateMachineData_s    									//variaveis associadas a maquina de estados do state grid:
{
	//GRUPO 1: grupo de variaveis de controle ou estado dinamico (sao mudados no transcorrer do programa e nao sao gravadas na memoria flash interna):
	bool  ConnectedElectricMeterInterface;        			  			//indica se o hardware de medicao de parametros eletricos foi inicializado com sucesso (seja ele de terceiros ou interface local)
	uint16_t QtdTentativasReConexao;                     	   			//usado para contabilizar em diversas funcoes quantas vezes tentou reconectar a alguma coisa (hardware de terceiros, fazer registry, etc)
	enum StatusRunningAPP_e StatusRunning;                     			// avalia como a app pode rodar: normal, com alguma restricao.l Isto depende dos KPIs da aplicacao e eh usado pelo algoritmo da propria aplicacao para tentar manter ela viva em modo standalone

	int novaLeituraMedidas;                                   			//usado para indicar se foi feita nova leitura de medidas. Usado pelo sistema de alarme para ver se tem que avaliar novos alarmes ou nao
	int tempoAppEmExecucaoSeg;
	int tempoAppEmExecucaoMin;
	int tempoAppEmExecucaoHoras;
	int      tempoAppEmExecucaoDias;
	enum StatesAPP_e State; 								   			//lista de estados da aplicacao
	bool deveImprimirTabelaValores;                            			//usado para fins de depuracao para imprimir na tela valores de depuracao
	uint8_t  AppStatusRegister2;
	uint8_t  AppStatusRegister1;
	uint8_t  AppStatusRegister0;
	bool habExecucaoApp; 									   			//variaveil para saber se está habil para leitura
	//GRUPO 2: variaveis de contagem de tempo da aplicacao (sao contabilizadas nao na aplicacao mas externamente por diferentes timers)
	int     contTempoEmFalhaMinu;                           			//conta quantidade de tempo que o dispositivo esta em estado de falha e tenta voltar novamente
	int  	contTempoNotificacaoEnvMsgMin;                  			//usado para contar tempo entre um envio e outro de um pacote de notificacao (se este recurso estiver habilitado)
	int  	contTempoMonitoramentoVariaveisFimMin;          			//armazena por quanto tempo uma variavel tem que ficar sendo monitorada
	int  	contTempoMonitoramentoVariaveisFreqAquiSeg;     			//armazena por quanto tempo uma variavel tem que ficar sendo monitorada
	int  	contTempoEntreLeiturasSeg;                      			//usado para contar o tempo decorrido entre uma medida em outra quando o dispositivo esta em estado de notificacao
	int  	freqCalculoSeg;								   				//frequencia de tempo para leituras do ADE9000
	int  	contTempoLifeTimeMin;                           			//controlar tempo para renovacao de lifetime
	int  	contaTempoTentativaReconexoesHardTerceirosSeg;
	int   	contaTempoEsperandoCadastroHoras;               			//usado para ver a quanto tempo esta esperando cadastro

	//GRUPO 3: variaveis de medição dos valores
	EnernetMeasurements_t  EnernetInstantaneousMeasurements;   			//guarda medidas instantaneas
	int PotenciaNominalKVA;    								   			//potencia nominal do transformador
	bool habStaticCalcApp;									   			//habilita o calculo dos valores estátisticos de medida elétricas

} StateMachineData_t;

typedef struct StatisticalValues_s    //variaveis associadas as medidas que devem ser enviadas por algum pacote:
{
	//variaves de controle dos pacotes estatisticos
	int contadorMedidasFeitas_pctNotificacao;               			//usado pela funcao de calculo de media e desvio padrao para o pacote 50(notificacao). Precisa saber qtas medidas foram feitas para se tirar media e desvio padrao
	int       contadorMedidasFeitas_pct24h;                    		 	//usado pela funcao de calculo de media e desvio padrao para o pacote 51 (de 24h). Precisa saber qtas medidas foram feitas para se tirar media e desvio padrao

	//Variaveis para alertas e alarmes. instantaneas (podem ser usadas em alertas e alarmes)
	uint8_t desequilibrioTensaoInstantaneo;                             //valores instantaneos de desequilibrio
	uint8_t desequilibrioCorrenteInstantaneo;                           //valores instantaneos de desequilibrio

	//Variaveis do pacote de notificacao (pacote Notificacao):
	float anguloMedioTensaoEntreFases_pctNotificacao[NUM_FASES_SEP];
	float desvioPadraoAnguloMedioTensaoEntreFases_pctNotificacao[NUM_FASES_SEP];
	float anguloMedioTensaoEntreFases_pct24h[NUM_FASES_SEP];
	float desvioPadraoAnguloMedioTensaoEntreFases_pct24h[NUM_FASES_SEP];
	float fatorPotenciaMedio_pctNotificacao[NUM_FASES_SEP];  			//No StateGrid vai de 0 ate 100
	float fatorPotenciaMedio_pct24h[NUM_FASES_SEP];          			//No StateGrid vai de 0 ate 100
	float desvioPadraoFatorPotencia_pctNotificacao[NUM_FASES_SEP];  	//No StateGrid vai de 0 ate 100
	float desvioPadraoFatorPotencia_pct24h[NUM_FASES_SEP];          	//No StateGrid vai de 0 ate 100
	float potReativaMedia_pctNotificacao[NUM_FASES_SEP];
	float potReativaMedia_pct24h[NUM_FASES_SEP];
	float desvioPadraoPotReativa_pct24h[NUM_FASES_SEP];
	float potAtivaMedia_pctNotificacao[NUM_FASES_SEP];
	float potAtivaMedia_pct24h[NUM_FASES_SEP];
	float desvioPadraoPotAtiva_pct24h[NUM_FASES_SEP];
	float potAparenteMedia_pctNotificacao[NUM_FASES_SEP];
	float potAparenteMedia_pct24h[NUM_FASES_SEP];
	float desvioPadraoPotAparente_pct24h[NUM_FASES_SEP];
	float desequilibrioMedioTensaoEntreFases_pctNotificacao;     		//Vai ser medido assim: Se RG1=fase A, RG2=fase B, RG3= face C. As medidas serao: Vab, Vbc e Vca.
	float desequilibrioMedioTensaoEntreFases_pct24h;     				//Vai ser medido assim: Se RG1=fase A, RG2=fase B, RG3= face C. As medidas serao: Vab, Vbc e Vca.
	float desequilibrioMedioCorrenteEntreFases_pctNotificacao;   		//Vai ser medido assim: Se RG1=fase A, RG2=fase B, RG3= face C. As medidas serao: Vab, Vbc e Vca.
	float desequilibrioMedioCorrenteEntreFases_pct24h;   				//Vai ser medido assim: Se RG1=fase A, RG2=fase B, RG3= face C. As medidas serao: Iab, Ibc e Ica.
	float frequenciaRedeMedia_pctNotificacao[NUM_FASES_SEP];
	float frequenciaRedeMedia_pct24h[NUM_FASES_SEP];
	int   PicoTensaoPositivo_pct24h[NUM_FASES_SEP];
	int   PicoTensaoNegativo_pct24h[NUM_FASES_SEP];
	float MenorFatorPotencia_pct24h[NUM_FASES_SEP];   					//No StateGrid vai de 0 ate 100
	int   MaiorDesequilibrioTensao_pct24h;
	int   MaiorDesequilibrioTensao_pctNotificacao;
	int   MaiorDesequilibrioCorrente_pct24h;
	int   MaiorDesequilibrioCorrente_pctNotificacao;
	int   PicoCorrente_pct24h[NUM_FASES_SEP];
	int   PicoPotenciaAparenteTotal_pct24h;
	float distorcaoHarmonicaCorrente_pctNotificacao[NUM_FASES_SEP];		// %
	float distorcaoHarmonicaCorrente_pct24h[NUM_FASES_SEP];
	float distorcaoHarmonicaTensao_pctNotificacao[NUM_FASES_SEP];   	// %
	float distorcaoHarmonicaTensao_pct24h[NUM_FASES_SEP];
	float desvioPadraoDistorcaoHarmonicaCorrente_pct24h[NUM_FASES_SEP];
	float desvioPadraoDistorcaoHarmonicaTensao_pct24h[NUM_FASES_SEP];

	float tensaoRMSmedia_pct24h[NUM_FASES_SEP];
	float tensaoRMSmedia_pctNotificacao[NUM_FASES_SEP];
	float desvioPadraoTensaoRMS_pctNotificacao[NUM_FASES_SEP];
	float desvioPadraoTensaoRMS_pct24h[NUM_FASES_SEP];
	float correnteRMSmedia_pctNotificacao[NUM_FASES_SEP];
	float correnteRMSmedia_pct24h[NUM_FASES_SEP];
	float desvioPadraoCorrenteRMS_pctNotificacao[NUM_FASES_SEP];
	float desvioPadraoCorrenteRMS_pct24h[NUM_FASES_SEP];

	int EnergiaAparente_pctNotificacao[NUM_FASES_SEP];              	//VA
	int EnergiaAparente_pct24h[NUM_FASES_SEP];
	int EnergiaAtiva_pctNotificacao[NUM_FASES_SEP];                 	//W
	int EnergiaAtiva_pct24h[NUM_FASES_SEP];
	int EnergiaReativa_pctNotificacao[NUM_FASES_SEP];              		//kW/h
	int EnergiaReativa_pct24h[NUM_FASES_SEP];

} StatisticalValues_t;  //obs.: se incluir nova variavel, nao esquecer de ir na funcao "ProcessaEstatisticasMedidas" e inicializar ela


//--------------------------- Predefinição das funções utilizadas no arquivo principal --------------------------- //
bool stateMachine(void);
void MainBuild_InitiateApp(void);
void App_loop(void);
void AppMainBuild_General_AppRunning(StateMachineData_t *d_StateMachineApp_cp);
