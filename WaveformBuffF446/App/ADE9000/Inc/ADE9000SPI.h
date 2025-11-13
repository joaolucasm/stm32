/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADE9000SPI_H
#define __ADE9000SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stm32f446xx.h>
#include <sys/_stdint.h>
#include "cmsis_os2.h"


/* Function prototypes ----------------------------------------------------------*/
void ADE9000SPI_Write16(uint16_t Address, uint16_t Data);
void ADE9000SPI_Write32(uint16_t Address , uint32_t Data );
uint16_t ADE9000SPI_Read16(uint16_t Address);
uint32_t ADE9000SPI_Read32(uint16_t Address);
void ADE9000SPI_BurstRead_DMA(uint16_t startAddr, int nWords,uint16_t *dma_rx_buffer);

#ifdef __cplusplus
}
#endif

#endif /*__ ADE9000SPI_H */

