/**
  ******************************************************************************  
  * File Name          : edt_bsp_spi.c
  * @author            : EDT Embedded Application Team
  * Description        : This file provides a set of firmware functions to SPI 
  *                      communicate with  external devices available on smart 
  *                      embedded display board. 
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
/* Includes ------------------------------------------------------------------*/
#include "edt_bsp_spi.h"

SPI_HandleTypeDef hspix;
/****************************************************************************************
  * @brief  EDT_SPI_Init
  * @param  NONE
  * @retval NONE
  * @note   EDT SPI Initial
****************************************************************************************/
void EDT_SPIx_Init(void)
{
  
  EDT_SPIx_DeInit();
    
  hspix.Instance = SPIx_INSTANCE;
  hspix.Init.Mode = SPI_MODE_MASTER;
  hspix.Init.Direction = SPI_DIRECTION_2LINES;
  hspix.Init.DataSize = SPI_DATASIZE_8BIT;
  hspix.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspix.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspix.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspix.Init.BaudRatePrescaler = SPIx_BAUDRATE;
  
  hspix.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspix.Init.TIMode = SPI_TIMODE_DISABLE;
  hspix.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspix.Init.CRCPolynomial = 7;
  hspix.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspix.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspix) != HAL_OK)
  {    
  }
} 
/******************************************************************************
  * @brief  EDT_SPI_De-Init
  * @param  NONE
  * @retval NONE
  * @note   EDT SPI De-Initial
*******************************************************************************/
void EDT_SPIx_DeInit(void)
{ 
  if (HAL_SPI_DeInit(&hspix) != HAL_OK)
  {   
  }
} 
/*****************************************************************************
  * @brief SPI MX USER USED TramsmiteReceive
  *        This function frees the hardware resources used in this example:
  * @param hspi      : SPI handle pointer
  *        pTxData   : SPI Tramsmite Data pointer
  *        pRxData   : SPI Recieve Data pointer
  *        Size      : SPI Tramsmite & Recieve Data Length
  *        Timeout   : SPI TramsmiteReceive wait time out
  * @retval HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef EDT_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, \
  uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{ 
   return HAL_SPI_TransmitReceive(hspi, pTxData, pRxData, Size, Timeout);
}
/*****************************************************************************
  * @brief SPI MX USER USED Tramsmite 
  *        This function frees the hardware resources used in this example:
  * @param hspi    : SPI handle pointer
  *        pdata   : SPI Tramsmite Data pointer
  *        Size    : SPI Tramsmite Data Length
  *        Timeout : SPI Tramsmite wait time out
  * @retval HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef EDT_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, \
  uint16_t Size, uint32_t Timeout)
{
   return HAL_SPI_Transmit(hspi, pData,  Size,  Timeout);
}
/*****************************************************************************
  * @brief SPI MX USER USED Receive
  *        This function frees the hardware resources used in this example:
  * @param hspi    : SPI handle pointer
  *        pdata   : SPI Receive Data pointer
  *        Size    : SPI Receive Data Length
  *        Timeout : SPI Receive wait time out
  * @retval HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef EDT_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, \
  uint16_t Size, uint32_t Timeout)
{
   return HAL_SPI_Receive(hspi, pData,  Size,  Timeout);
}
/*****************************************************************************
  * @brief SPI MX USER USED TramsmiteReceive Interrupt
  *        This function frees the hardware resources used in this example:
  * @param hspi      : SPI handle pointer
  *        pTxData   : SPI Tramsmite Data pointer
  *        pRxData   : SPI Recieve Data pointer
  *        Size      : SPI Tramsmite & Recieve Data Length
  *        Timeout   : SPI TramsmiteReceive wait time out
  * @retval HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef EDT_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, \
  uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
  HAL_StatusTypeDef Status;
  Status = HAL_SPI_TransmitReceive_IT(hspi, pTxData, pRxData, Size);
  return Status;
}
/*****************************************************************************
  * @brief SPI MX USER USED Tramsmite Interrupt
  *        This function frees the hardware resources used in this example:
  * @param hspi      : SPI handle pointer
  *        pData     : SPI Tramsmite Data pointer
  *        Size      : SPI Tramsmite Data Length
  *        Timeout   : SPI TramsmiteReceive wait time out
  * @retval HAL_StatusTypeDef
******************************************************************************/
__weak HAL_StatusTypeDef EDT_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, \
  uint8_t *pData, uint16_t Size)
{
   HAL_StatusTypeDef Status =HAL_ERROR;
   Status =  HAL_SPI_Transmit_IT(hspi, pData, Size); 
   return Status;
}
/*****************************************************************************
  * @brief SPI MX USER USED Receive Interrupt
  *        This function frees the hardware resources used in this example:
  * @param hspi      : SPI handle pointer
  *        pData     : SPI Recieve Data pointer
  *        Size      : SPI Recieve Data Length
  *        Timeout   : SPI Recieve Data wait time out
  * @retval HAL_StatusTypeDef
******************************************************************************/
__weak HAL_StatusTypeDef EDT_SPI_Receive_IT(SPI_HandleTypeDef *hspi, \
  uint8_t *pData, uint16_t Size)
{
   HAL_StatusTypeDef Status=HAL_ERROR;
   Status = HAL_SPI_Receive_IT(hspi, pData,  Size);  
   return Status;
}
/*****************************************************************************
  * USED SPIx IRQHandler  FUNCTION 
*****************************************************************************/
void SPIx_IRQHandler(void)
{
  /* USER CODE SPI1 IRQHandler */
  EDT_SPI_IRQHandler();
}
/*****************************************************************************
  * USED SPI1 IRQHandler Callback FUNCTION 
*****************************************************************************/
__weak void EDT_SPI_IRQHandler(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the SPI1_IRQ_Callback can be implemented in the user file
   */
   HAL_SPI_IRQHandler(&hspix);
}
/*****************************************************************************
USED BYTE SPI Transmite Finlish Irq CallBack 
*****************************************************************************/
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if(hspi == &hspix){    
     EDT_SPI_TxCpltCallback();
     /* USER CODE BEGIN Transmite finlish interrupt */
  }
}
/*****************************************************************************
  * USED SPI1 Transmite finlish interrupt Callback FUNCTION 
*****************************************************************************/
__weak void EDT_SPI_TxCpltCallback(void)
{
   /* NOTE : This function should not be modified, when the callback is needed,
            the SPI1_TxCpltCallback can be implemented in the user file
   */
}
/*****************************************************************************
USED BYTE SPI Receive Finlish Irq CallBack 
*****************************************************************************/
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if(hspi == &hspix){  
     /* USER CODE BEGIN Receive finlish interrupt by */
     EDT_SPI_RxCpltCallback();
  }
}
/*****************************************************************************
  * USED SPI1 Receive finlish interrupt Callback FUNCTION 
*****************************************************************************/
__weak void EDT_SPI_RxCpltCallback(void)
{
   /* NOTE : This function should not be modified, when the callback is needed,
            the SPI1_RxCpltCallback can be implemented in the user file
   */
}

 /*******(C) COPYRIGHT Emerging Display Technologies Corp. **** END OF FILE ***/

