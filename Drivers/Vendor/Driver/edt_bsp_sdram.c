/**
  ******************************************************************************  
  * File Name          : edt_bsp_sdram.c
  * @author            : EDT Embedded Application Team
  * Description        : This file includes the SDRAM driver for the memory
  *                      device mounted on smart embedded display boards.
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
//#ifdef HAL_SDRAM_MODULE_ENABLED
#include "edt_bsp_sdram.h"   

SDRAM_HandleTypeDef sdramHandle;
static FMC_SDRAM_TimingTypeDef Timing;
static FMC_SDRAM_CommandTypeDef Command;


/**
  * @brief  Initializes the SDRAM device.
  * @retval SDRAM status
  */
uint8_t EDT_SDRAM_Init(uint32_t bank)
{ 
  static uint8_t sdramstatus = SDRAM_ERROR;
  /* SDRAM device configuration */
  
    
  sdramHandle.Instance = FMC_SDRAM_DEVICE;
    
  /* Timing configuration for 100Mhz as SD clock frequency (System clock is up to 200Mhz) */
  Timing.LoadToActiveDelay    = 2;
  Timing.ExitSelfRefreshDelay = 7;
  Timing.SelfRefreshTime      = 4;
  Timing.RowCycleDelay        = 7;
  Timing.WriteRecoveryTime    = 2;
  Timing.RPDelay              = 2;
  Timing.RCDDelay             = 2;
  
  sdramHandle.Init.SDBank             = bank;
  sdramHandle.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;
  sdramHandle.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
  sdramHandle.Init.MemoryDataWidth    = SDRAM_MEMORY_WIDTH;
  sdramHandle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  sdramHandle.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;
  sdramHandle.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  sdramHandle.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
  sdramHandle.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
  sdramHandle.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

  if(HAL_SDRAM_Init(&sdramHandle, &Timing) != HAL_OK)
  {
    sdramstatus = SDRAM_ERROR;
  }
  else
  {
    sdramstatus = SDRAM_OK;
  }
  
  /* SDRAM initialization sequence */
  EDT_SDRAM_Initialization_sequence(&sdramHandle , REFRESH_COUNT ,  sdramHandle.Init.SDBank );
  
  return sdramstatus;
}

/**
  * @brief  DeInitializes the SDRAM device.
  * @retval SDRAM status
  */
uint8_t EDT_SDRAM_DeInit(void)
{ 
  static uint8_t sdramstatus = SDRAM_ERROR;
  /* SDRAM device de-initialization */
  sdramHandle.Instance = FMC_SDRAM_DEVICE;

  if(HAL_SDRAM_DeInit(&sdramHandle) != HAL_OK)
  {
    sdramstatus = SDRAM_ERROR;
  }
  else
  {
    sdramstatus = SDRAM_OK;
  }
  
  /* SDRAM controller de-initialization */
  
  return sdramstatus;
}

/**
  * @brief  Programs the SDRAM device.
  * @param  RefreshCount: SDRAM refresh counter value 
  * @retval None
  */
void EDT_SDRAM_Initialization_sequence(SDRAM_HandleTypeDef *hsdram , uint32_t RefreshCount, uint32_t bank)
{
  __IO uint32_t tmpmrd = 0;
  __IO uint32_t CommandTarget = 0;
  
  /* Step 1: Configure a clock configuration enable command */
  switch(bank)
  {
  default:
  case   FMC_SDRAM_BANK1:
	         CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
		  break;
  case   FMC_SDRAM_BANK2:
	         CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2;
		  break;
  }

  Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
  Command.CommandTarget          = CommandTarget;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, &Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */ 
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);
    
  /* Step 3: Configure a PALL (precharge all) command */ 
  Command.CommandMode            = FMC_SDRAM_CMD_PALL;
  Command.CommandTarget          = CommandTarget;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, &Command, SDRAM_TIMEOUT);
  
  /* Step 4: Configure an Auto Refresh command */ 
  Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command.CommandTarget          = CommandTarget;
  Command.AutoRefreshNumber      = 8;
  Command.ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, &Command, SDRAM_TIMEOUT);
  
  /* Step 5: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |\
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |\
                     SDRAM_MODEREG_CAS_LATENCY_2           |\
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |\
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  
  Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
  Command.CommandTarget          = CommandTarget;
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, &Command, SDRAM_TIMEOUT);
  
  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(hsdram, RefreshCount);
}
/**
  * @brief  Configure common DMA parameters.
  * @retval SDRAM status
  */
