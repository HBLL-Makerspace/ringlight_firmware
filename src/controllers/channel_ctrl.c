#include<stdint.h>

#include<controllers/channel_ctrl.h>

#define MAX_NUM_CHANNELS 3

static Channel channels[MAX_NUM_CHANNELS] = {
    {
        .r = 0,
        .g = 0,
        .b = 0,
        .intensity = 0
    },
    {
        .r = 0,
        .g = 0,
        .b = 0,
        .intensity = 0
    },
    {
        .r = 0,
        .g = 0,
        .b = 0,
        .intensity = 0
    }
};

void chn_ctrl_init() {
    channels[0].r = 0xff;
    channels[0].b = 0xff;
    channels[0].g = 0xff;
    channels[0].intensity = 0xff;
}

void chn_ctrl_set_channel_color(uint8_t channel, uint32_t color) {
    if (channel < MAX_NUM_CHANNELS) {
        channels[channel].intensity = color && 0xff;
        color = color >> 8;
        channels[channel].b = color && 0xff;
        color = color >> 8;
        channels[channel].g = color && 0xff;
        color = color >> 8;
        channels[channel].r = color && 0xff;
        chn_ctrl_update_leds();
    }
}

void chn_ctrl_set_channel_color_rgb(uint8_t channel, uint8_t r, uint8_t g, uint8_t b) {
    if (channel < MAX_NUM_CHANNELS) {
        channels[channel].r = r;
        channels[channel].g = g;
        channels[channel].b = b;
        chn_ctrl_update_leds();
    }
}

void chn_ctrl_set_channel_color_hsv(uint8_t channel, uint8_t h, uint8_t s, uint8_t v) {}

uint32_t chn_ctrl_get_channel_color(uint8_t channel) {
    if (channel < MAX_NUM_CHANNELS) {
        return (uint32_t)channels[channel].r << 24 | (uint32_t)channels[channel].g << 16 | (uint32_t)channels[channel].b << 8 | (uint32_t)channels[channel].intensity;
    }
    return 0;
}

uint8_t chn_ctrl_get_channel_color_r(uint8_t channel) {
    if (channel < MAX_NUM_CHANNELS) {
        return channels[channel].r;
    }
    return 0;
}
uint8_t chn_ctrl_get_channel_color_g(uint8_t channel) {
    if (channel < MAX_NUM_CHANNELS) {
        return channels[channel].g;
    }
    return 0;
}
uint8_t chn_ctrl_get_channel_color_b(uint8_t channel) {
    if (channel < MAX_NUM_CHANNELS) {
        return channels[channel].b;
    }
    return 0;
}

uint8_t chn_ctrl_get_channel_color_h(uint8_t channel) {
    return 0;
}
uint8_t chn_ctrl_get_channel_color_s(uint8_t channel) {return 0;}
uint8_t chn_ctrl_get_channel_color_v(uint8_t channel) {return 0;}

void chn_ctrl_set_channel_intesity(uint8_t channel, uint8_t intensity) {
    if (channel < MAX_NUM_CHANNELS) {
        channels[channel].intensity = intensity;
        chn_ctrl_update_leds();
    }
}

void chn_ctrl_update_leds() {
    for (uint8_t i = 0; i < MAX_NUM_CHANNELS; i++) {

    }
}
