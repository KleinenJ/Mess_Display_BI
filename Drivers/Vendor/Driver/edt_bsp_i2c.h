/**
  ******************************************************************************  
  * File Name          : edt_bsp_i2c.h
  * @author            : EDT Embedded Application Team
  * Description        : This file contains the common defines and functions 
  *                      prototypes for the edt_bsp_i2c.c driver.
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
#ifndef __EDT_BSP_I2C_H
#define __EDT_BSP_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      10
/* Size of Reception buffer */
#define RXBUFFERSIZE                      10

extern I2C_HandleTypeDef I2cHandle;

void EDT_I2Cx_Init(void);
void EDT_I2Cx_DeInit(void); 
HAL_StatusTypeDef EDT_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, \
    uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef EDT_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, \
    uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef EDT_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, \
    uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, \
    uint8_t *pData, uint16_t Size, uint32_t Timeout);  
HAL_StatusTypeDef EDT_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, \
    uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, \
    uint8_t *pData, uint16_t Size, uint32_t Timeout);  
HAL_StatusTypeDef EDT_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, \
    uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef EDT_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, \
  uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

void EDT_I2C_EV_IRQHandler(void);
void EDT_I2C_ER_IRQHandler(void);
void EDT_I2C_MasterTxCpltCallback(void);
void EDT_I2C_MasterRxCpltCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __EDT_BSP_I2C_H */

 /*******(C) COPYRIGHT Emerging Display Technologies Corp. **** END OF FILE ***/


