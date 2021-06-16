/*
 * tcb.h
 *
 * Created: 6/4/2021 1:52:50 PM
 *  Author: justb
 */ 


#ifndef TCB_H_
#define TCB_H_

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
	#endif

	int8_t TIMER_0_init();
	int8_t TIMER_0_set_period(uint16_t count);
	int8_t TIMER_0_start();
	int8_t TIMER_0_stop();
	int8_t TIMER_0_clear();
	int8_t TIMER_0_set_period_callback(void(*callback)());

#ifdef __cplusplus
}
#endif


#endif /* TCB_H_ */