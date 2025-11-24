/*
 * fuelcell_fsm.c
 *
 *  Created on: 29 Okt 2025
 *      Author: User
 */

#include "fuelcell_fsm.h"

fsm_t fsm;

uint32_t get_time_ms(void) {
	return HAL_GetTick(); /* dummy time source */
}

static bool eval_eq(int a, int b) {
	return (a == b);
}

extern FullCell_t fuelcell;
extern FullCell_t fuelcell_dummy;

uint8_t display_start;

int fc_state_flag = 0;
int stop_flag = 0;
int timeout_flag = 0;
int prestart_flag = 0;

extern uint32_t g_last_can_rx_time;

void fc_set_state(FullCell_t *fuelcell, fsm_t *fsm) {
	fuelcell->state = fsm->current_state;
}

void fc_set_enable_command(FullCell_t *fuelcell, uint8_t value) {
	fuelcell->enable_command = value;
}

uint8_t fc_get_enable_command(FullCell_t *fuelcell) {
	return fuelcell->enable_command;
}
fcmode_t fc_check_mode(FullCell_t *fuelcell) {
	return fuelcell->mode;
}

/* -----------------------------
 * STATE_INIT
 * ----------------------------- */
static void on_enter_init(void) {
	/* Perform initialization */
	DEBUG_PRINT("FSM", "IN INIT");

	fc_io_start_indicator(OFF);
	fc_io_stand_indicator(ON);
	fc_io_emer_indicator(OFF);

	fc_io_system(OFF);
	fc_io_can_filter(OFF); // menerima can kembali

	timeout_flag = 0;
	fc_state_flag = 0;
//	fuelcell.enable_command = 0;
//	fc_set_enable_command(&fuelcell, 0);

#ifdef DUMMY
	fc_set_enable_command(&fuelcell_dummy, 0);
#else
    fc_set_enable_command(&fuelcell, 0);
#endif
}

static void on_update_init(void) {
	/* Check system status, wait for start command */
	DEBUG_PRINT("FSM", "RUN INIT");

#ifdef DUMMY
//	fc_state_flag = fuelcell.enable_command;
	fc_state_flag = fc_get_enable_command(&fuelcell_dummy);
	// this function return 0 and 1 integer format, always return 0 on independent mode
//	if (fuelcell.mode == INTEGRATED) {
//		if (fc_can_check_timeout())
//			timeout_flag = 1;
//	}


//	if (fc_check_mode(&fuelcell_dummy) == INTEGRATED) {
//		if (fc_can_check_timeout())
//			timeout_flag = 1;
	}
#else
	//	fc_state_flag = fuelcell.enable_command;
		fc_state_flag = fc_get_enable_command(&fuelcell);
		// this function return 0 and 1 integer format, always return 0 on independent mode
	//	if (fuelcell.mode == INTEGRATED) {
	//		if (fc_can_check_timeout())
	//			timeout_flag = 1;
	//	}

//		if (fc_check_mode(&fuelcell) == INTEGRATED) {
//			if (fc_can_check_timeout()) timeout_flag = 1;
//		}
#endif
}

static void on_exit_init(void) {
	/* Cleanup before leaving INIT */
	DEBUG_PRINT("FSM", "OUT INIT");

	fc_io_stand_indicator(OFF);
	fc_io_can_filter(ON);
}

/* -----------------------------
 * STATE_STARTING
 * ----------------------------- */
static void on_enter_prestarting(void) {
	/* Begin startup sequence */
	DEBUG_PRINT("FSM", "IN PRESTARTING");

	fc_io_start_indicator(OFF);
	fc_io_stand_indicator(OFF);
	fc_io_emer_indicator(OFF);

	fc_io_system(OFF);
	fc_io_can_filter(ON);

#ifdef DUMMY
	prestart_flag = fc_sensor_precheck(&fuelcell_dummy);
#else
	prestart_flag = fc_sensor_precheck(&fuelcell);
#endif
}

static void on_update_prestarting(void) {
	/* Monitor startup progress */
	DEBUG_PRINT("FSM", "RUN PRESTARTING");
//	fc_io_stand_indicator_toggle();

}

static void on_exit_prestarting(void) {
	/* Cleanup after startup */
	DEBUG_PRINT("FSM", "OUT PRESTARTING");
	prestart_flag = 0; // reset prestart flag
	fc_io_can_filter(OFF);
}

/* -----------------------------
 * STATE_STARTING
 * ----------------------------- */
