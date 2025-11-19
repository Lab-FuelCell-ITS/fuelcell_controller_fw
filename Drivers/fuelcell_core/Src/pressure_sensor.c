/*
 * pressure_sensor.c
 *
 *  Created on: Nov 17, 2025
 *      Author: User
 */

#include "pressure_sensor.h"

#define ADS1115_ADDRESS 0x48
const float voltageConv = 6.114f / 32768.0f;

void PressureSensor_Init(PressureSensor_HandleTypeDef *hps, I2C_HandleTypeDef *hi2c)
{
    hps->hi2c = hi2c;
    hps->reading = 0;

    hps->voltage[0] = 0.0f;
    hps->voltage[1] = 0.0f;

    hps->bar[0] = 0.0f;
    hps->bar[1] = 0.0f;
}

float PressureSensor_ConvertToBar(float voltage)
{
    float bar = (voltage - 0.5f) / 4.0f;

    if (bar < 0.0f) bar = 0.0f;
    if (bar > 1.0f) bar = 1.0f;

    return bar;
}

void PressureSensor_ReadBlocking(PressureSensor_HandleTypeDef *hps, uint32_t timeout)
{
    unsigned char ADSwrite[6];
    I2C_HandleTypeDef *hi2c = hps->hi2c;

    for (int i = 0; i < 2; i++) {

        ADSwrite[0] = 0x01;
        ADSwrite[1] = (i == 0) ? 0xE1 : 0xF1;
        ADSwrite[2] = 0x83;

        HAL_I2C_Master_Transmit(hi2c, ADS1115_ADDRESS << 1, ADSwrite, 3, timeout);

        ADSwrite[0] = 0x00;
        HAL_I2C_Master_Transmit(hi2c, ADS1115_ADDRESS << 1, ADSwrite, 1, timeout);

        HAL_Delay(20);

        HAL_I2C_Master_Receive(hi2c, ADS1115_ADDRESS << 1, ADSwrite, 2, timeout);

        hps->reading = (int16_t)((ADSwrite[0] << 8) | ADSwrite[1]);

        if (hps->reading < 0)
            hps->reading = 0;

        hps->voltage[i] = hps->reading * voltageConv;

        // save bar result directly into handle
        hps->bar[i] = PressureSensor_ConvertToBar(hps->voltage[i]);
    }
}

