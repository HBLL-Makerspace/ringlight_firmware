/**
 * \file
 * \brief Real Time Clock controller
 * 
 * Driver for the RTC/PIT timer/counters on the attiny 0-series. Currently only
 * a heartbeat functionality is implmented. This is a way for the programmer and 
 * user to see that the devide is operational. Relies on a function called 
 * INDICATOR_set_level(bool level) to be implmeneted in the port.h file.
 * The heartbeat will turn the LED(rather the pin) on and off at a specific interval
 * mimicing a heartbeat, indicating that the cpu is running. If there is no heartbeat
 * it is a good indicator that something is going wrong, either interrupts are disabled
 * or the CPU is in a stuck state.
 * 
 * \author Ben Brenkman
 * \date 24 June 2021
 */

#ifndef RTC_H__
#define RTC_H__

#include <compiler.h>
#include <utils_assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initializes the Real Time Clock driver.
 * \return \c void
 */
void RTC_init(void);

/**
 * \brief Enables the heartbeat.
 * \return \c void
 */
void RTC_enable_heartbeat(void);

/**
 * \brief Disables the heartbeat.
 * \return \c void
 */
void RTC_disable_heartbeat(void);

#ifdef __cplusplus
}
#endif

#endif