static void on_enter_starting(void) {
	/* Begin startup sequence */
	DEBUG_PRINT("FSM", "IN STARTING");

	fc_io_start_indicator(OFF);
	fc_io_stand_indicator(OFF);
	fc_io_emer_indicator(OFF);

	fc_io_system(ON);
	fc_io_can_filter(ON);
}

static void on_update_starting(void) {
	/* Monitor startup progress */
	DEBUG_PRINT("FSM", "RUN STARTING");
	fc_io_stand_indicator_toggle();
}

static void on_exit_starting(void) {
	/* Cleanup after startup */
	DEBUG_PRINT("FSM", "OUT STARTING");

	fc_io_can_filter(OFF);
}

/* -----------------------------
 * STATE_ACTIVE
 * ----------------------------- */
static void on_enter_active(void) {
	/* Enable main operation */
	DEBUG_PRINT("FSM", "IN ACTIVE");

	fc_io_start_indicator(ON);
	fc_io_stand_indicator(OFF);
	fc_io_emer_indicator(OFF);

	fc_io_system(ON);
	fc_io_can_filter(OFF);

}

static void on_update_active(void) {
	/* Perform active tasks */
	DEBUG_PRINT("FSM", "RUN ACTIVE");

#ifdef DUMMY
//	fc_state_flag = fuelcell.enable_command;
	fc_state_flag = fc_get_enable_command(&fuelcell_dummy);
	// this function return 0 and 1 integer format, always return 0 on independent mode
//	if (fuelcell.mode == INTEGRATED) {
//		if (fc_can_check_timeout())
//			timeout_flag = 1;
//	}
	if (fc_check_mode(&fuelcell_dummy) == INTEGRATED) {
		if (fc_can_check_timeout())
			timeout_flag = 1;
	}
#else
	//	fc_state_flag = fuelcell.enable_command;
		fc_state_flag = fc_get_enable_command(&fuelcell);
		// this function return 0 and 1 integer format, always return 0 on independent mode
	//	if (fuelcell.mode == INTEGRATED) {
	//		if (fc_can_check_timeout())
	//			timeout_flag = 1;
	//	}
		if (fc_check_mode(&fuelcell) == INTEGRATED) {
			if (fc_can_check_timeout())
				timeout_flag = 1;
		}
#endif



}

static void on_exit_active(void) {
	/* Prepare to stop or timeout */
	DEBUG_PRINT("FSM", "OUT ACTIVE");
	fc_io_start_indicator(OFF);
}

/* -----------------------------
 * STATE_PRE_TIMEOUT
 * ----------------------------- */
static void on_enter_pretimeout(void) {
	/* Shut down peripherals, save logs */
	DEBUG_PRINT("FSM", "IN PRETIMEOUT");

	fc_io_start_indicator(OFF);
	fc_io_stand_indicator(OFF);
	fc_io_emer_indicator(OFF);

	fc_io_system(ON);
	fc_io_can_filter(ON);

}

static void on_update_pretimeout(void) {
	/* Wait for restart command or sleep */
	DEBUG_PRINT("FSM", "RUN STOP");
	fc_io_stand_indicator_toggle();
}

static void on_exit_pretimeout(void) {
	/* Prepare to transition back to INIT */
	DEBUG_PRINT("FSM", "OUT STOP");

	fc_io_start_indicator(OFF);
	fc_io_can_filter(OFF);
}

/* -----------------------------
 * STATE_TIMEOUT
 * ----------------------------- */
static void on_enter_timeout(void) {
	/* Trigger timeout handling */
	DEBUG_PRINT("FSM", "IN TIMEOUT");

	fc_io_start_indicator(OFF);
	fc_io_stand_indicator(OFF);
	fc_io_emer_indicator(ON);

	fc_io_system(OFF);
	fc_io_can_filter(OFF);

}

static void on_update_timeout(void) {
	/* Monitor for reset or recovery */
	DEBUG_PRINT("FSM", "RUN TIMEOUT");
	HAL_GPIO_TogglePin(EX_0_GPIO_Port, EX_0_Pin);
	//timeout_flag = fuelcell.enable_command;
#ifdef DUMMY
	timeout_flag = fc_get_enable_command(&fuelcell_dummy);
#else
	timeout_flag = fc_get_enable_command(&fuelcell);
#endif

}

static void on_exit_timeout(void) {
	/* Reset timeout counters */
	DEBUG_PRINT("FSM", "OUT TIMEOUT");

	timeout_flag = 0;
	fc_state_flag = 0;
	//	fuelcell.enable_command = 0;
#ifdef DUMMY
	fc_set_enable_command(&fuelcell_dummy, 0);
#else
    fc_set_enable_command(&fuelcell, 0);
#endif

	HAL_GPIO_WritePin(EX_0_GPIO_Port, EX_0_Pin, GPIO_PIN_SET);

	fc_io_emer_indicator(OFF);

}

