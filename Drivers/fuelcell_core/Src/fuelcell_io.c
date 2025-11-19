/*
 * fuelcell_io.c
 *
 *  Created on: Oct 29, 2025
 *      Author: User
 */


#include "fuelcell_io.h"

inline void fc_io_start_indicator(fc_state_t state)
{
    FC_WRITE(FC_START_IND_PORT, FC_START_IND_PIN, state);
}

inline void fc_io_start_indicator_toggle()
{
    HAL_GPIO_TogglePin(FC_START_IND_PORT, FC_START_IND_PIN);
}

inline void fc_io_stand_indicator(fc_state_t state)
{
    FC_WRITE(FC_STAND_IND_PORT, FC_STAND_IND_PIN, state);
}

inline void fc_io_stand_indicator_toggle()
{
    HAL_GPIO_TogglePin(FC_STAND_IND_PORT, FC_STAND_IND_PIN);
}

inline void fc_io_emer_indicator(fc_state_t state)
{
    FC_WRITE(FC_EMER_IND_PORT, FC_EMER_IND_PIN, state);

}

inline void fc_io_system(fc_state_t state)
{
    FC_WRITE(FC_SYSTEM_1_PORT, FC_SYSTEM_1_PIN, state);
    FC_WRITE(FC_SYSTEM_2_PORT, FC_SYSTEM_2_PIN, state);
}

inline void fc_io_can_filter(fc_state_t state)
{
//    FC_WRITE(FC_CAN_FILTER_PORT, FC_CAN_FILTER_PIN, state);
}
