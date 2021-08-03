/**
 * \file
 */

#include<drivers/rtc.h>
#include<system_init.h>
#include<drivers/usart.h>
#include<utils/ws2812_test_suite.h>
#include<controllers/channel_ctrl.h>

#include<drivers/ws2812.h>

#include<stdio.h>
#include<util/delay.h>

#define NUMPIXELS      24
#define LEDS_PER_GROUP 4
#define NUMBER_GROUPS 6
#define DEGREE_SPACING 43


int main(void) {
	system_init();
	RTC_enable_heartbeat();

    // WS2812_init(WS2812_GRB, LEDS_PER_GROUP * 6, &PORTA.OUT, PIN6_bp);

    // _delay_ms(500);

    // WS2812_clear();
    // WS2812_show();

    // chn_ctrl_set_channel_color_rgb(0, 10, 0, 0);
    // chn_ctrl_set_channel_color_rgb(1, 0, 10, 0);
    // chn_ctrl_set_channel_color_rgb(2, 0, 0, 10);
    // chn_ctrl_set_channel_intesity(0, 100);
    // chn_ctrl_set_channel_intesity(1, 100);
    // chn_ctrl_set_channel_intesity(2, 100);

    // chn_ctrl_set_channel_intesity(0, 0);
    // chn_ctrl_set_channel_intesity(1, 0);
    // chn_ctrl_set_channel_intesity(2, 0);

    // _delay_ms(500);

    // for (uint8_t i = 0; i < NUMPIXELS; i ++) {
    //     WS2812_set_pixel_color_RGB(i, 5, 10, 1);
    // }

    FS1_set_level(true);
    FS2_set_level(true);

	while(1) {

        // _delay_ms(2000);
        // chn_ctrl_set_channel_intesity(0, 255);
        // chn_ctrl_set_channel_intesity(1, 255);
        // chn_ctrl_set_channel_intesity(2, 255);
        // _delay_ms(10);
        // chn_ctrl_set_channel_intesity(0, 0);
        // chn_ctrl_set_channel_intesity(1, 0);
        // chn_ctrl_set_channel_intesity(2, 0);
        // _delay_ms(2000);


        // FS2_set_level(true);
        // _delay_ms(2000);
        // FS1_set_level(true);
        // _delay_ms(500);
        // FS1_set_level(false);
        // FS2_set_level(false);
        // _delay_ms(1000);

        // WS2812_show();
        // _delay_ms(500);

        // ws2812_run_all_tests(NUMBER_GROUPS, LEDS_PER_GROUP, DEGREE_SPACING);

        static uint8_t count = 0;
        // PWM_set_duty_cycle_ch5(count);
        chn_ctrl_set_channel_intesity(0, count);
        chn_ctrl_set_channel_intesity(1, count);
        chn_ctrl_set_channel_intesity(2, count);
        _delay_ms(50);
        count++;
        if (count >= 255) {
            count = 0;
        }

        // TX_set_dir(PORT_DIR_OUT);
        // TX_set_level(true);
        // RX_set_dir(PORT_DIR_OUT);
        // RX_set_level(true);

        // printf("Hello\r\n");
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
        // _delay_ms(500);

        // printf("Hello\n");
        // USART_write('h');

        // uint8_t c = USART_read();
        // USART_write(c);
        // chn_ctrl_set_channel_intesity(0, 20);
        // chn_ctrl_set_channel_intesity(1, 20);
        // chn_ctrl_set_channel_intesity(2, 20);
        // _delay_ms(1000);
        // chn_ctrl_set_channel_intesity(0, 0);
        // chn_ctrl_set_channel_intesity(1, 0);
        // chn_ctrl_set_channel_intesity(2, 0);


	}

	return 0;
}
