/*
 * ws2812b_test_suite.h
 *
 * Created: 6/5/2021 1:35:48 PM
 *  Author: justb
 */ 


#ifndef WS2812B_TEST_SUITE_H_
#define WS2812B_TEST_SUITE_H_

#include <ws2812_driver.h>

void full_rl_rainbow();
void chasing_rainbow();
void ramp_blue();
void ramp_red();
void ramp_green();
void ramp_white();
void snake_rgb();

void ws2812_run_all_tests(uint8_t numGroups, uint8_t numLedsPerGroup, uint8_t degreeSpacing);


#endif /* WS2812B_TEST_SUITE_H_ */