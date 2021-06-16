/*
 * delay.h
 *
 * Created: 6/4/2021 1:52:17 PM
 *  Author: justb
 */ 


#ifndef DELAY_H_
#define DELAY_H_


#include "tcb.h"
#include <atomic.h>


#ifdef __cplusplus
extern "c" {
	#endif

	static volatile uint16_t __delay_ms_past = 0;

	static void __delay_timer_callback() {
		__delay_ms_past++;
	}

	static void delay_ms(uint16_t ms) {
		TIMER_0_set_period_callback(__delay_timer_callback);
		TIMER_0_clear();
		TIMER_0_set_period(20000);
		TIMER_0_start();
		__delay_ms_past = 0;
		while(__delay_ms_past < ms) {}
		TIMER_0_stop();
		return;
	}


	#ifdef __cplusplus
}
#endif


#endif /* DELAY_H_ */