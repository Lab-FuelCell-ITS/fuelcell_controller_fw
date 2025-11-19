/*
 * utils.h
 *
 *  Created on: 29 Okt 2025
 *      Author: User
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "fuelcell.h"


#ifdef FUELCELL_DEBUG
  #define DEBUG_PRINT(label, msg)  CDC_Print(label, msg)
#else
  #define DEBUG_PRINT(label, msg)  ((void)0)
#endif


typedef union {
	float value;      // the float itself
	uint8_t bytes[4];   // raw byte representation
} FloatPack_t;

void CDC_Print(const char *label, const char *msg);

#endif /* INC_UTILS_H_ */
