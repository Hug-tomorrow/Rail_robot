/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "can.h"
#include "usart.h"
#include "gpio.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_can.h"
#include "bsp_canopen.h"
#include "bsp_ps2.h"
#include "bsp_oled.h"
#include "bsp_uart.h"  
#include "bsp_crc32.h"
#include "CAN_TXRX.h"
#include "cmsis_os.h"
#include "stdio.h"//vsprintf
#include "string.h"//strlen
#include "stdint.h"//unit8
#include "stdarg.h"//ÈÎÒâ´®¿Úprintf
#include "struct_typedef.h"
#include "user_lib.h"
#include "bsp_buzzer.h"
#include "bsp_gpio.h"
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PS2_CS_Pin GPIO_PIN_2
#define PS2_CS_GPIO_Port GPIOE
#define PS2_CLK_Pin GPIO_PIN_3
#define PS2_CLK_GPIO_Port GPIOE
#define Proximity_Switch_Pin GPIO_PIN_10
#define Proximity_Switch_GPIO_Port GPIOD
#define Proximity_Switch_EXTI_IRQn EXTI15_10_IRQn
#define Buzzer_Pin GPIO_PIN_11
#define Buzzer_GPIO_Port GPIOD
#define LED_GREE_Pin GPIO_PIN_13
#define LED_GREE_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOD
#define LED_BLUE_Pin GPIO_PIN_15
#define LED_BLUE_GPIO_Port GPIOD
#define OLED_DC_Pin GPIO_PIN_4
#define OLED_DC_GPIO_Port GPIOD
#define OLED_RES_Pin GPIO_PIN_5
#define OLED_RES_GPIO_Port GPIOD
#define OLED_SDA_Pin GPIO_PIN_6
#define OLED_SDA_GPIO_Port GPIOD
#define OLED_SCL_Pin GPIO_PIN_7
#define OLED_SCL_GPIO_Port GPIOD
#define PS2_DI_Pin GPIO_PIN_0
#define PS2_DI_GPIO_Port GPIOE
#define PS2_DO_Pin GPIO_PIN_1
#define PS2_DO_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
