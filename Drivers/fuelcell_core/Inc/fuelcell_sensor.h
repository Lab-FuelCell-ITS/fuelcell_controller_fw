/*
 * fuelcell_sensor.h
 *
 *  Created on: Nov 16, 2025
 *      Author: User
 */

#ifndef FUELCELL_CORE_INC_FUELCELL_SENSOR_H_
#define FUELCELL_CORE_INC_FUELCELL_SENSOR_H_

#include "fuelcell.h"
#include "cerebral55.h"
#include "MAX6675.h"
#include "pressure_sensor.h"

void fc_sensor_init(void);
void fc_sensor_ReaBlocking(void);

#endif /* FUELCELL_CORE_INC_FUELCELL_SENSOR_H_ */
