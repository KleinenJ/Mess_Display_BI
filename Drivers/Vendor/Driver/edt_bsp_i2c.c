/**
  ******************************************************************************  
  * File Name          : edt_bsp_i2c.c
  * @author            : EDT Embedded Application Team
  * Description        : This file provides a set of firmware functions to I2C 
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
#include "edt_bsp_i2c.h"

I2C_HandleTypeDef I2cHandle;

/**
  * @brief  Initializes I2C HAL.
  * @param  i2c_handler : I2C handler
  * @retval None
  */
/*****************************************************************************
  * @brief  I2C2 Initial
  * @param  
  * @retval None
******************************************************************************/
void EDT_I2Cx_Init(void) 
{ 
/*##-1- Configure the I2C peripheral ######################################*/
  
  EDT_I2Cx_DeInit();
  
  I2cHandle.Instance             = I2Cx_INSTANCE;  
  I2cHandle.Init.Timing          = I2Cx_TIMING;  
  I2cHandle.Init.OwnAddress1     = 0xA5;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress2     = 0xFF;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; 
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
   
  if(HAL_I2C_Init(&I2cHandle) != HAL_OK) 
  {
    /* Initialization Error */
  }
}
/*****************************************************************************
  * @brief  I2C2 De-Initial
  * @param  
  * @retval None
******************************************************************************/
void EDT_I2Cx_DeInit(void) 
{    
  if(HAL_I2C_DeInit(&I2cHandle) != HAL_OK) 
  {
    /* DeInitialization Error */
  }
}

/*****************************************************************************
  *  * @brief I2CMX USER USED Tramsmite Interrupt
  *        This function frees the hardware resources used in this example:
  * @param hi2c         : I2C handle pointer
  *        DevAddress   : I2C Tramsmite Data pointer
  *        pdata        : I2C Tramsmite Data pointer
  *        Size         : I2C Tramsmite Data Length Max:8
  *        Timeout      : I2C Tramsmite wait time out
  * @retval             HAL_StatusTypeDef
******************************************************************************/
__weak HAL_StatusTypeDef EDT_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, \
    uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
       HAL_StatusTypeDef  Status  =  HAL_ERROR;   
       Status = HAL_I2C_Master_Transmit_IT(hi2c,DevAddress,pData,Size);
       return Status;
}
/*****************************************************************************
  *  * @brief I2CMX USER USED Receive Interrupt
  *        This function frees the hardware resources used in this example:
  * @param hi2c         : I2C handle pointer
  *        DevAddress   : I2C Receive Data pointer
  *        pdata        : I2C Receive Data pointer
  *        Size         : I2C Receive Data Length Max:8
  *        Timeout      : I2C Receive wait time out
  * @retval             HAL_StatusTypeDef
******************************************************************************/
__weak HAL_StatusTypeDef EDT_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, \
    uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
       HAL_StatusTypeDef  Status  =  HAL_ERROR;   
       Status =HAL_I2C_Master_Receive_IT(hi2c,DevAddress,pData,Size);
       return Status;
}
/*****************************************************************************
  *  * @brief I2CMX USER USED Tramsmite 
  *        This function frees the hardware resources used in this example:
  * @param hi2c         : I2C handle pointer
  *        DevAddress   : I2C device address
  *        MemAddress   : I2C Internal memory address
  *        MemAddSize:  : I2C Size of internal memory address
  *        pdata        : I2C Tramsmite Data pointer
  *        Size         : I2C Tramsmite Data Length Max:8
  *        Timeout      : I2C Tramsmite wait time out
  * @retval             HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef EDT_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, \
    uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, \
      uint8_t *pData, uint16_t Size, uint32_t Timeout)  
{
  return HAL_I2C_Mem_Write(hi2c, DevAddress, MemAddress, MemAddSize, pData, \
    Size, Timeout);  
}
/*****************************************************************************
  *  * @brief I2CMX USER USED Tramsmite 
  *        This function frees the hardware resources used in this example:
  * @param hi2c         : I2C handle pointer
  *        DevAddress   : I2C device address
  *        MemAddress   : I2C Internal memory address
  *        MemAddSize:  : I2C Size of internal memory address
  *        pdata        : I2C Tramsmite Data pointer
  *        Size         : I2C Tramsmite Data Length Max:8
  *        Timeout      : I2C Tramsmite wait time out
  * @retval             HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef EDT_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, \
  uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, \
    uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  return HAL_I2C_Mem_Read(hi2c, DevAddress, MemAddress, MemAddSize, pData, \
    Size, Timeout);  
}
/*****************************************************************************
  *  * @brief I2CMX USER USED Tramsmite Interrupt
  *        This function frees the hardware resources used in this example:
  * @param hi2c         : I2C handle pointer
  *        DevAddress   : I2C Tramsmite Data pointer
  *        pdata        : I2C Tramsmite Data pointer
  *        Size         : I2C Tramsmite Data Length Max:8
  *        Timeout      : I2C Tramsmite wait time out
  * @retval             HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef EDT_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, \
  uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  return HAL_I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout);
}
/*****************************************************************************
  *  * @brief I2CMX USER USED Receive 
  *        This function frees the hardware resources used in this example:
  * @param hi2c         : I2C handle pointer
  *        DevAddress   : I2C Receive Data pointer
  *        pdata        : I2C Receive Data pointer
  *        Size         : I2C Receive Data Length Max:8
  *        Timeout      : I2C Receive wait time out
  * @retval             HAL_StatusTypeDef
******************************************************************************/
HAL_StatusTypeDef EDT_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, \
  uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  return HAL_I2C_Master_Receive(hi2c, DevAddress, pData, Size, Timeout);
}
/*****************************************************************************
#  USER CODE I2Cx_EV_IRQHandler 
******************************************************************************/
void I2Cx_EV_IRQHandler(void)
{
    EDT_I2C_EV_IRQHandler();
}
/*****************************************************************************
#  USER CODE EDT_I2C_EV_IRQHandler 
******************************************************************************/
__weak void EDT_I2C_EV_IRQHandler(void)
{
   HAL_I2C_EV_IRQHandler(&I2cHandle);
}
/*****************************************************************************
  ##USER CODE I2Cx_ER_IRQHandler##
******************************************************************************/
void I2Cx_ER_IRQHandler(void)
{
   EDT_I2C_ER_IRQHandler();
}
/*****************************************************************************
  ##USER CODE EDT_I2C_ER_IRQHandler##
******************************************************************************/
__weak void EDT_I2C_ER_IRQHandler(void)
{
   HAL_I2C_ER_IRQHandler(&I2cHandle);
}
/*****************************************************************************
 void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
******************************************************************************/
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if(hi2c==&I2cHandle){
    /* ##USER CODE IRQn HAL_I2C_TxCpltCallback##*/
    EDT_I2C_MasterTxCpltCallback();
  }
}
/*****************************************************************************
 void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
******************************************************************************/
__weak void EDT_I2C_MasterTxCpltCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the SPI1_RxCpltCallback can be implemented in the user file
   */
}
/*****************************************************************************
 void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
******************************************************************************/
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if(hi2c==&I2cHandle){
     /* USER CODE IRQn HAL_I2C_RxCpltCallback */
    EDT_I2C_MasterRxCpltCallback();
  }
}
/*****************************************************************************
 void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
******************************************************************************/
__weak void EDT_I2C_MasterRxCpltCallback(void)
{
 /* NOTE : This function should not be modified, when the callback is needed,
            the SPI1_RxCpltCallback can be implemented in the user file
   */
}

 /*******(C) COPYRIGHT Emerging Display Technologies Corp. **** END OF FILE ***/

