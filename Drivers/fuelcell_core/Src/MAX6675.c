#include "MAX6675.h"

/* Init handle and register SPI callback */
HAL_StatusTypeDef MAX6675_Init(MAX6675_Handle_t *h,
                               SPI_HandleTypeDef *hspi,
                               GPIO_TypeDef *cs_port,
                               uint16_t cs_pin)
{
    if (!h || !hspi || !cs_port) return HAL_ERROR;

    h->hspi    = hspi;
    h->cs_port = cs_port;
    h->cs_pin  = cs_pin;

    h->temp = 0;
    h->status = 0;
    // CS idle high
    HAL_GPIO_WritePin(cs_port, cs_pin, GPIO_PIN_SET);

    return HAL_OK;
}

/* Blocking read */
HAL_StatusTypeDef MAX6675_ReadBlocking(MAX6675_Handle_t *h, uint32_t timeout_ms)
{
    uint8_t buf[2];
    float temp;

    HAL_GPIO_WritePin(h->cs_port, h->cs_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef st = HAL_SPI_Receive(h->hspi, buf, 1, timeout_ms);
    HAL_GPIO_WritePin(h->cs_port, h->cs_pin, GPIO_PIN_SET);

    if (st != HAL_OK) return st;


    h->status = (((buf[0] | (buf[1] << 8)) >> 2) & 0x0001); // State of Connecting
    temp = ((((buf[0] | buf[1] << 8))) >> 3); // Temperature Data Extraction
    temp *= 0.25;
    h->temp = temp;

    return HAL_OK;
}




