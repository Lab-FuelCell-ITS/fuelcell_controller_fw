/*
 * fuelcell_init.h
 *
 *  Created on: Nov 7, 2025
 *      Author: User
 */

#ifndef FUELCELL_CORE_INC_FUELCELL_CAN_H_
#define FUELCELL_CORE_INC_FUELCELL_CAN_H_

#include "fuelcell.h"


HAL_StatusTypeDef fc_can_init(void);


void fc_can_gcan_transmit(void);
void fc_can_lcan_transmit(void);

void fc_can_gcan_callback(void);
void fc_can_lcan_callback(void);

int fc_can_check_timeout();
#endif /* FUELCELL_CORE_INC_FUELCELL_CAN_H_ */
