/*
 * utils.c
 *
 *  Created on: 29 Okt 2025
 *      Author: User
 */


#include "fuelcell_utils.h"

char cdc_tx_buffer[64];

void CDC_Print(const char *label, const char *msg) {

	int len = snprintf(cdc_tx_buffer, sizeof(cdc_tx_buffer), "[%s] %s\r\n", label, msg);

	CDC_Transmit_FS((uint8_t*) cdc_tx_buffer, (uint16_t) len);
}
