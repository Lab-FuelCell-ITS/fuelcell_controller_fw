#ifndef MAX6675_H
#define MAX6675_H

#ifdef __cplusplus
extern "C" {
#endif

/* change this include to match your MCU family if needed */
#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * MAX6675 handle structure.
 *
 * - hspi:      pointer to HAL SPI handle (e.g. &hspi3)
 * - cs_port:   GPIO port for chip-select
 * - cs_pin:    GPIO pin for chip-select
 * - tcf:       thermocouple connection flag (true = connected / ok)
 * - raw:       raw 2-byte buffer (MSB first)
 * - busy:      true while DMA transfer in progress (optional use)
 * - data_ready:true when new data arrived (set by ISR or driver)
 * - temp:      last parsed temperature in Celsius
 * - status:    last raw status bits (user-defined use)
 *
 * Notes:
 * - The library code assumes CS is active LOW and idle HIGH.
 * - If you use DMA, assert CS before starting DMA and deassert in Rx callback.
 */
typedef struct MAX6675_Handle_t {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t           cs_pin;

    float          temp;       /* last parsed temperature (C) */
    uint8_t        status;     /* user/status field (e.g. fault bit) */

} MAX6675_Handle_t;


/* Initialize a MAX6675 handle.
 * - h must point to a valid MAX6675_Handle_t reserved by caller.
 * - hspi is the HAL SPI handle (e.g. &hspi3).
 * - cs_port/cs_pin specify the chip-select GPIO.
 *
 * This function initializes the handle fields and leaves CS high (idle).
 * It does NOT start a read. If you want DMA-read support, register or call
 * the RX complete callback as appropriate in your project.
 */
HAL_StatusTypeDef MAX6675_Init(MAX6675_Handle_t *h,
                               SPI_HandleTypeDef *hspi,
                               GPIO_TypeDef *cs_port,
                               uint16_t cs_pin);




/* Blocking read: perform a blocking SPI read of the sensor (uses HAL_SPI_Receive).
 * - timeout_ms is passed to HAL_SPI_Receive.
 * - On success the latest raw bytes are stored to h->raw, h->temp and h->status are set.
 * Returns HAL_OK on success.
 */
HAL_StatusTypeDef MAX6675_ReadBlocking(MAX6675_Handle_t *h, uint32_t timeout_ms);



#ifdef __cplusplus
}
#endif

#endif /* MAX6675_H */
