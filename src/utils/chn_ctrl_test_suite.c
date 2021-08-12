#include<util/delay.h>

#include<drivers/ws2812.h>
#include<controllers/channel_ctrl.h>
#include<utils/chn_ctrl_test_suite.h>

#define LOOP_DELAY 5

static void chn_ctrl_ramp_red(uint8_t chn) {
    for(uint8_t i = 0; i < 255; i++) {
        chn_ctrl_set_channel_color_rgb(chn, i, 0, 0);
        _delay_ms(LOOP_DELAY);
    }
    chn_ctrl_set_channel_color_rgb(chn, 0, 0, 0);
}

static void chn_ctrl_ramp_green(uint8_t chn) {
    for(uint8_t i = 0; i < 255; i++) {
        chn_ctrl_set_channel_color_rgb(chn, 0, i, 0);
        _delay_ms(LOOP_DELAY);
    }
    chn_ctrl_set_channel_color_rgb(chn, 0, 0, 0);
}

static void chn_ctrl_ramp_blue(uint8_t chn) {
    for(uint8_t i = 0; i < 255; i++) {
        chn_ctrl_set_channel_color_rgb(chn, 0, 0, i);
        _delay_ms(LOOP_DELAY);
    }
    chn_ctrl_set_channel_color_rgb(chn, 0, 0, 0);
}

static void chn_ctrl_ramp_white(uint8_t chn) {
    for(uint8_t i = 0; i < 255; i++) {
        chn_ctrl_set_channel_color_rgb(chn, i, i, i);
        _delay_ms(LOOP_DELAY);
    }
    chn_ctrl_set_channel_color_rgb(chn, 0, 0, 0);
}

static void chn_ctrl_ramp_major_white(uint8_t chn) {
    for(uint8_t i = 0; i < 255; i++) {
        chn_ctrl_set_channel_color_w(chn, i);
        _delay_ms(LOOP_DELAY);
    }
    chn_ctrl_set_channel_color_w(chn, 0);
}

static uint32_t chn_ctrl_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return ((uint32_t)w) | ((uint32_t)r << 24) | ((uint32_t)g <<  16) | ((uint32_t)b >> b);
  }
  
  // Input a value 0 to 255 to get a color value.
  // The colors are a transition r - g - b - back to r.
  static uint32_t chn_ctrl_Wheel(uint8_t WheelPos) {
	  WheelPos = 255 - WheelPos;
	  if (WheelPos < 85) {
		  return chn_ctrl_RGBW(255 - WheelPos * 3, 0, WheelPos * 3, 0);
	  }
	  if (WheelPos < 170) {
		  WheelPos -= 85;
		  return chn_ctrl_RGBW(0, WheelPos * 3, 255 - WheelPos * 3, 0);
	  }
	  WheelPos -= 170;
	  return chn_ctrl_RGBW(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
  }

static void chn_ctrl_ramp_with_rainbow(uint8_t chn) {
    for (uint8_t i = 0; i < 255; i++) {
        chn_ctrl_set_channel_color(chn, WS2812_Wheel(i));
        _delay_ms(LOOP_DELAY * 16);
    }

    chn_ctrl_set_channel_color_rgb(chn, 0, 0, 0);
}

void chn_ctrl_test_suite_run_all() {
    chn_ctrl_set_channel_intesity(0, 255);
    chn_ctrl_set_channel_intesity(1, 255);
    chn_ctrl_set_channel_intesity(2, 255);

    chn_ctrl_ramp_red(0);
    chn_ctrl_ramp_green(0);
    chn_ctrl_ramp_blue(0);
    chn_ctrl_ramp_white(0);
    chn_ctrl_ramp_major_white(0);

    chn_ctrl_ramp_red(1);
    chn_ctrl_ramp_green(1);
    chn_ctrl_ramp_blue(1);
    chn_ctrl_ramp_white(1);
    chn_ctrl_ramp_major_white(1);

    chn_ctrl_ramp_red(2);
    chn_ctrl_ramp_green(2);
    chn_ctrl_ramp_blue(2);
    chn_ctrl_ramp_white(2);
    chn_ctrl_ramp_major_white(2);

    // chn_ctrl_ramp_with_rainbow(0);
    // chn_ctrl_ramp_with_rainbow(1);
    // chn_ctrl_ramp_with_rainbow(2);

    // chn_ctrl_set_channel_color(0, 0x768f23ff);
    // chn_ctrl_set_channel_color(1, 0x768f23ff);
    // chn_ctrl_set_channel_color(2, 0x768f23ff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0x7d9826ff);
    // chn_ctrl_set_channel_color(1, 0x7d9826ff);
    // chn_ctrl_set_channel_color(2, 0x7d9826ff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0x88a529ff);
    // chn_ctrl_set_channel_color(1, 0x88a529ff);
    // chn_ctrl_set_channel_color(2, 0x88a529ff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0x97b72dff);
    // chn_ctrl_set_channel_color(1, 0x97b72dff);
    // chn_ctrl_set_channel_color(2, 0x97b72dff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0xa4c731ff);
    // chn_ctrl_set_channel_color(1, 0xa4c731ff);
    // chn_ctrl_set_channel_color(2, 0xa4c731ff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0xadcf3dff);
    // chn_ctrl_set_channel_color(1, 0xadcf3dff);
    // chn_ctrl_set_channel_color(2, 0xadcf3dff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0xb5d450ff);
    // chn_ctrl_set_channel_color(1, 0xb5d450ff);
    // chn_ctrl_set_channel_color(2, 0xb5d450ff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0xbfd967ff);
    // chn_ctrl_set_channel_color(1, 0xbfd967ff);
    // chn_ctrl_set_channel_color(2, 0xbfd967ff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0xc8df7cff);
    // chn_ctrl_set_channel_color(1, 0xc8df7cff);
    // chn_ctrl_set_channel_color(2, 0xc8df7cff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0xd9e9a4ff);
    // chn_ctrl_set_channel_color(1, 0xd9e9a4ff);
    // chn_ctrl_set_channel_color(2, 0xd9e9a4ff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0xeef5d6ff);
    // chn_ctrl_set_channel_color(1, 0xeef5d6ff);
    // chn_ctrl_set_channel_color(2, 0xeef5d6ff);
    // _delay_ms(1000);

    // chn_ctrl_set_channel_color(0, 0xff929bff);
    // chn_ctrl_set_channel_color(1, 0xff929bff);
    // chn_ctrl_set_channel_color(2, 0xff929bff);
    // _delay_ms(5);
    // chn_ctrl_set_channel_color(0, 0xff929bff);
    // chn_ctrl_set_channel_color(1, 0xff929bff);
    // chn_ctrl_set_channel_color(2, 0xff929bff);
    // _delay_ms(1000);

    chn_ctrl_set_channel_color(0, 0x00000000);
    chn_ctrl_set_channel_color(1, 0x00000000);
    chn_ctrl_set_channel_color(2, 0x00000000);
}