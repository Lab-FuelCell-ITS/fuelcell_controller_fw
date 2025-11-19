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
#define FLASH_CS_Pin GPIO_PIN_1
#define FLASH_CS_GPIO_Port GPIOC
#define FLASH_MISO_Pin GPIO_PIN_2
#define FLASH_MISO_GPIO_Port GPIOC
#define FLASH_MOSI_Pin GPIO_PIN_3
#define FLASH_MOSI_GPIO_Port GPIOC
#define EX_0_Pin GPIO_PIN_4
#define EX_0_GPIO_Port GPIOA
#define EX_1_Pin GPIO_PIN_5
#define EX_1_GPIO_Port GPIOA
#define EX_2_Pin GPIO_PIN_6
#define EX_2_GPIO_Port GPIOA
#define EX_3_Pin GPIO_PIN_7
#define EX_3_GPIO_Port GPIOA
#define EX_4_Pin GPIO_PIN_4
#define EX_4_GPIO_Port GPIOC
#define EX_5_Pin GPIO_PIN_5
#define EX_5_GPIO_Port GPIOC
#define EX_6_Pin GPIO_PIN_0
#define EX_6_GPIO_Port GPIOB
#define EX_7_Pin GPIO_PIN_1
#define EX_7_GPIO_Port GPIOB
#define EX_8_Pin GPIO_PIN_2
#define EX_8_GPIO_Port GPIOB
#define EX_9_Pin GPIO_PIN_7
#define EX_9_GPIO_Port GPIOE
#define Relay_0_Pin GPIO_PIN_8
#define Relay_0_GPIO_Port GPIOE
#define Relay_1_Pin GPIO_PIN_9
#define Relay_1_GPIO_Port GPIOE
#define Relay_2_Pin GPIO_PIN_10
#define Relay_2_GPIO_Port GPIOE
#define Relay_3_Pin GPIO_PIN_11
#define Relay_3_GPIO_Port GPIOE
#define Relay_4_Pin GPIO_PIN_12
#define Relay_4_GPIO_Port GPIOE
#define Relay_5_Pin GPIO_PIN_13
#define Relay_5_GPIO_Port GPIOE
#define Relay_6_Pin GPIO_PIN_14
#define Relay_6_GPIO_Port GPIOE
#define Relay_7_Pin GPIO_PIN_15
#define Relay_7_GPIO_Port GPIOE
#define FLASH_SCK_Pin GPIO_PIN_10
#define FLASH_SCK_GPIO_Port GPIOB
#define L_CAN_RX_Pin GPIO_PIN_12
#define L_CAN_RX_GPIO_Port GPIOB
#define L_CAN_TX_Pin GPIO_PIN_13
#define L_CAN_TX_GPIO_Port GPIOB
#define NEXTION_TX_Pin GPIO_PIN_9
#define NEXTION_TX_GPIO_Port GPIOA
#define NEXTION_RX_Pin GPIO_PIN_10
#define NEXTION_RX_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define G_CAN_RX_Pin GPIO_PIN_0
#define G_CAN_RX_GPIO_Port GPIOD
#define G_CAN_TX_Pin GPIO_PIN_1
#define G_CAN_TX_GPIO_Port GPIOD
#define MAX_0_CS_Pin GPIO_PIN_6
#define MAX_0_CS_GPIO_Port GPIOD
#define MAX_1_CS_Pin GPIO_PIN_7
#define MAX_1_CS_GPIO_Port GPIOD
#define ADS_SCL_Pin GPIO_PIN_6
#define ADS_SCL_GPIO_Port GPIOB
#define ADS_SDA_Pin GPIO_PIN_7
#define ADS_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
