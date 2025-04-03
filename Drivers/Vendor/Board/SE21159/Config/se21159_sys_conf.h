/**
  ******************************************************************************  
  * File Name          : se21159_sys_conf.h
  * @author            : EDT Embedded Application Team
  * @brief             : This file contains some configurations required for the
  *                       smart embedded display board.
  * @brief             : EDT <https://smartembeddeddisplay.com/>
  ******************************************************************************
  * @attention  
  * COPYRIGHT(c) 2023 Emerging Display Technologies Corp.
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
#ifndef __SE21159_SYS_CONF_H
#define __SE21159_SYS_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif    

/******************* DISPLAY Private define************************/   
#define EVK070016
#define USE_BL_PWM   
//#define USE_LCD_SleepFunction
/******************* CTP Private define****************************/   
#define ILI2511
/******************* External Interface Private define*************/      
#define USE_RS232
#define USE_RS485
#define USE_EX_I2C 
#define USE_SPI     
#define USE_CAN 
#define USE_GPIO
#define USE_dp83848
#define USE_TCPIP
#define USE_EEROM_I2C

 /** Board support package version: 6.1.0 (tpa file) ****************/
#define __EDT_AT_VERSION_MAIN  (0x06UL) /*!< [31:24] main version */
#define __EDT_AT_VERSION_SUB1  (0x01UL) /*!< [23:16] sub1 version */
#define __EDT_AT_VERSION_SUB2  (0x00UL) /*!< [15:8] ?sub2 version */
#define __EDT_AT_VERSION ((__EDT_AT_VERSION_MAIN << 24)\
                             |(__EDT_AT_VERSION_SUB1 << 16)\
                             |(__EDT_AT_VERSION_SUB2 << 8 ))
inline uint32_t EDT_GetTPAVersion(void){ return __EDT_AT_VERSION;}

/******************************************************************************/
/******************************************************************************/
/* Do not modify following #include statements, unless you know how to do it. */
/******************************************************************************/
/******************************************************************************/
/*----------------------Includes for USE_STM32H7 -----------------------------*/

#include "stm32h7xx_hal.h"
#if defined(STM32H750xx)
#include "edt_bsp_sdram.h"
#endif 
   
#if defined(EVK070016)
   #include "lcd/et070016dba.h"
   #include "edt_bsp_lcd.h"
#endif  
   
#if defined(USE_BL_PWM)
  #include "edt_bsp_backlight.h"
#endif  /* End of USE_BL_PWM */
   
#if defined(ILI2511)
 #include "edt_bsp_ctp.h"
#endif
   
#if defined(USE_RS232)||defined(USE_RS485)||defined(USE_UART)
  #include "edt_bsp_uart.h"
#endif


#if defined(USE_CAN)
 #include "edt_bsp_can.h"
#endif

#if defined(USE_SPI)     
  #include "edt_bsp_spi.h"
#endif 

#if defined(USE_EX_I2C)
  #include "edt_bsp_i2c.h"
#endif

#if defined(USE_GPIO)
  #include "edt_bsp_io.h"
#endif

#if defined(USE_EEROM_I2C)
  #include "edt_h7xx_eeprom_i2c.h"
#endif

/***********************************************************************/

/* ------------------------ user define -----------------------------------*/
#define LCD_BL_PWM_TIMER_CLK()  __HAL_RCC_TIM15_CLK_ENABLE()
#define LCD_BL_PWM_TIMER        TIM15
#define LCD_BL_PWM_CHANNEL      TIM_CHANNEL_2

#define RS232_BAUD              115200
#define RS232_INSTANCE          USART2
#define RS232_IRQn              USART2_IRQn
#define RS232_IRQHandler        USART2_IRQHandler
#define RS232_IRQ_SetPriority   9

#define RS485_BAUD              115200
#define RS485_INSTANCE          UART7
#define RS485_IRQn              UART7_IRQn
#define RS485_IRQHandler        UART7_IRQHandler
#define RS485_IRQ_SetPriority   8

#define SPIx_INSTANCE           SPI1
#define SPIx_BAUDRATE         SPI_BAUDRATEPRESCALER_128  // 1.5625Mhz
//#define SPIx_BAUDRATE         SPI_BAUDRATEPRESCALER_64   // 3.125Mhz
//#define SPIx_BAUDRATE         SPI_BAUDRATEPRESCALER_32;  // 6.25M
//#define SPIx_BAUDRATE         SPI_BAUDRATEPRESCALER_16;  // 12.5MHz
//#define SPIx_BAUDRATE         SPI_BAUDRATEPRESCALER_8;   // 25MHz

