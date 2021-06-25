/**
 * \file
 * \brief Driver initialization manager
 * 
 * Initializes all the various drivers that are used. Will
 * run through and call every init function. To add another
 * driver first write the driver code then add the init function
 * in the apporpriate spot with the other drivers init functions.
 * 
 * \author Ben Brenkman
 * \date 24 June 2021
 */

#ifndef DRIVER_INIT_H__
#define DRIVER_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initializes all the drivers.
 * 
 * Function called when sysetm_init() is called.
 * \return \c void
 */
void driver_init(void);

#ifdef __cplusplus
}
#endif

#endif // DRIVER_INIT_H__