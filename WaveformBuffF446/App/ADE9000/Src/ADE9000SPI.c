/* Includes ------------------------------------------------------------------*/
#include "ADE9000SPI.h"
#include "stdbool.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;
extern osMutexId_t spiMutexHandle;

uint16_t dma_tx_buffer[DMA_BUFFER_SIZE_16_BITS] = {0};

// ========================================================================================================================================================
//
// 											FUNÇÕES "UNSAFE" (Sem Mutex - Para uso dentro da task em waveform.c)
//
// ========================================================================================================================================================

void ADE9000SPI_Write16_Unsafe(uint16_t Address, uint16_t Data)
{
	Address = ((Address << 4) & 0xFFF0);
	uint16_t rxData[2] = {0};

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&Address, (uint8_t*)&rxData[0], 1, 100);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&Data, (uint8_t*)&rxData[1], 1, 100);
	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);
}

void ADE9000SPI_Write32_Unsafe(uint16_t Address, uint32_t Data)
{
	Address = ((Address << 4) & 0xFFF0);
	uint16_t temp_highpacket = ((Data & 0xFFFF0000) >> 16);
	uint16_t temp_lowpacket = (Data & 0x0000FFFF);
	uint16_t rxData[3] = {0};

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&Address, (uint8_t*)&rxData[0], 1, 100);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&temp_highpacket, (uint8_t*)&rxData[1], 1, 100);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&temp_lowpacket, (uint8_t*)&rxData[2], 1, 100);
	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);
}

uint16_t ADE9000SPI_Read16_Unsafe(uint16_t Address)
{
	uint16_t returnData = 0;
	Address = (((Address << 4) & 0xFFF0) + 8);
	uint16_t txData;
	uint16_t rxData;

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);

	txData = Address;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)&rxData, 1, 100);

	txData = 0x0000;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)&returnData, 1, 100);

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);
	return returnData;
}

uint32_t ADE9000SPI_Read32_Unsafe(uint16_t Address)
{
	uint32_t returnData = 0;
	Address = (((Address << 4) & 0xFFF0) + 8);
	uint16_t txData, rxData;
	uint16_t temp_highpacket = 0;
	uint16_t temp_lowpacket = 0;

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);

	txData = Address;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)&rxData, 1, 100);

	txData = 0x0000;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)&temp_highpacket, 1, 100);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)&temp_lowpacket, 1, 100);

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);

	returnData = ((uint32_t)temp_highpacket << 16) | temp_lowpacket;
	return returnData;
}

// ========================================================================================================================================================
//
// 											FUNÇÕES SEGURAS (Com Mutex - Para uso geral fora da Task de Waveform)
//
// ========================================================================================================================================================

void ADE9000SPI_Write16(uint16_t Address, uint16_t Data)
{
	//osMutexAcquire(spiMutexHandle, osWaitForever);

	Address = ((Address << 4) & 0xFFF0);

	uint16_t rxData[2] = {0};

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&Address,(uint8_t*)&rxData[0],1,100);
	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&Data,(uint8_t*)&rxData[1],1,100);

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);

	//osMutexRelease(spiMutexHandle);
}

void ADE9000SPI_Write32(uint16_t Address , uint32_t Data )
{
	//osMutexAcquire(spiMutexHandle, osWaitForever);

	Address = ((Address << 4) & 0xFFF0);

	uint16_t temp_highpacket;
	uint16_t temp_lowpacket;
	uint16_t rxData[3] = {0};

	temp_highpacket= ((Data & 0xFFFF0000) >> 16);
	temp_lowpacket= (Data & 0x0000FFFF);

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&Address,(uint8_t*)&rxData[0],1,100);
	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&temp_highpacket,(uint8_t*)&rxData[1],1,100);
	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&temp_lowpacket,(uint8_t*)&rxData[2],1,100);

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);

	//osMutexRelease(spiMutexHandle);
}

uint16_t ADE9000SPI_Read16(uint16_t Address)
{
	uint16_t returnData = 0;

	//osMutexAcquire(spiMutexHandle, osWaitForever);

	Address = (((Address << 4) & 0xFFF0) + 8);

	uint16_t txData, rxData;

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);

	txData = Address;
	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&txData,(uint8_t *)&rxData,1,100);
	txData = 0x0000;
	HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&txData,(uint8_t *)&returnData,1,100);

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);

	//osMutexRelease(spiMutexHandle);
	return returnData;
}

uint32_t ADE9000SPI_Read32(uint16_t Address)
{

	uint32_t returnData = 0;

	//osMutexAcquire(spiMutexHandle, osWaitForever);

	Address = (((Address << 4) & 0xFFF0) + 8);

	uint16_t txData,rxData;
	uint16_t temp_highpacket = 0;
	uint16_t temp_lowpacket = 0;

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);

	txData = Address;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)&rxData, 1, 100);
	txData = 0x0000;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)&temp_highpacket, 1, 100);
	txData = 0x0000;
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)&temp_lowpacket, 1, 100);

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_SET);

	returnData = ((uint32_t)temp_highpacket << 16) | temp_lowpacket;

	//osMutexRelease(spiMutexHandle);
	return returnData;
}

void ADE9000SPI_BurstRead_DMA(uint16_t startAddr, int nWords,uint16_t *dma_rx_buffer)
{
	startAddr = (((startAddr <<4) & 0xFFF0) + 8);
	uint16_t txCommand = startAddr;
	uint16_t rxDummy = 0x0000;


	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&txCommand, (uint8_t*)&rxDummy, 1, HAL_MAX_DELAY);
	HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t*)dma_tx_buffer, (uint8_t*)dma_rx_buffer, nWords);
}

void ADE9000SPI_BurstRead_DMA_Start(uint16_t startAddr, int nWords, uint16_t *rx_buffer)
{
	startAddr = (((startAddr << 4) & 0xFFF0) + 8);
	uint16_t txCommand = startAddr;
	uint16_t rxDummy = 0x0000;

	HAL_GPIO_WritePin(CS_SPI1_GPIO_Port, CS_SPI1_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&txCommand, (uint8_t*)&rxDummy, 1, 10);

	HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t*)dma_tx_buffer, (uint8_t*)rx_buffer, nWords);
}