#define I2Cx_INSTANCE           I2C4
  /*I2C Speed Mode Standard Mode*/
//#define I2Cx_TIMING             ((uint32_t)0x10C0ECFF)  //  100KHz
  /*I2C Speed Mode Fast Mode */
//  #define I2Cx_TIMING         ((uint32_t)0x1040ECFF)  //  100KHz
//  #define I2Cx_TIMING         ((uint32_t)0x0090E5FF)  //  200KHz
  #define I2Cx_TIMING         ((uint32_t)0x009034B6)  //  400KHz

#define FDCANx_INSTANCE         FDCAN1
#define CAN_BAUDRATE500K
/* FDCAN_BAUDRATE option
  'CAN_BAUDRATE1M
  'CAN_BAUDRATE500K
  'CAN_BAUDRATE250K
  'CAN_BAUDRATE125K
*/
#define CANBUS_RX_ID_MODE   0
/* CANBUS_RX_ID_MODE
  '0 : FDCAN_FILTER_RANGE; Range filter from FilterID1 to FilterID2
  '1 : FDCAN_FILTER_DUAL; Dual ID filter for FilterID1 or FilterID2
  '2 : FDCAN_FILTER_MASK; Classic filter: FilterID1 = filter, FilterID2 = mask
  '3 : FDCAN_FILTER_RANGE_NO_EIDM; Range filter from FilterID1 to FilterID2,
                                  EIDM mask not applied
*/
#if ( CANBUS_RX_ID_MODE == 0 )
 #define FDCANx_ID1            0       /*Range filter from FilterID1 */
 #define FDCANx_ID2            0X7FF       /*Range filter to   FilterID2 */
#elif ( CANBUS_RX_ID_MODE == 1 )
 #define FDCANx_ID1            0x321       /*Dual ID filter for FilterID1*/
 #define FDCANx_ID2            0X798       /*Dual ID filter for FilterID2*/
#elif ( CANBUS_RX_ID_MODE == 2 )
 #define FDCANx_ID1            0x321       /*Classic filter: FilterID1=filter*/
 #define FDCANx_ID2            0X123       /*Classic filter: FilterID2=mask  */
#elif ( CANBUS_RX_ID_MODE == 3 )
 #define FDCANx_ID1            0x321       /*Range filter from FilterID1 ,
                                                  EIDM mask not applied*/
 #define FDCANx_ID2            0x321       /*Range filter to   FilterID2 ,
                                                  EIDM mask not applied*/
#else
 #define FDCANx_ID1               0        // CANBUS_RX_ID_MODE 6 : for test
 #define FDCANx_ID2           0X7FF
#endif

/* ethernet Local static IP config*/
#define EDT_IP_ADDR0        192
#define EDT_IP_ADDR1        168
#define EDT_IP_ADDR2        21
#define EDT_IP_ADDR3        21
/*NETMASK*/
#define EDT_NETMASK_ADDR0   255
#define EDT_NETMASK_ADDR1   255
#define EDT_NETMASK_ADDR2   255
#define EDT_NETMASK_ADDR3   0
/*Gateway Address*/
#define EDT_GW_ADDR0        192
#define EDT_GW_ADDR1        168
#define EDT_GW_ADDR2        21
#define EDT_GW_ADDR3        247
      
#define EDT_TCPIP_PORT      7      
      
#define EDT_ETH_MAC_ADDR0    (0x00UL)
#define EDT_ETH_MAC_ADDR1    (0x02UL)
#define EDT_ETH_MAC_ADDR2    (0x01UL)
#define EDT_ETH_MAC_ADDR3    (0x02UL)
#define EDT_ETH_MAC_ADDR4    (0x01UL)
#define EDT_ETH_MAC_ADDR5    (0x02UL)
      
      
/* ethernet connect remote tcp server IP config*/
#define EDT_REMOTE_SERVER_IP_ADDR0        192
#define EDT_REMOTE_SERVER_IP_ADDR1        168
#define EDT_REMOTE_SERVER_IP_ADDR2        1
#define EDT_REMOTE_SERVER_IP_ADDR3        2
      
#define EDT_REMOTE_SERVER_PORT           8            
      
#ifdef __cplusplus
}
#endif

#endif /* __SE21159_SYS_CONF_H */

/************************ (C) COPYRIGHT EDT *****END OF FILE****/
