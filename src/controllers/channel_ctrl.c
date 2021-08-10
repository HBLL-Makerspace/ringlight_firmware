#include<math.h>
#include<stdint.h>

#include<utils/utils.h>

#include<drivers/pwm.h>
#include<controllers/channel_ctrl.h>
#include<drivers/ws2812.h>

#define MAX_NUM_CHANNELS 3
#define MAX_BRIGHTNESS_INPUT 255
#define A_VAL 0.5
#define P_MAX 15.9687194227 // Pmax = Smax ^ a;

#define NUMPIXELS      12
#define LEDS_PER_GROUP 4
#define NUMBER_GROUPS 3
#define DEGREE_SPACING 43

/*
 * There is 18 times more light per level of the white LED's verses the ws2812b leds. 
 */

static Channel channels[MAX_NUM_CHANNELS] = {
    {
        .r = 0,
        .g = 0,
        .b = 0,
        .intensity = 0,
        .pwm_ctrl = PWM_set_duty_cycle_ch3
    },
    {
        .r = 0,
        .g = 0,
        .b = 0,
        .intensity = 0,
        .pwm_ctrl = PWM_set_duty_cycle_ch4
    },
    {
        .r = 0,
        .g = 0,
        .b = 0,
        .intensity = 0,
        .pwm_ctrl = PWM_set_duty_cycle_ch5
    }
};

void chn_ctrl_init() {
    PWM_enable();
	PWM_enable_ch3();
	PWM_enable_ch4();
	PWM_enable_ch5();

    WS2812_init(WS2812_GRB, NUMBER_GROUPS * LEDS_PER_GROUP, &PORTA.OUT, PIN6_bp);
    chn_ctrl_update_leds();
}

void chn_ctrl_set_channel_color(uint8_t channel, uint32_t color) {
    if (channel < MAX_NUM_CHANNELS) {
        channels[channel].r = color >> 24;
        channels[channel].g = color >> 16;
        channels[channel].b = color >> 8;
        channels[channel].intensity = color;
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
        chn_ctrl_update_leds(true);
    }
}

static uint8_t chn_ctrl_pwr_level(uint8_t power) {
    return (uint8_t)(pow(P_MAX * ((float)power/(float)255), 1/A_VAL) + 0.5);
}

static void chn_ctrl_update_chn(uint8_t channel) {
    if (channel < MAX_NUM_CHANNELS) {
        Channel chn = channels[channel];
        uint8_t r = chn.r;
        uint8_t g = chn.g;
        uint8_t b = chn.b;

        uint8_t white = minu8(r, g);
        white = minu8(g, b);

        if (white < 50) {
            white = 0;
        }

        r -= white;
        g -= white;
        b -= white;

        chn.pwm_ctrl(chn_ctrl_pwr_level(white >> 2));

        uint8_t start = LEDS_PER_GROUP * channel;
        for (uint8_t i = 0; i < LEDS_PER_GROUP; i++) {
            WS2812_set_pixel_color_RGB(start + i, r, g, b);
        }

        // start = LEDS_PER_GROUP * channel + (MAX_NUM_CHANNELS * LEDS`_PER_GROUP);
        // for (uint8_t i = 0; i < LEDS_PER_GROUP; i++) {
        //     WS2812_set_pixel_color_RGB(start + i, r, g, b);
        // }
    }
}

void chn_ctrl_update_leds() {
    for (uint8_t i = 0; i < MAX_NUM_CHANNELS; i++) {
        chn_ctrl_update_chn(i);
    }
    WS2812_show();
}
