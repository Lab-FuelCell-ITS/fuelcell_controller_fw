
#ifndef INC_FUELCELL_H_
#define INC_FUELCELL_H_

#define FUELCELL_DEBUG 1 // 0 or 1

#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "fsm.h"

typedef enum {
	STATE_INIT = 0,
	STATE_STARTING,
	STATE_ACTIVE,
	STATE_PRE_TIMEOUT,
	STATE_TIMEOUT,
	STATE_STOP
} state_t;

typedef struct {
	// Current operating state
	state_t state;

	uint8_t enable_command;
	// Electrical measurements
	float voltage;       // Stack output voltage (Volts)
	float current;       // Stack output current (Amperes)

	// Pressure measurements
	float pressure[2];      // Internal stack pressure (e.g., Bar or kPa)
	float tank_pressure; // Hydrogen tank pressure (e.g., Bar or kPa)
	float temp[2];

	uint8_t operating_status;
	uint8_t fault_over_current;    // 0 = no error, 1 = error
	uint8_t fault_over_voltage;    // 0 = no error, 1 = error
	uint8_t fault_over_temperature; // 0 = no error, 1 = error

} FullCell_t;

/* recursive call */
#include "fuelcell_can.h"
#include "fuelcell_fsm.h"
#include "fuelcell_io.h"
#include "fuelcell_utils.h"
#include "fuelcell_sensor.h"

void fc_init();
void fc_update();

#endif
