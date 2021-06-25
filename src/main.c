/**
 * \file
 * 
 */

/**
 * \mainpage Ring Light Firmware
 * 
 * \section Introduction
 * This is the firmware for the ringlights that go in the BYU libraries 3D scanning rig. The  ring lights will control the internal lighting environment of the whole rig. There is both RGB and white LEDs that are used in unision to produce a colored environement. The ring lights are dasiy chained and individually addressable.
 * 
 * This is an experimental portion of the firmware that will utilize the command line and make build system. The biggest draw backs are the lack of simualtors available in the linux environment that cover the attiny1606. The only one I could find was the Atmel Studio AVR simualtor. Part of this project will be to ensure that the build system here will work and can also run in that simulator if needed.
 * \section Getting Started
 * View the REAMDE to get started.
 */

#include <system_init.h>
#include <drivers/rtc.h>
#include <drivers/pwm.h>
#include <util/delay.h>

int main(void) {
	system_init();
	RTC_enable_heartbeat();
	PWM_enable();
	PWM_set_duty_cycle_ch3(100);
	PWM_enable_ch3();
	PWM_set_duty_cycle_ch4(100);
	PWM_enable_ch4();
	PWM_set_duty_cycle_ch5(100);
	PWM_enable_ch5();

	while(1) {

	}

	return 0;
}
