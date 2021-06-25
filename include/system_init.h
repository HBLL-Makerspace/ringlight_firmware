/**
 * \file
 * \brief Initialize the entire system.
 * 
 * Initialize the whole system, set all the pins to be grounded and initialize the drivers.
 * 
 * \author Ben Brenkman
 * \date 24 June 2021
 */

#ifndef SYSTEM_INIT_H__
#define SYSTEM_INIT_H__

#include <clock_config.h>
#include <port.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialize the system.
 * 
 * Sets all the pins to there default levels. So we know exaclty what
 * state the mcu is in before we do anything else. Then initialize all
 * the drivers and enable interrupts.
 * 
 * \return \c void
 */
void system_init(void);

/**
 * \brief Initialize the mcu pins.
 * 
 * Sets all the pins to be grounded. This way we know exactly what state
 * the mcu is in before we continue with the initialization.
 * 
 * \return \c void
 */
void system_mcu_init(void);

#ifdef __cplusplus
}
#endif

#endif // __SYSTEM_INIT_H__