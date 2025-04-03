/**
  ******************************************************************************  
  * File Name          : edt_bsp_io.c
  * @author            : EDT Embedded Application Team
  * Description        : This file provides a set of firmware functions to 
  *                     external GPIO available on smart embedded display board. 
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
#include "edt_bsp_io.h"

/*****************************************************************
  * @brief  Configures GPIO_0~GPIO_5.
  * @retval None
  ****************************************************************/
void EDT_GPIO_Mode(GPIO_TypeDef * port,uint16_t pin, bool mode)
{
  GPIO_InitTypeDef  gpio_init_structure;
  
  /* Enable the GPIO_LED clock */

  __HAL_RCC_GPIO_CLK_ENABLE(port);

  /* Configure the GPIO_x pin */  
  if (mode==GPIO_IN) {
      gpio_init_structure.Mode = GPIO_MODE_INPUT;
      gpio_init_structure.Pull = GPIO_NOPULL;
  } else {
      gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;     
      gpio_init_structure.Pull = GPIO_PULLUP;
  }
  gpio_init_structure.Pin = pin;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW; 
  HAL_GPIO_Init(port, &gpio_init_structure);
}

void EDT_GPIO_Write (GPIO_TypeDef *port, uint16_t pin, bool value)
{
  if (value) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
  }
}

GPIO_PinState EDT_GPIO_Read (GPIO_TypeDef *port, uint16_t pin)
{
    return HAL_GPIO_ReadPin(port, pin);
}

uint8_t  EDT_GPIO_ReadALL(void)
{
   uint8_t gpioStatus =0;   
#if defined(GPIO0_Pin)
    if (EDT_GPIO_Read(GPIO0_GPIO_Port, GPIO0_Pin))  // read GPIO 0
      gpioStatus |= 0x01;
    else
      gpioStatus &= 0xfe;
#endif
#if defined(GPIO1_Pin)
    if (EDT_GPIO_Read(GPIO1_GPIO_Port, GPIO1_Pin))  // read GPIO 1
      gpioStatus |= 0x02;
    else
      gpioStatus &= 0xfd;
#endif
#if defined(GPIO2_Pin)
    if (EDT_GPIO_Read(GPIO2_GPIO_Port, GPIO2_Pin))  // read GPIO 2
      gpioStatus |= 0x04;
    else
      gpioStatus &= 0xfb;
#endif
#if defined(GPIO3_Pin)
    if (EDT_GPIO_Read(GPIO3_GPIO_Port, GPIO3_Pin))  // read GPIO 3
      gpioStatus |= 0x08;
    else
      gpioStatus &= 0xf7;
#endif
#if defined(GPIO4_Pin)
    if (EDT_GPIO_Read(GPIO4_GPIO_Port, GPIO4_Pin))  // read GPIO 4
      gpioStatus |= 0x10;
    else
      gpioStatus &= 0xef;
#endif
#if defined(GPIO5_Pin)
    if (EDT_GPIO_Read(GPIO5_GPIO_Port, GPIO5_Pin))  // read GPIO 5
      gpioStatus |= 0x20;
    else
      gpioStatus &= 0xdf;
#endif
    return gpioStatus;
}
/*********************************************************
  * @brief  Configures LED on GPIO.
  * @retval None
  *******************************************************/
void EDT_LED_Init(EDT_IO_TypeDef Led)
{  
  EDT_LED_DeInit(Led);
#if defined(LED_RTC_TS_Pin)
  EDT_GPIO_Mode(LED_RTC_TS_GPIO_Port, LED_RTC_TS_Pin, GPIO_OUT);
#endif
  EDT_LED_Off(Led);
}
/*****************************************************************
  * @brief  DeInit LEDs.
  * @param  Led: LED to be configured. 
  *          This parameter can be one of the following values:
  *            @arg  LED1
  * @note Led DeInit does not disable the GPIO clock
  * @retval None
  *******************************************************************/
void EDT_LED_DeInit(EDT_IO_TypeDef Led)
{
  /* Turn off LED */
  EDT_LED_Off(LED1);
  /* Configure the GPIO_LED pin */
#if defined(LED_RTC_TS_Pin)
  HAL_GPIO_DeInit(LED_RTC_TS_GPIO_Port, LED_RTC_TS_Pin);
#endif
}
/************************************************************************
  * @brief  Turns selected LED On.
  * @param  Led: LED to be set on 
  *          This parameter can be one of the following values:
  *            @arg  LED1
  * @retval None
  **********************************************************************/
void EDT_LED_On(EDT_IO_TypeDef Led)
{
#if defined(LED_RTC_TS_Pin)
  HAL_GPIO_WritePin(LED_RTC_TS_GPIO_Port, LED_RTC_TS_Pin, GPIO_PIN_RESET);
#endif
}
/************************************************************************
  * @brief  Turns selected LED Off. 
  * @param  Led: LED to be set off
  *          This parameter can be one of the following values:
  *            @arg  LED1
  * @retval None
  ***********************************************************************/
void EDT_LED_Off(EDT_IO_TypeDef Led)
{
#if defined(LED_RTC_TS_Pin)
  HAL_GPIO_WritePin(LED_RTC_TS_GPIO_Port, LED_RTC_TS_Pin, GPIO_PIN_SET);
#endif
}
/*************************************************************************
  * @brief  Toggles the selected LED.
  * @param  Led: LED to be toggled
  *          This parameter can be one of the following values:
  *            @arg  LED1
  * @retval None
  ************************************************************************/
void EDT_LED_Toggle(EDT_IO_TypeDef Led)
{
#if defined(LED_RTC_TS_Pin)
  HAL_GPIO_TogglePin(LED_RTC_TS_GPIO_Port, LED_RTC_TS_Pin);
#endif
} 

 /*******(C) COPYRIGHT Emerging Display Technologies Corp. **** END OF FILE ***/

