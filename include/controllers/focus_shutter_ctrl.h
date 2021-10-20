/**
 * \file
 * \date 16 September 2021
 * \author Jack Damiano
 * 
 * \brief Controller for the focus and shutter on the cameras.
 * 
 * Each board has an output for the focus and shutter of the cameras.
 * This controller incorporates delays and duty cycles to facilitate
 * the focus/shutter process.
 * 
 */

#ifndef FOCUS_SHUTTER_CTRL_H
#define FOCUS_SHUTTER_CTRL_H

#include<stdint.h>
#include<stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


//function declarations go here
/**
 * \brief LED test
 * 
 * LED test
 */
void focus_shutter_led_test(uint8_t focus, uint8_t shutter);

#ifdef __cplusplus
}
#endif

#endif