/*
 * fuelcell.c
 *
 *  Created on: Nov 17, 2025
 *      Author: User
 */

#include "fuelcell.h"

#ifdef DUMMY
FullCell_t fuelcell_dummy;
#else
FullCell_t fuelcell;
#endif

extern Cerebral55_Handle_t cerebral_1;
extern Cerebral55_Handle_t cerebral_2;

extern MAX6675_Handle_t max1;
extern MAX6675_Handle_t max2;
extern PressureSensor_HandleTypeDef pressure;
extern FloatPack_t lcan_data;

void fc_check_fault(FullCell_t *fc) {
	// Voltage fault
	fc->fault_over_voltage = (fc->voltage < UNDERVOLT)
			|| (fc->voltage > OVERVOLT);

	// Overcurrent fault
	fc->fault_over_current = (fc->current > OVERCURRENT);

	// Temperature fault (temp[0])
	fc->fault_over_temperature = (fc->temp[0] < UNDERTEMP)
			|| (fc->temp[0] > OVERTEMP);

	// Operating status
	fc->operating_status = (fc->state != STATE_ACTIVE);
}

void fc_init() {
// Always check for a null pointer
	// Set the initial state
#ifdef DUMMY
	fuelcell_dummy.state = STATE_INIT;
		// Zero all sensor values
	fuelcell_dummy.enable_command = 0;     // Default ke Disable
	fuelcell_dummy.voltage = 0.0f;
	fuelcell_dummy.current = 0.0f;
	fuelcell_dummy.pressure[0] = 0.0f;
	fuelcell_dummy.pressure[1] = 0.0f;
	fuelcell_dummy.temp[0] = 0.0f;
	fuelcell_dummy.temp[1] = 0.0f;
	fuelcell_dummy.tank_pressure = 0.0f;
#else

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
#endif

	fc_can_init();
	fc_sensor_init();
	fc_fsm_init();

}

GPIO_PinState fc_gpio_mode_input;

void fc_update() {

#ifdef DUMMY
	fc_check_fault(&fuelcell_dummy);

	if(HAL_GPIO_ReadPin(EX_3_GPIO_Port, EX_3_Pin)) fuelcell_dummy.mode = INTEGRATED;
	else fuelcell_dummy.mode = STANDALONE;
#else
	fuelcell.temp[0] = max1.temp;
	fuelcell.temp[1] = max2.temp;

	fuelcell.pressure[0] = pressure.bar[0];
	fuelcell.pressure[1] = pressure.bar[1];

	fuelcell.voltage = (cerebral_1.data.V_out + cerebral_2.data.V_out) / 2.0f;
	fuelcell.current = (cerebral_1.data.I_out + cerebral_2.data.I_out) / 2.0f;

	fuelcell.tank_pressure = lcan_data.value;
	fc_check_fault(&fuelcell);

	if(HAL_GPIO_ReadPin(EX_3_GPIO_Port, EX_3_Pin)) fuelcell.mode = INTEGRATED;
	else fuelcell.mode = STANDALONE;
#endif


}

