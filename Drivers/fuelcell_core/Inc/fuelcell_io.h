/*
 * fuelcell_io.h
 *
 *  Created on: Oct 29, 2025
 *      Author: User
 */

#ifndef INC_FUELCELL_IO_H_
#define INC_FUELCELL_IO_H_

#include "fuelcell.h"

/* Logical state abstraction */
typedef enum {
    OFF = 0u,
    ON  = 1u
} fc_state_t;


/* === Pin mappings (adjust to your board) === */
#define FC_START_IND_PORT     Relay_1_GPIO_Port
#define FC_START_IND_PIN      Relay_1_Pin

#define FC_STAND_IND_PORT     Relay_0_GPIO_Port
#define FC_STAND_IND_PIN      Relay_0_Pin

#define FC_EMER_IND_PORT      Relay_2_GPIO_Port
#define FC_EMER_IND_PIN       Relay_2_Pin

#define FC_SYSTEM_1_PORT        Relay_3_GPIO_Port
#define FC_SYSTEM_1_PIN         Relay_3_Pin

#define FC_SYSTEM_2_PORT        Relay_4_GPIO_Port
#define FC_SYSTEM_2_PIN         Relay_4_Pin

#define FC_CAN_FILTER_PORT    GPIOB
#define FC_CAN_FILTER_PIN     GPIO_PIN_6

/* Helper macro to ON/OFF to HAL level */
#define FC_WRITE(port, pin, s) HAL_GPIO_WritePin((port), (pin), ((s) == ON ? GPIO_PIN_SET : GPIO_PIN_RESET))



/* Public API */
void fc_io_start_indicator(fc_state_t state);
void fc_io_start_indicator_toggle();

void fc_io_stand_indicator(fc_state_t state);
void fc_io_stand_indicator_toggle();

void fc_io_emer_indicator(fc_state_t state);
void fc_io_system(fc_state_t state);
void fc_io_can_filter(fc_state_t state);




#endif /* INC_FUELCELL_IO_H_ */