void EDT_SDRAM_LinkDMA(SDRAM_HandleTypeDef *hsdram)
{

#if defined(STM32F750xx)
	static DMA_HandleTypeDef dma_handle;

		__SDRAM_DMAx_CLK_ENABLE();

	  /* Configure common DMA parameters */
	  dma_handle.Init.Channel             = SDRAM_DMAx_CHANNEL;
	  dma_handle.Init.Direction           = DMA_MEMORY_TO_MEMORY;
	  dma_handle.Init.PeriphInc           = DMA_PINC_ENABLE;
	  dma_handle.Init.MemInc              = DMA_MINC_ENABLE;
	  dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	  dma_handle.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
	  dma_handle.Init.Mode                = DMA_NORMAL;
	  dma_handle.Init.Priority            = DMA_PRIORITY_HIGH;
	  dma_handle.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	  dma_handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	  dma_handle.Init.MemBurst            = DMA_MBURST_SINGLE;
	  dma_handle.Init.PeriphBurst         = DMA_PBURST_SINGLE;

	  dma_handle.Instance = SDRAM_DMAx_STREAM;

	   /* Associate the DMA handle */
	  __HAL_LINKDMA(hsdram, hdma, dma_handle);

	  /* Deinitialize the stream for new transfer */
	  HAL_DMA_DeInit(&dma_handle);

	  /* Configure the DMA stream */
	  HAL_DMA_Init(&dma_handle);

	  /* NVIC configuration for DMA transfer complete interrupt */
	  HAL_NVIC_SetPriority(SDRAM_DMAx_IRQn, SDRAM_IRQ_SetPriority, 0);
	  HAL_NVIC_EnableIRQ(SDRAM_DMAx_IRQn);
#elif defined(STM32H750xx)
	static MDMA_HandleTypeDef mdma_handle;

	__MDMAx_CLK_ENABLE();

	  /* Configure common MDMA parameters */
	mdma_handle.Init.Request = MDMA_REQUEST_SW;
	mdma_handle.Init.TransferTriggerMode = MDMA_BLOCK_TRANSFER;
	mdma_handle.Init.Priority = MDMA_PRIORITY_HIGH;
	mdma_handle.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
	mdma_handle.Init.SourceInc = MDMA_SRC_INC_WORD;
	mdma_handle.Init.DestinationInc = MDMA_DEST_INC_WORD;
	mdma_handle.Init.SourceDataSize = MDMA_SRC_DATASIZE_WORD;
	mdma_handle.Init.DestDataSize = MDMA_DEST_DATASIZE_WORD;
	mdma_handle.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
	mdma_handle.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
	mdma_handle.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
	mdma_handle.Init.BufferTransferLength = 128;
	mdma_handle.Init.SourceBlockAddressOffset = 0;
	mdma_handle.Init.DestBlockAddressOffset = 0;

	mdma_handle.Instance = SDRAM_MDMAx_CHANNEL;

	   /* Associate the DMA handle */
	__HAL_LINKDMA(hsdram, hmdma, mdma_handle);

	  /* Deinitialize the stream for new transfer */
	HAL_MDMA_DeInit(&mdma_handle);

	  /* Configure the DMA stream */
	HAL_MDMA_Init(&mdma_handle);

	  /* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(SDRAM_MDMAx_IRQn, FMC_IRQ_SetPriority, 0);
	HAL_NVIC_EnableIRQ(SDRAM_MDMAx_IRQn);

#endif
}
/**
  * @brief  Reads an amount of data from the SDRAM memory in polling mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read  
  * @param  uwDataSize: Size of read data from the memory
  * @retval SDRAM status
  */
uint8_t EDT_SDRAM_ReadData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
  if(HAL_SDRAM_Read_32b(&sdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return SDRAM_ERROR;
  }
  else
  {
    return SDRAM_OK;
  } 
}

/**
  * @brief  Reads an amount of data from the SDRAM memory in DMA mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read  
  * @param  uwDataSize: Size of read data from the memory
  * @retval SDRAM status
  */
uint8_t EDT_SDRAM_ReadData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize)
{
  if(HAL_SDRAM_Read_DMA(&sdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return SDRAM_ERROR;
  }
  else
  {
    return SDRAM_OK;
  }     
}

/**
  * @brief  Writes an amount of data to the SDRAM memory in polling mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written  
  * @param  uwDataSize: Size of written data from the memory
  * @retval SDRAM status
  */
uint8_t EDT_SDRAM_WriteData(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize) 
{
  if(HAL_SDRAM_Write_32b(&sdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return SDRAM_ERROR;
  }
  else
  {
    return SDRAM_OK;
  }
}

/**
  * @brief  Writes an amount of data to the SDRAM memory in DMA mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written  
  * @param  uwDataSize: Size of written data from the memory
  * @retval SDRAM status
  */
uint8_t EDT_SDRAM_WriteData_DMA(uint32_t uwStartAddress, uint32_t *pData, uint32_t uwDataSize) 
{
  if(HAL_SDRAM_Write_DMA(&sdramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return SDRAM_ERROR;
  }
  else
  {
    return SDRAM_OK;
  } 
}

/**
  * @brief  Sends command to the SDRAM bank.
  * @param  SdramCmd: Pointer to SDRAM command structure 
  * @retval SDRAM status
  */  
uint8_t EDT_SDRAM_Sendcmd(FMC_SDRAM_CommandTypeDef *SdramCmd)
{
  if(HAL_SDRAM_SendCommand(&sdramHandle, SdramCmd, SDRAM_TIMEOUT) != HAL_OK)
  {
    return SDRAM_ERROR;
  }
  else
  {
    return SDRAM_OK;
  }
}
//#endif
 /*******(C) COPYRIGHT Emerging Display Technologies Corp. **** END OF FILE ***/
