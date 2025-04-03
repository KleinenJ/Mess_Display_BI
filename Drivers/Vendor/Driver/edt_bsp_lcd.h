/**
  ******************************************************************************  
  * File Name          : edt_bsp_lcd.h
  * @author            : EDT Embedded Application Team
  * Description        : This file contains the common defines and functions 
  *                      prototypes for the edt_bsp_lcd.c driver.
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
#ifndef __EDT_BSP_LCD_H
#define __EDT_BSP_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 
   
/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"  
#include "main.h"
#include "edt_bsp_sdram.h"
extern LTDC_HandleTypeDef  hltdc;
extern DMA2D_HandleTypeDef  hdma2d;

 typedef struct
 {
   bool lcdstate;           //LCD ON/OFF
   uint16_t width;
   uint16_t height;
 }LCD_StructDef;

#define  LCD_LayerCfgTypeDef    LTDC_LayerCfgTypeDef
   
#define  LCD_OK                 ((uint8_t)0x00)
#define  LCD_ERROR              ((uint8_t)0x01)
#define  LCD_TIMEOUT            ((uint8_t)0x02)

uint8_t  EDT_LCD_Init(void);
uint8_t  EDT_LCD_DeInit(void);
uint32_t EDT_LCD_GetXSize(void);
uint32_t EDT_LCD_GetYSize(void);
void     EDT_LCD_SetSize(LCD_StructDef * lcd, uint16_t width, uint16_t height);
void     EDT_LCD_DisplayOff(void);
void     EDT_LCD_DisplayOn(void);
void     EDT_LCD_SetDisplayStatus(bool lcdstatus);
bool     EDT_LCD_GetDisplayStatus(void);
void     EDT_LCD_Reset(void);
void     EDT_LCD_Clear(LTDC_HandleTypeDef *hLtdcHandler , uint32_t Color);
#if !defined(USE_BL_PWM)
void     EDT_LCD_BL_ON(void);
void     EDT_LCD_BL_OFF(void);
#endif
void     EDT_LCD_LayerInit(uint16_t LayerIndex, uint32_t FB_Address);

/****************sleep function*****************************
   1. initial Sleep function first: "EDT_LCD_SleepFun_Init()"
   2. provide the sleep time : "EDT_LCD_Sleep_Time(20)"
      The display will turn off after 20 seconds.
 ***********************************************************/
void    EDT_LCD_SleepFunction_Init(void);
void    EDT_LCD_Sleep_Function(void);
void    EDT_LCD_Sleep_Time(uint16_t value);
void    EDT_Sleep_SuspendLCDSleepTask(void);
void    EDT_Sleep_ResumeLCDSleepTask(void);
bool    EDT_Sleep_GetDetected(void); 
void    EDT_Sleep_SetDetected(bool bl); 
/**
  * @brief  LCD color
  */
/*******************************************************
#define LCD_COLOR_BLUE          ((uint32_t)0xFF0000FF)
#define LCD_COLOR_GREEN         ((uint32_t)0xFF00FF00)
#define LCD_COLOR_RED           ((uint32_t)0xFFFF0000)
#define LCD_COLOR_CYAN          ((uint32_t)0xFF00FFFF)
#define LCD_COLOR_MAGENTA       ((uint32_t)0xFFFF00FF)
#define LCD_COLOR_YELLOW        ((uint32_t)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN    ((uint32_t)0xFF80FF80)
#define LCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE      ((uint32_t)0xFF000080)
#define LCD_COLOR_DARKGREEN     ((uint32_t)0xFF008000)
#define LCD_COLOR_DARKRED       ((uint32_t)0xFF800000)
#define LCD_COLOR_DARKCYAN      ((uint32_t)0xFF008080)
#define LCD_COLOR_DARKMAGENTA   ((uint32_t)0xFF800080)
#define LCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
#define LCD_COLOR_WHITE         ((uint32_t)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY     ((uint32_t)0xFFD3D3D3)
#define LCD_COLOR_GRAY          ((uint32_t)0xFF808080)
#define LCD_COLOR_DARKGRAY      ((uint32_t)0xFF404040)
#define LCD_COLOR_BLACK         ((uint32_t)0xFF000000)
#define LCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
#define LCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT   ((uint32_t)0xFF000000)
******************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __EDT_BSP_LCD_H */

 /*******(C) COPYRIGHT Emerging Display Technologies Corp. **** END OF FILE ***/
