/**
 * \file
 */

#include<system_init.h>
#include<drivers/rtc.h>
#include<drivers/pwm.h>
#include<drivers/usart.h>
#include<util/delay.h>
#include<stdio.h>
#include<drivers/ws2812.h>
#include<utils/ws2812_test_suite.h>
#define NUMPIXELS      24
#define LEDS_PER_GROUP 4
#define NUMBER_GROUPS 6
#define DEGREE_SPACING 43

int main(void) {
	system_init();
	RTC_enable_heartbeat();
    // PORTC_set_pin_dir(0, PORT_DIR_OUT);
    // PORTC_set_pin_dir(1, PORT_DIR_OUT);
    // PORTC_set_pin_dir(3, PORT_DIR_OUT);
    // PORTC_set_pin_dir(4, PORT_DIR_OUT);
    // PORTC_set_pin_dir(5, PORT_DIR_OUT);

    // PORTC_set_pin_level(0, true);
    // PORTC_set_pin_level(1, true);
    // PORTC_set_pin_level(3, true);
    // PORTC_set_pin_level(4, true);
    // PORTC_set_pin_level(5, true);

	// PWM_enable();
	// PWM_set_duty_cycle_ch3(100);
	// PWM_enable_ch3();
	// PWM_set_duty_cycle_ch4(255);
	// PWM_enable_ch4();
	// PWM_set_duty_cycle_ch5(1);
	// PWM_enable_ch5();

    // TX_set_dir(PORT_DIR_OUT);
    // TX_set_level(true);
    // RX_set_dir(PORT_DIR_OUT);
    // RX_set_level(true);

    // WS2812_init(WS2812_GRB, NUMBER_GROUPS * LEDS_PER_GROUP, &PORTA.OUT, PIN6_bp);

	while(1) {

        // ws2812_run_all_tests(NUMBER_GROUPS, LEDS_PER_GROUP, DEGREE_SPACING);

        // static uint8_t count = 0;
        // PWM_set_duty_cycle_ch5(count);
        // _delay_ms(10);
        // count++;
        // if (count >= 255) {
        //     count = 0;
        // }

        // TX_set_dir(PORT_DIR_OUT);
        // TX_set_level(true);
        // RX_set_dir(PORT_DIR_OUT);
        // RX_set_level(true);

        printf("Hello\r\n");
        //USART_write(0xff);
        // CHN_0_set_level(true);
        // CHN_1_set_level(true);
        // CHN_2_set_level(true);

        // _delay_ms(500);

        // CHN_0_set_level(false);
        // CHN_1_set_level(false);
        // CHN_2_set_level(false);

        // _delay_ms(500);

        // FS2_set_level(true);
        // _delay_ms(2000);
        // FS1_set_level(true);
        // _delay_ms(500);
        // FS2_set_level(false);
        // FS1_set_level(false);
        // _delay_ms(1000);
        _delay_ms(500);
	}

	return 0;
}