/* -----------------------------
 * STATE_STOP
 * ----------------------------- */
static void on_enter_stop(void) {
	/* Shut down peripherals, save logs */
	DEBUG_PRINT("FSM", "IN STOP");

	fc_io_start_indicator(OFF);
	fc_io_stand_indicator(OFF);
	fc_io_emer_indicator(OFF);

	fc_io_system(OFF);
	fc_io_can_filter(ON);

}

static void on_update_stop(void) {
	/* Wait for restart command or sleep */
	DEBUG_PRINT("FSM", "RUN STOP");
	fc_io_stand_indicator_toggle();
}

static void on_exit_stop(void) {
	/* Prepare to transition back to INIT */
	DEBUG_PRINT("FSM", "OUT STOP");

	fc_io_start_indicator(OFF);
	fc_io_can_filter(OFF);

	timeout_flag = 0;
	fc_state_flag = 0;
	//	fuelcell.enable_command = 0;
#ifdef DUMMY
	fc_set_enable_command(&fuelcell_dummy, 0);
#else
    fc_set_enable_command(&fuelcell, 0);
#endif


}

void fc_fsm_init(void) {

	fsm_init(&fsm, STATE_INIT, get_time_ms);

	fsm_trans_t *t = NULL;

	/* STATE INIT transisi ke STATE_STARTING */
	fsm_add_transition(&fsm, &t, STATE_INIT, STATE_PRE_STARTING);
	fsm_add_event_cmp(&fsm, t, &fc_state_flag, 1, eval_eq);

	fsm_add_transition(&fsm, &t, STATE_PRE_STARTING, STATE_STARTING);
	fsm_add_event_cmp(&fsm, t, &prestart_flag, 1, eval_eq);

	fsm_add_transition(&fsm, &t, STATE_PRE_STARTING, STATE_INIT);
	fsm_add_event_cmp(&fsm, t, &prestart_flag, 0, eval_eq);

	fsm_add_transition(&fsm, &t, STATE_STARTING, STATE_ACTIVE);
	fsm_add_event_timeout(&fsm, t, 10000);

	fsm_add_transition(&fsm, &t, STATE_ACTIVE, STATE_STOP);
	fsm_add_event_cmp(&fsm, t, &fc_state_flag, 0, eval_eq);

	fsm_add_transition(&fsm, &t, STATE_ACTIVE, STATE_PRE_TIMEOUT);
	fsm_add_event_cmp(&fsm, t, &timeout_flag, 1, eval_eq);

	fsm_add_transition(&fsm, &t, STATE_PRE_TIMEOUT, STATE_TIMEOUT);
	fsm_add_event_timeout(&fsm, t, 10000);

	fsm_add_transition(&fsm, &t, STATE_TIMEOUT, STATE_STOP);
	fsm_add_event_cmp(&fsm, t, &timeout_flag, 0, eval_eq);

	fsm_add_transition(&fsm, &t, STATE_STOP, STATE_INIT);
	fsm_add_event_timeout(&fsm, t, 10000);

	fsm_register_state_actions(&fsm, STATE_INIT, on_enter_init, on_update_init,
			on_exit_init);

	fsm_register_state_actions(&fsm, STATE_PRE_STARTING, on_enter_prestarting,
			on_update_prestarting, on_exit_prestarting);

	fsm_register_state_actions(&fsm, STATE_STARTING, on_enter_starting,
			on_update_starting, on_exit_starting);

	fsm_register_state_actions(&fsm, STATE_ACTIVE, on_enter_active,
			on_update_active, on_exit_active);

	fsm_register_state_actions(&fsm, STATE_PRE_TIMEOUT, on_enter_pretimeout,
			on_update_pretimeout, on_exit_pretimeout);

	fsm_register_state_actions(&fsm, STATE_TIMEOUT, on_enter_timeout,
			on_update_timeout, on_exit_timeout);

	fsm_register_state_actions(&fsm, STATE_STOP, on_enter_stop, on_update_stop,
			on_exit_stop);

}

inline void fc_fsm_update(void) {
	fsm_run(&fsm);
//	fuelcell.state = fsm.current_state;
#ifdef DUMMY
    fc_set_state(&fuelcell_dummy, &fsm);
#else
	fc_set_state(&fuelcell, &fsm);
#endif

}
