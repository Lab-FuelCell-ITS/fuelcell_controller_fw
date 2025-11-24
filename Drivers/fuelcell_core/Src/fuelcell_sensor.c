/*
 * fuelcell_sensor.c
 *
 *  Created on: Nov 16, 2025
 *      Author: User
 */
#include "fuelcell_sensor.h"

#define CEREBRAL_BUFFER 512

uint8_t buffer1[CEREBRAL_BUFFER];
uint8_t buffer2[CEREBRAL_BUFFER];

Cerebral55_Handle_t cerebral_1;
Cerebral55_Handle_t cerebral_2;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern SPI_HandleTypeDef hspi3;

MAX6675_Handle_t max1;
MAX6675_Handle_t max2;

extern I2C_HandleTypeDef hi2c1;
PressureSensor_HandleTypeDef pressure;

static void cerebral_1_Callback(UART_HandleTypeDef *huart, uint16_t Size);
static void cerebral_2_Callback(UART_HandleTypeDef *huart, uint16_t Size);

void fc_sensor_init(void) {
	MAX6675_Init(&max1, &hspi3, MAX_1_CS_GPIO_Port, MAX_1_CS_Pin);
	MAX6675_Init(&max2, &hspi3, MAX_0_CS_GPIO_Port, MAX_0_CS_Pin);

	PressureSensor_Init(&pressure, &hi2c1);

	Cerebral55_Init(&cerebral_1);
	HAL_UART_RegisterRxEventCallback(&huart2, cerebral_1_Callback);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, buffer1, CEREBRAL_BUFFER);

	Cerebral55_Init(&cerebral_2);
	HAL_UART_RegisterRxEventCallback(&huart3, cerebral_2_Callback);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, buffer2, CEREBRAL_BUFFER);
}

int fc_sensor_precheck(FullCell_t *fuelcell) {
	if ((fuelcell->temp[0] < UNDERTEMP) || (fuelcell->temp[0] > OVERTEMP)) {
		fuelcell->fault_over_temperature = 1;

		return 0;

	} else {
		fuelcell->fault_over_temperature = 0;
	}

	if (fuelcell->tank_pressure < UNDERPRESSURE) {
		return 0;
	} else {

	}

	return 1;

}
void fc_sensor_ReaBlocking(void) {
	MAX6675_ReadBlocking(&max1, 500);
	MAX6675_ReadBlocking(&max2, 500);
	PressureSensor_ReadBlocking(&pressure, 500);
}

static void cerebral_1_Callback(UART_HandleTypeDef *huart, uint16_t Size) {

	for (uint16_t i = 0; i < Size; ++i) {
		Cerebral55_ProcessByte(&cerebral_1, buffer1[i]);
	}
}

static void cerebral_2_Callback(UART_HandleTypeDef *huart, uint16_t Size) {

	for (uint16_t i = 0; i < Size; ++i) {
		Cerebral55_ProcessByte(&cerebral_2, buffer2[i]);
	}
}
