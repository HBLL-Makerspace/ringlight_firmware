/**
 * \file
 * \date 27 July 2021
 * \author Ben Brenkman
 * 
 * \brief Controller for the channels on the ring light.
 * 
 * There are 3 channels on the ring light. Each channel consists of 24 ws2812b rgb leds and 12 white JB3030 (20mW) leds.
 * This mix of different LEDs with different circuits makes it a bit confusing to control. The WS2812b leds are all on the 
 * same ciruit and the white leds are broken up into 6 groups of 6 leds each. This controller abstracts all the different
 * leds into 3 channels, with red, green and blue colors. The controller will handle turning on the LEDs, whether white
 * or color, to get the desired color.
 * 
 * \see https://forum.arduino.cc/t/logarithmic-scaling-for-led-dimming/144333/11
 * \see https://en.wikipedia.org/wiki/Stevens%27s_power_law#:~:text=Stevens's%20power%20law%20is%20an,sensation%20created%20by%20the%20stimulus.&text=The%20theory%20is%20named%20after,Stevens%20(1906%E2%80%931973).
 */

#ifndef CHANNEL_CONTROLLER_H__
#define CHANNEL_CONTROLLER_H__

#include<stdint.h>
#include<stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Channel color struct
 * 
 * The color for each channel is defined by an R, G and B value. There is also
 * an intensity value that is a multiplier for the final color. So the intensity
 * or brightness can be controlled seperatlly.
 */
typedef struct Channel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
    uint8_t intensity;
    void (*pwm_set_duty)(uint8_t);
    void (*pwm_enable)();
    void (*pwm_disable)();
    void (*chn_set_level)(const bool);
} Channel;

/**
 * \brief Initialize the channel controller.
 * 
 * Sets the channel colors when initialized. Gets old values from 
 * eeprom and sets them to the appropriate channel.
 */
void chn_ctrl_init();

/**
 * \brief Sets channel color.
 * 
 * \param [in] color The color in hex using the following format 0xRRGGBBII
 * \param [in] channel The channel to set the color on
 */
void chn_ctrl_set_channel_color(uint8_t channel, uint32_t color);

/**
 * \brief Sets the channel color.
 * 
 * Sets the channel color to with the defined rgb values.
 * \param [in] channel The channel to set the color on
 * \param [in] r Red value of color.
 * \param [in] g Green value of color.
 * \param [in] b Blue value of color.
 */
void chn_ctrl_set_channel_color_rgb(uint8_t channel, uint8_t r, uint8_t g, uint8_t b);

/**
 * \brief Sets the channel color with white.
 * 
 * Sets the channel color to with the defined rgb values.
 * \param [in] channel The channel to set the color on
 * \param [in] r Red value of color.
 * \param [in] g Green value of color.
 * \param [in] b Blue value of color.
 * \param [in] w White value of color.
 */
void chn_ctrl_set_channel_color_rgbw(uint8_t channel, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/**
 * \brief Sets the channel white color.
 * 
 * Sets the channel color to with the defined rgb values.
 * \param [in] channel The channel to set the color on
 * \param [in] w White value of color.
 */
void chn_ctrl_set_channel_color_w(uint8_t channel, uint8_t w);

/**
 * \brief Sets the channel color.
 * 
 * Sets the channel color to with the defined hsv values.
 * \param [in] channel The channel to set the color on
 * \param [in] h Hue of color.
 * \param [in] s Saturation of color.
 * \param [in] v Value of color.
 * \warning Not implemented
 */
void chn_ctrl_set_channel_color_hsv(uint8_t channel, uint8_t h, uint8_t s, uint8_t v);

uint32_t chn_ctrl_get_channel_color(uint8_t channel);
uint8_t chn_ctrl_get_channel_color_r(uint8_t channel);
uint8_t chn_ctrl_get_channel_color_g(uint8_t channel);
uint8_t chn_ctrl_get_channel_color_b(uint8_t channel);
uint8_t chn_ctrl_get_channel_color_h(uint8_t channel);
uint8_t chn_ctrl_get_channel_color_s(uint8_t channel);
uint8_t chn_ctrl_get_channel_color_v(uint8_t channel);

void chn_ctrl_set_channel_intesity(uint8_t channel, uint8_t intensity);
void chn_ctrl_update_leds();

#ifdef __cplusplus
}
#endif

#endif