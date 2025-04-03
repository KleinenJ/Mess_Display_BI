/**
  ******************************************************************************
  * @file    et070016dba.h
  * @author  EDT Embedded Application Team
  * @version V1.1.0
  * @date    31-Aug-2023
  * @brief   This file contains all the constants parameters for the ET070016ba
  *          LCD component.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2023 EDT</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
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
#ifndef __ET070016DBA_H
#define __ET070016DBA_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/  
  /** 
  * @brief  ET070016DBA Size  
  */     
    #define  TFT_WIDTH            ((uint16_t)1024)          /* LCD PIXEL WIDTH            */
    #define  TFT_HEIGHT           ((uint16_t)600)           /* LCD PIXEL HEIGHT           */
  /*   Time  Typ.
    DCLK                             51.2    Mhz
    Horizontal display area(thd)     1024    DCLK
    HSYNC period time(th)            1344    DCLK
    HSYNC blanking (thb+thfp)        320     DCLK

    Vertical display area(tvd)       600     DCLK
    VSYNC period time(tv)            635     DCLK
    VSYNC blanking (tvb+tvfp)        35      DCLK
  */
/** 
  * @brief  ET070016DBA Innolux Timing  
  */    
/** ET070016dba HSYNC Timing **/   
    #define  TFT_HSYNC            ((uint16_t)20)  /* Horizontal synchronization */
    #define  TFT_HBP              ((uint16_t)160)   /* Horizontal back porch      */
    #define  TFT_HFP              ((uint16_t)140)   /* Horizontal front porch     */
/** ET070016dba VSYNC Timing **/   
    #define  TFT_VSYNC            ((uint16_t)2)    /* Vertical synchronization   */
    #define  TFT_VBP              ((uint16_t)25)   /* Vertical back porch        */
    #define  TFT_VFP              ((uint16_t)10)   /* Vertical front porch       */
  
#if defined(STM32H750xx)
/**
*    STM32H750 LCD clock configuration              
*    DCLK       = 25MHZ / TFT_PLL3M * TFT_PLL3N / TFT_PLL3R 
*ex: DCLK       = 25MHz  / 5 * 164 / 16 =51.25Mhz 
*    FLM        = DCLK / ((TFT_WIDTH+TFT_HSYNC+TFT_HBP+TFT_HFP) * ( TFT_HEIGHT+ TFT_VSYNC+ TFT_VBP+TFT_VFP))
*ex: FLM        = 51.25Mz / ((1024+20+160+140) * (600+2+23+10))  = 60.05Hz(16.6ms)
**/ 
    #define  TFT_PLL3M           5
    #define  TFT_PLL3N           164
    #define  TFT_PLL3P           2
    #define  TFT_PLL3Q           2   
    #define  TFT_PLL3R           16
#else
/**   
     STM32F7XX  LCD clock configuration                          
*    PLLSAI_VCO = TFT_SAIN = 288 Mhz                  
*    DCLK       = TFT_SAIN / TFT_SAIR / TFT_DIVR           
*ex: DCLK       = 288MHz   / 3        / 2        = 48Mhz 
*    FLM        = DCLK / ((TFT_WIDTH+TFT_HSYNC+TFT_HBP+TFT_HFP) * ( TFT_HEIGHT+ TFT_VSYNC+ TFT_VBP+TFT_VFP))
*ex: FLM        = 48Mz / ((1024+20+160+140) * (600+2+23+10))  = 56.24Hz (17.7ms)
**/ 
    #define  TFT_SAIN            ((uint16_t)48 * 6 ) /*This value must be a multiple of 48 and max. is 432*/  
    #define  TFT_SAIP            6                   /*Its range must be 2~7 */      
    #define  TFT_SAIR            3                   /*Its range must be 2~7 */      
    #define  TFT_DIVR            RCC_PLLSAIDIVR_2    /*Its allowed value is 2,4,8,16*/  
    #define  TFT_SAIQ            2                   /*Its allowed value is 2,4,8,16*/  
    #define  TFT_DIVQ            1                   /*Its allowed value is 2,4,8,16*/  
#endif
   
/**LCD Polarity**/
    #define  HS_POLARITY    LTDC_HSPOLARITY_AL      /* horizontal synchronization polarity */
    #define  VS_POLARITY    LTDC_VSPOLARITY_AL      /* vertical synchronization polarity */
    #define  DE_POLARITY    LTDC_DEPOLARITY_AL      /* not data enable polarity */
    #define  PC_POLARITY    LTDC_PCPOLARITY_IIPC     /* pixel clock polarity */

#ifdef __cplusplus
}
#endif

#endif /* __ET070016DBA_H */

/************************ (C) COPYRIGHT EDT *****END OF FILE****/
