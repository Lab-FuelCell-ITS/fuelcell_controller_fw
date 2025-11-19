/*
 * fuelcell.c
 *
 *  Created on: Nov 17, 2025
 *      Author: User
 */

#include "fuelcell.h"

FullCell_t fuelcell;

extern Cerebral55_Handle_t cerebral_1;
extern Cerebral55_Handle_t cerebral_2;

extern MAX6675_Handle_t max1;
extern MAX6675_Handle_t max2;
extern PressureSensor_HandleTypeDef pressure;
extern FloatPack_t lcan_data;

void fc_init() {
// Always check for a null pointer
	// Set the initial state
	fuelcell.state = STATE_INIT;
	// Zero all sensor values
	fuelcell.enable_command = 0;     // Default ke Disable
	fuelcell.voltage = 0.0f;
	fuelcell.current = 0.0f;
	fuelcell.pressure[0] = 0.0f;
	fuelcell.pressure[1] = 0.0f;
	fuelcell.temp[0] = 0.0f;
	fuelcell.temp[1] = 0.0f;
	fuelcell.tank_pressure = 0.0f;

	fc_can_init();
	fc_sensor_init();
	fc_fsm_init();
}

#define UNDERVOLT	(60.0f)   // 5%
#define OVERVOLT	(74.0f)   // 4.5 bar
#define OVERCURRENT	(160.0f)  // 20 C
#define UNDERTEMP	(20.0f)  // 60 C
#define OVERTEMP	(60.0f)

void fc_update() {
	fuelcell.temp[0] = max1.temp;
	fuelcell.temp[1] = max2.temp;

	fuelcell.pressure[0] = pressure.bar[0];
	fuelcell.pressure[1] = pressure.bar[1];

	fuelcell.voltage = (cerebral_1.data.V_out + cerebral_2.data.V_out) / 2.0f;
	fuelcell.current = (cerebral_1.data.I_out + cerebral_2.data.I_out) / 2.0f;

	fuelcell.tank_pressure = lcan_data.value;

	// check fault
//	if ((fuelcell.voltage < UNDERVOLT) || (fuelcell.voltage > OVERVOLT))
//		fuelcell.fault_over_voltage = 1;
//	else
//		fuelcell.fault_over_voltage = 0;
//
//	if (fuelcell.current > OVERCURRENT)
//		fuelcell.fault_over_current = 1;
//	else
//		fuelcell.fault_over_current = 0;
//
//
//	if ((fuelcell.temp[0] < UNDERTEMP) || (fuelcell.temp[0] > OVERTEMP)) fuelcell.fault_over_temperature = 1;
//	else fuelcell.fault_over_temperature = 0;


	if((fuelcell.state != STATE_ACTIVE)) fuelcell.operating_status = 1;
	else fuelcell.operating_status = 0;
}

