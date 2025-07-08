/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include <stdbool.h>
  #include "cmsis_os.h"
  #include "se21159_sys_conf.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void hw_init ( void );
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EE_WC_Pin GPIO_PIN_5
#define EE_WC_GPIO_Port GPIOB
#define FS_OV_Current_Pin GPIO_PIN_10
#define FS_OV_Current_GPIO_Port GPIOG
#define SD_CS_Pin GPIO_PIN_15
#define SD_CS_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_6
#define SPI_CS_GPIO_Port GPIOB
#define POE_FB_Pin GPIO_PIN_11
#define POE_FB_GPIO_Port GPIOC
#define ETH_INT_Pin GPIO_PIN_12
#define ETH_INT_GPIO_Port GPIOC
#define LCD_STBYB_Pin GPIO_PIN_5
#define LCD_STBYB_GPIO_Port GPIOE
#define RS232_F_ON_Pin GPIO_PIN_4
#define RS232_F_ON_GPIO_Port GPIOE
#define CAN_STB_Pin GPIO_PIN_3
#define CAN_STB_GPIO_Port GPIOE
#define EXT_RST_Pin GPIO_PIN_2
#define EXT_RST_GPIO_Port GPIOD
#define LED_RTC_TS_Pin GPIO_PIN_13
#define LED_RTC_TS_GPIO_Port GPIOC
#define I2C_GPIO1_Pin GPIO_PIN_8
#define I2C_GPIO1_GPIO_Port GPIOI
#define LCD_BL_PWM_Pin GPIO_PIN_6
#define LCD_BL_PWM_GPIO_Port GPIOE
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOC
#define ETH_MCO_Pin GPIO_PIN_9
#define ETH_MCO_GPIO_Port GPIOC
#define LIN_CS_WAKE_Pin GPIO_PIN_11
#define LIN_CS_WAKE_GPIO_Port GPIOI
#define LIN_FAULT_Pin GPIO_PIN_3
#define LIN_FAULT_GPIO_Port GPIOG
#define EXT_INT_Pin GPIO_PIN_2
#define EXT_INT_GPIO_Port GPIOG
#define RS232_INV_Pin GPIO_PIN_0
#define RS232_INV_GPIO_Port GPIOC
#define RS232_F_OFF_Pin GPIO_PIN_10
#define RS232_F_OFF_GPIO_Port GPIOF
#define LCD_POWER_RESET_Pin GPIO_PIN_12
#define LCD_POWER_RESET_GPIO_Port GPIOB
#define FS_PW_SW_Pin GPIO_PIN_4
#define FS_PW_SW_GPIO_Port GPIOA
#define LCD_BL_CTRL_Pin GPIO_PIN_14
#define LCD_BL_CTRL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
