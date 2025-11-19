#ifndef CEREBRAL55_H
#define CEREBRAL55_H

#include "main.h" // change to your MCU series HAL header if needed
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  CEREBRAL55_STATE_UNKNOWN = 0,
  CEREBRAL55_STATE_STANDBY, // $@
  CEREBRAL55_STATE_RUN,     // $^
  CEREBRAL55_STATE_SHUT     // $%
} Cerebral55_State_t;

typedef struct {
  float V_in;
  float I_in;
  float P_in;
  float Temp;
  float V_out;
  float I_out;
  float P_out;
  float V_set;
  float I_set;
  int   RampBit;
  float RampRate;
} Cerebral55_OperatingData_t;

#define C55_BUF_SIZE 1024

typedef struct {
  // public
  Cerebral55_State_t state;
  Cerebral55_OperatingData_t data;

  // internal (do not touch)
  uint8_t buf[C55_BUF_SIZE];
  uint16_t idx;
  uint8_t prev;
  uint8_t in_msg; // 0 = waiting, 1 = collecting
} Cerebral55_Handle_t;

// Initialize handle (zeroes internal fields). Call once before use.
void Cerebral55_Init(Cerebral55_Handle_t *h);

// Feed one received byte into parser.
// Returns true when a full message (state or run) was completed and parsed.
bool Cerebral55_ProcessByte(Cerebral55_Handle_t *h, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif // CEREBRAL55_H
