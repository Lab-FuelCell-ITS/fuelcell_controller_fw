/*
 * pressure_sensor.h
 *
 *  Created on: Nov 17, 2025
 *      Author: User
 */

#ifndef FUELCELL_CORE_INC_PRESSURE_SENSOR_H_
#define FUELCELL_CORE_INC_PRESSURE_SENSOR_H_

#include "fuelcell.h"
#include "i2c.h"


typedef struct {
    I2C_HandleTypeDef *hi2c;
    float voltage[2];
    float bar[2];       // store converted pressure
    int16_t reading;
} PressureSensor_HandleTypeDef;

void PressureSensor_Init(PressureSensor_HandleTypeDef *hps, I2C_HandleTypeDef *hi2c);
void PressureSensor_ReadBlocking(PressureSensor_HandleTypeDef *hps, uint32_t timeout);
float PressureSensor_ConvertToBar(float voltage);



#endif /* FUELCELL_CORE_INC_PRESSURE_SENSOR_H_ */
