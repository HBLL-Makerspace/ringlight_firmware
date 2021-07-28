#ifndef CHANNEL_CONTROLLER_H__
#define CHANNEL_CONTROLLER_H__

#include<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Channel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t intensity;
} Channel;

void chn_ctrl_init();

void chn_ctrl_set_channel_color(uint8_t channel, uint32_t color);
void chn_ctrl_set_channel_color_rgb(uint8_t channel, uint8_t r, uint8_t g, uint8_t b);
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