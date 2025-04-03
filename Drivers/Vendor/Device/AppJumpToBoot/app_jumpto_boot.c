/**
  ******************************************************************************
  * @file    app_jumpto_boot.c
  * @author  MCD Application Team
  * @brief   OpenBootloader application entry point
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_jumpto_boot.h"
#include <stdbool.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if defined (STM32F750xx)

#ifndef F750_OP_BOOT_EN
#define F750_OP_BOOT_EN       0X00AA
#endif

#ifndef F750_OP_BOOT_DN
#define F750_OP_BOOT_DN       0X0040
#endif
#elif defined (STM32H750xx)

#ifndef H750_OP_BOOT_EN
#define H750_OP_BOOT_EN       0XAA00
#endif
#ifndef H750_OP_BOOT_DN
#define H750_OP_BOOT_DN       0X1FF0
#endif
#endif

static uint16_t OpenBootloader_Read_BootOBFlash(void);
static void OpenBootloader_Write_BootOBFlash(uint16_t value);

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to select which protocol will be used when communicating with the host.
  * @param  None.
  * @retval None.
  */
void OpenBootloader_JumpToBootloader(void)
{
	#if defined (STM32F750xx)
	if(OpenBootloader_Read_BootOBFlash()==F750_OP_BOOT_DN)
	 OpenBootloader_Write_BootOBFlash(F750_OP_BOOT_EN);
	#elif defined (STM32H750xx)
	 if(OpenBootloader_Read_BootOBFlash()==H750_OP_BOOT_DN)
	  OpenBootloader_Write_BootOBFlash(H750_OP_BOOT_EN);
	#endif
	NVIC_SystemReset();
}


#if defined (STM32F750xx)||defined(STM32H750xx)
/*****************************************************************************
      When want to Check  Bootloader flag (STM32F750/STM32H750)
*****************************************************************************/
#if 1
static uint16_t OpenBootloader_Read_BootOBFlash(void)
{
 static uint16_t mode;
 FLASH_OBProgramInitTypeDef OBInit;

#if defined (STM32F750xx)

  HAL_FLASHEx_OBGetConfig(&OBInit);
  mode =  OBInit.BootAddr1;

#elif defined (STM32H750xx)

  if(HAL_FLASH_Unlock()== HAL_OK){
   if(HAL_FLASH_OB_Unlock()== HAL_OK){
  static uint32_t mode32;

  OBInit.Banks     = FLASH_BANK_1;
  HAL_FLASHEx_OBGetConfig(&OBInit);


  mode32 =  OBInit.BootAddr1;
  mode = mode32>>16;
     if(HAL_FLASH_OB_Lock()== HAL_OK){
            HAL_FLASH_Lock();
     }
   }
}
#endif
  return mode;
}
#endif
/*******************************************************************************
    When want to Go To Bootloader Setting Bootloader flag (STM32F750/STM32H750)
*******************************************************************************/
static void OpenBootloader_Write_BootOBFlash(uint16_t value)
{
#if 1
  FLASH_OBProgramInitTypeDef OBInit;
#if defined (STM32F750xx)
  HAL_FLASHEx_OBGetConfig(&OBInit);

  OBInit.BootAddr1 = value;
  OBInit.WRPState = OB_RDP_LEVEL_0;
  if(HAL_FLASH_Unlock()== HAL_OK){

   if(HAL_FLASH_OB_Unlock()== HAL_OK){

         HAL_FLASHEx_OBProgram(&OBInit);
         HAL_FLASH_OB_Launch();
         if(HAL_FLASH_OB_Lock()== HAL_OK){
            HAL_FLASH_Lock();
         }
      }
    }
#elif defined (STM32H750xx)
   HAL_FLASHEx_OBGetConfig(&OBInit);
   OBInit.OptionType=OPTIONBYTE_BOOTADD;
   OBInit.BootConfig=OB_BOOT_ADD1;
   OBInit.BootAddr1 = value<<16;
   OBInit.Banks=FLASH_BANK_1;
   OBInit.WRPState = OB_RDP_LEVEL_1;
   if(HAL_FLASH_OB_Unlock()== HAL_OK)
   {
   if(HAL_FLASH_Unlock()== HAL_OK)
   {
      if(HAL_FLASHEx_OBProgram(&OBInit)==HAL_OK)
       {
         HAL_FLASH_OB_Launch();
         if(HAL_FLASH_OB_Lock()== HAL_OK)
         {
            HAL_FLASH_Lock();
         }
       }
    }
   }
#endif
#endif
}
#endif
