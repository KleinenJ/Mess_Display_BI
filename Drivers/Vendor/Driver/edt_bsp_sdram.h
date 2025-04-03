/**
  ******************************************************************************  
  * File Name          : edt_bsp_sdram.h
  * @author            : EDT Embedded Application Team
  * Description        : This file contains the common defines and functions 
  *                      prototypes for the edt_bsp_sdram.c driver.
  * @brief             : EDT <https://smartembeddeddisplay.com/>
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT(c) 2020 Emerging Display Technologies Corp.
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of Emerging Display Technologies Corp. nor the names of 
  *      its contributors may be used to endorse or promote products derived from 
  *      this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EDT_BSP_SDRAM_H
#define __EDT_BSP_SDRAM_H

#ifdef __cplusplus
 extern "C" {
#endif 

//#ifdef HAL_SDRAM_MODULE_ENABLED
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** 
  * @brief  SDRAM status structure definition  
  */     
#define SDRAM_OK         ((uint8_t)0x00)
#define SDRAM_ERROR      ((uint8_t)0x01)

#if defined(STM32F750xx)
#define SDRAM_DEVICE_ADDR  ((uint32_t)0xC0000000)
#define SDRAM_DEVICE_SIZE  ((uint32_t)0x800000)  /* SDRAM device size in MBytes */
#else
#define SDRAM_DEVICE_ADDR  ((uint32_t)0xD0000000)
#define SDRAM_DEVICE_SIZE  ((uint32_t)0x1000000)  /* SDRAM device size in MBytes */
#endif

#if defined(FMC16)
	#define SDRAM_MEMORY_WIDTH               FMC_SDRAM_MEM_BUS_WIDTH_16
#else
	#define SDRAM_MEMORY_WIDTH               FMC_SDRAM_MEM_BUS_WIDTH_32
#endif   

#define SDCLOCK_PERIOD                   FMC_SDRAM_CLOCK_PERIOD_2 

#define REFRESH_COUNT                    ((uint32_t)0x0603)   /* SDRAM refresh counter (100Mhz SD clock) */
   
#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)
 
#if defined(STM32F750xx)

#define __SDRAM_DMAx_CLK_ENABLE           __HAL_RCC_DMA2_CLK_ENABLE
#define __SDRAM_DMAx_CLK_DISABLE          __HAL_RCC_DMA2_CLK_DISABLE
#define SDRAM_DMAx_CHANNEL                DMA_CHANNEL_0
#define SDRAM_DMAx_STREAM                 DMA2_Stream0
#define SDRAM_DMAx_IRQn                   DMA2_Stream0_IRQn
#define SDRAM_IRQ_SetPriority             0x0F

#elif defined(STM32H750xx)
#define __MDMAx_CLK_ENABLE          __HAL_RCC_MDMA_CLK_ENABLE
#define __MDMAx_CLK_DISABLE         __HAL_RCC_MDMA_CLK_DISABLE

#define SDRAM_MDMAx_CHANNEL         MDMA_Channel0
#define SDRAM_MDMAx_IRQn            MDMA_IRQn

#define FMC_IRQ_SetPriority     0x0F
#endif


/**
  * @brief  FMC SDRAM Mode definition register defines
  */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

uint8_t EDT_SDRAM_Init(uint32_t bank);
uint8_t EDT_SDRAM_DeInit(void);
void EDT_SDRAM_Initialization_sequence(SDRAM_HandleTypeDef *hsdram , uint32_t RefreshCount, uint32_t bank);
void EDT_SDRAM_LinkDMA(SDRAM_HandleTypeDef *hsdram);
uint8_t EDT_SDRAM_ReadData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t EDT_SDRAM_ReadData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t EDT_SDRAM_WriteData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t EDT_SDRAM_WriteData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize);
uint8_t EDT_SDRAM_Sendcmd(FMC_SDRAM_CommandTypeDef *SdramCmd);

//#endif

#ifdef __cplusplus
}
#endif

#endif /* __EDT_BSP_SDRAM_H */

 /*******(C) COPYRIGHT Emerging Display Technologies Corp. **** END OF FILE ***/
