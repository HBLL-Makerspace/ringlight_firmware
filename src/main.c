/**
 * \file
 */
#include<stdio.h>
#include<util/delay.h>

#include<drivers/rtc.h>
#include<system_init.h>
#include<utils/comm_test_suit.h>
#include<utils/chn_ctrl_test_suite.h>

#include<drivers/usart.h>

#include<comm/frame.h>
#include<comm/comm_handler.h>
#include<commands/commands.h>


#define NUMPIXELS      12
#define LEDS_PER_GROUP 4
#define NUMBER_GROUPS  3
#define DEGREE_SPACING 43


int main(void) {
	system_init();
	RTC_enable_heartbeat();

	while(1) {

        // FS2_set_level(true);
        // _delay_ms(3000);
        // FS1_set_level(true);
        // _delay_ms(500);
        // FS1_set_level(false);
        // FS2_set_level(false);
        // _delay_ms(1000);


        chn_ctrl_test_suite_run_all();

        // printf("Hello\n");
        // _delay_ms(500);

        // char c = USART_read();
        // printf("%c", c);

        // comm_test_suite_run_all();

        // This is the main loop, it should be very short. Cannot use printf in the loop.
        // comm_handler_tick();

        // if (comm_handler_did_receive_frame()) {
        //     comm_frame frame = comm_handler_get_frame();
        //     // printf("received frame\n");
        //     // printf("ID: 0x%x ",  frame.id);
        //     // printf("CMD: 0x%x ",  frame.cmd);
        //     uint8_t len = command_get_from_id(frame.cmd)->len;
        //     // printf("DATA: [ ");
        //     for (uint8_t i = 0; i < len; i++) {
        //         // printf("0x%x ", frame.data[i]);
        //     }
        //     // printf("]\n");

        //     Command* cmd = command_get_from_id(frame.cmd);
        //     cmd->process(frame.data);
        // }
	}

	return 0;
}
