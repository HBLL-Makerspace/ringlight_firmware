/*
 * ws2812b_test_suite.c
 *
 * Created: 6/5/2021 1:37:21 PM
 *  Author: justb
 */ 

#include <ws2812b_test_suite.h>
#include <clock_config.h>
#include <util/delay.h>

static uint8_t num_groups = 0;
static uint8_t num_leds_per_group = 0;
static uint8_t degree_spacing = 0;

void full_rl_rainbow() {
	for (int j = 0; j < 255; j += 1) {
		for (int i = 0; i < num_groups * num_leds_per_group; i++) {
			WS2812_set_pixel_color_C(i, WS2812_Wheel(j)); // Moderately bright green color.
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(10);
	}
}

void chasing_rainbow() {
	for (int j = 0; j < 255; j += 1) {
		for (int i = 0; i < num_groups; i++) {
			// WS2812_Color_RGB takes RGB values, from 0,0,0 up to 255,255,255
			for (uint8_t k = 0; k < num_leds_per_group; k++) {
				WS2812_set_pixel_color_C((i * num_leds_per_group) + k, WS2812_Wheel(j + (i * degree_spacing))); // Moderately bright green color.
			}
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(10);
	}
}

void ramp_blue() {
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < num_groups * num_leds_per_group; i++) {
			WS2812_set_pixel_color_C(i, WS2812_Color_RGB(0, 0, j)); // Moderately bright green color.}
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(1);
	}
	for (int j = 255; j > 0; j--) {
		for (int i = 0; i < num_groups * num_leds_per_group; i++) {
			WS2812_set_pixel_color_C(i, WS2812_Color_RGB(0, 0, j)); // Moderately bright green color.}
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(1);
	}
}

void ramp_green() {
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < num_groups * num_leds_per_group; i++) {
			WS2812_set_pixel_color_C(i, WS2812_Color_RGB(0, j, 0)); // Moderately bright green color.}
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(1);
	}
	for (int j = 255; j > 0; j--) {
		for (int i = 0; i < num_groups * num_leds_per_group; i++) {
			WS2812_set_pixel_color_C(i, WS2812_Color_RGB(0, j, 0)); // Moderately bright green color.}
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(1);
	}
}

void ramp_red() {
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < num_groups * num_leds_per_group; i++) {
		WS2812_set_pixel_color_C(i, WS2812_Color_RGB(j, 0, 0)); // Moderately bright green color.}
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(1);
	}
	for (int j = 255; j > 0; j--) {
		for (int i = 0; i < num_groups * num_leds_per_group; i++) {
			WS2812_set_pixel_color_C(i, WS2812_Color_RGB(j, 0, 0)); // Moderately bright green color.}
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(1);
	}
}

void ramp_white() {
	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < num_groups * num_leds_per_group; i++) {
			WS2812_set_pixel_color_C(i, WS2812_Color_RGB(j, j, j)); // Moderately bright green color.}
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(1);
	}
	for (int j = 255; j > 0; j--) {
		for (int i = 0; i < num_groups * num_leds_per_group; i++) {
			WS2812_set_pixel_color_C(i, WS2812_Color_RGB(j, j, j)); // Moderately bright green color.}
		}
		WS2812_show(); // This sends the updated pixel color to the hardware.
		_delay_ms(1);
	}
}

void snake_rgb() {
	uint16_t num_leds = num_groups * num_leds_per_group;
	for (int16_t i = 0; i < num_leds; i++) {
		int16_t red_led_num = i;
		int16_t orange_led_num = (i - 1) < 0 ? num_leds + (i - 1) : i - 1;
		int16_t yellow_led_num = (i - 2) < 0 ? num_leds + (i - 2) : i - 2;
		int16_t spring_green_led_num = (i - 3) < 0 ? num_leds + (i - 3) : i - 3;
		int16_t green_led_num = (i - 4) < 0 ? num_leds + (i - 4) : i - 4;
		int16_t turquoise_led_num = (i - 5) < 0 ? num_leds + (i - 5) : i - 5;
		int16_t cyan_led_num = (i - 6) < 0 ? num_leds + (i - 6) : i - 6;
		int16_t ocean_led_num = (i - 7) < 0 ? num_leds + (i - 7) : i - 7;
		int16_t blue_led_num = (i - 8) < 0 ? num_leds + (i - 8) : i - 8;
		int16_t violet_led_num = (i - 9) < 0 ? num_leds + (i - 9) : i - 9;
		int16_t magenta_led_num = (i - 10) < 0 ? num_leds + (i - 10) : i - 10;
		int16_t raspberry_led_num = (i - 11) < 0 ? num_leds + (i - 11) : i - 11;
		int16_t clear_led_num = (i - 12) < 0 ? num_leds + (i - 12) : i - 12;
		WS2812_set_pixel_color_RGB(red_led_num, 255, 0, 0);
		WS2812_set_pixel_color_RGB(orange_led_num, 255, 125, 0);
		WS2812_set_pixel_color_RGB(yellow_led_num, 255, 255, 0);
		WS2812_set_pixel_color_RGB(spring_green_led_num, 125, 255, 0);
		WS2812_set_pixel_color_RGB(green_led_num, 0, 255, 0);
		WS2812_set_pixel_color_RGB(turquoise_led_num, 0, 255, 125);
		WS2812_set_pixel_color_RGB(cyan_led_num, 0, 255, 255);
		WS2812_set_pixel_color_RGB(ocean_led_num, 0, 125, 255);
		WS2812_set_pixel_color_RGB(blue_led_num, 0, 0, 255);
		WS2812_set_pixel_color_RGB(violet_led_num, 125, 0, 255);
		WS2812_set_pixel_color_RGB(magenta_led_num, 255, 0, 255);
		WS2812_set_pixel_color_RGB(raspberry_led_num, 255, 0, 125);
		WS2812_set_pixel_color_RGB(clear_led_num, 0, 0, 0);
		WS2812_show();
		_delay_ms(50);
	}
}


// Must first initialize driver before running this function
void ws2812_run_all_tests(uint8_t numGroups, uint8_t numLedsPerGroup, uint8_t degreeSpacing) {
	num_groups = numGroups;
	num_leds_per_group = numLedsPerGroup;
	degree_spacing = degreeSpacing;
	
	ramp_blue();
	ramp_green();
	ramp_red();
	ramp_white();
	full_rl_rainbow();
	full_rl_rainbow();
	chasing_rainbow();
	chasing_rainbow();
	WS2812_clear();
	snake_rgb();
	snake_rgb();
	snake_rgb();
}
