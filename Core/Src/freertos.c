/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fuelcell.h"
#include "MAX6675.h"
#include "cerebral55.h"
#include "pressure_sensor.h"

#include "spi.h"
extern CAN_HandleTypeDef hcan2;
extern CAN_HandleTypeDef hcan1;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for FSMTask */
osThreadId_t FSMTaskHandle;
const osThreadAttr_t FSMTask_attributes = {
  .name = "FSMTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sensorTask */
osThreadId_t sensorTaskHandle;
const osThreadAttr_t sensorTask_attributes = {
  .name = "sensorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for commTask */
osThreadId_t commTaskHandle;
const osThreadAttr_t commTask_attributes = {
  .name = "commTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void fsmtask(void *argument);
void sensortask(void *argument);
void commtask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of FSMTask */
  FSMTaskHandle = osThreadNew(fsmtask, NULL, &FSMTask_attributes);

  /* creation of sensorTask */
  sensorTaskHandle = osThreadNew(sensortask, NULL, &sensorTask_attributes);

  /* creation of commTask */
  commTaskHandle = osThreadNew(commtask, NULL, &commTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_fsmtask */
/**
 * @brief  Function implementing the FSMTask thread.
 * @param  argument: Not used
 * @retval None
 */

/* USER CODE END Header_fsmtask */
void fsmtask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN fsmtask */

	/* Infinite loop */
	for (;;) {
		fc_fsm_update();
		osDelay(500);
	}
  /* USER CODE END fsmtask */
}

/* USER CODE BEGIN Header_sensortask */
/**
 * @brief Function implementing the sensorTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_sensortask */
void sensortask(void *argument)
{
  /* USER CODE BEGIN sensortask */
	/* Infinite loop */


	for (;;) {
		fc_sensor_ReaBlocking();
		fc_update();
		osDelay(200);

	}
  /* USER CODE END sensortask */
}

/* USER CODE BEGIN Header_commtask */
/**
* @brief Function implementing the commTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_commtask */
void commtask(void *argument)
{
  /* USER CODE BEGIN commtask */
  /* Infinite loop */


  for(;;)
  {
	fc_can_gcan_transmit();
    osDelay(100);
  }
  /* USER CODE END commtask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

