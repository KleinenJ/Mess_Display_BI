/**
  ******************************************************************************  
  * File Name          : edt_bsp_io.h
  * @author            : EDT Embedded Application Team
  * Description        : This file contains the common defines and functions 
  *                      prototypes for the edt_bsp_io.c driver.
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
#ifndef __EDT_BSP_IO_H
#define __EDT_BSP_IO_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
typedef enum 
{
  LED1 ,  
}EDT_IO_TypeDef;

typedef struct
{
  GPIO_TypeDef *port;
  uint8_t       pin;  
  bool          io_mode;  
}EDT_IO_InitTypeDef;

#define GPIO_IN         1
#define GPIO_OUT        0

#define __HAL_RCC_GPIO_CLK_ENABLE(__GPIOx__)   do { \
                      __IO uint32_t tmpreg; \
                      tmpreg = (0x1U << GPIO_GET_INDEX(__GPIOx__)); \
                      SET_BIT(RCC->AHB1ENR, tmpreg);\
                      /* Delay after an RCC peripheral clock enabling */ \
                      tmpreg = READ_BIT(RCC->AHB1ENR, tmpreg);\
                      UNUSED(tmpreg); \
                    } while(0)          

void          EDT_GPIO_Init(void);
void          EDT_LED_Init (EDT_IO_TypeDef Led);
void          EDT_LED_DeInit (EDT_IO_TypeDef Led);
void          EDT_LED_On (EDT_IO_TypeDef Led);
void          EDT_LED_Off (EDT_IO_TypeDef Led);
void          EDT_LED_Toggle (EDT_IO_TypeDef Led);
void          EDT_GPIO_Mode (GPIO_TypeDef *port, uint16_t pin, bool mode);
void          EDT_GPIO_Write (GPIO_TypeDef *port, uint16_t pin, bool value);
GPIO_PinState EDT_GPIO_Read (GPIO_TypeDef *port, uint16_t pin);  
uint8_t       EDT_GPIO_ReadALL(void);

#ifdef __cplusplus
}
#endif

#endif /* __EDT_BSP_IO_H */

 /*******(C) COPYRIGHT Emerging Display Technologies Corp. **** END OF FILE ***/


