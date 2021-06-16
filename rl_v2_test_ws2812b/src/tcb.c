/*
 * tcb.c
 *
 * Created: 6/4/2021 1:53:22 PM
 *  Author: justb
 */ 

#include <tcb.h>
#include <driver_init.h>

static void(*TIMER_0_callback)() = NULL;

/**
 * \brief Initialize tcb interface
 *
 * \return Initialization status.
 */
int8_t TIMER_0_init()
{

	TCB0.CCMP = 0x0; /* Compare or Capture: 0x0 */

	TCB0.CNT = 0x0; /* Count: 0x0 */

	TCB0.CTRLB = 0 << TCB_ASYNC_bp /* Asynchronous Enable: disabled */
		 | 0 << TCB_CCMPEN_bp /* Pin Output Enable: disabled */
		 | 0 << TCB_CCMPINIT_bp /* Pin Initial State: disabled */
		 | 0 << TCB_CNTMODE_INT_gc; /* Periodic Interrupt */

	TCB0.DBGCTRL = 0 << TCB_DBGRUN_bp; /* Debug Run: disabled */

	TCB0.EVCTRL = 0 << TCB_CAPTEI_bp /* Event Input Enable: disabled */
		 | 0 << TCB_EDGE_bp /* Event Edge: disabled */
		 | 0 << TCB_FILTER_bp; /* Input Capture Noise Cancellation Filter: disabled */

	TCB0.INTCTRL = 1 << TCB_CAPT_bp /* Capture or Timeout: disabled */;

	TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc /* CLK_PER (No Prescaling) */
		 | 0 << TCB_ENABLE_bp /* Enable: disabled */
		 | 0 << TCB_RUNSTDBY_bp /* Run Standby: disabled */
		 | 0 << TCB_SYNCUPD_bp; /* Synchronize Update: disabled */

	return 0;
}

int8_t TIMER_0_set_period(uint16_t count) {
	TCB0.CCMP = count;
	return 0;
}

int8_t TIMER_0_start() {
	TCB0.CTRLA = TCB0.CTRLA | (1 << TCB_ENABLE_bp);
	return 0;
}

int8_t TIMER_0_stop() {
	TCB0.CTRLA = TCB0.CTRLA & !(1 << TCB_ENABLE_bp);
	return 0;
}

int8_t TIMER_0_clear() {
	TCB0.CNT = 0x0;
	return 0;
}

int8_t TIMER_0_set_period_callback(void(*callback)()) {
	TIMER_0_callback = callback;
	return 0;
}

ISR(TCB0_INT_vect) {
	TCB0.INTFLAGS = 1;
	if (TIMER_0_callback != NULL)
		TIMER_0_callback();
}
