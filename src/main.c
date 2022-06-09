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
#include<commands/cmd_set_shutter_focus.h>


#define NUMPIXELS      12
#define LEDS_PER_GROUP 4
#define NUMBER_GROUPS  3
#define DEGREE_SPACING 43
#define LED_ON 1


int main(void) {
	system_init();
    comm_handler_init();
	RTC_enable_heartbeat();
    uint8_t led = LED_ON;

	while(1) {

        // FS2_set_level(true);
        // _delay_ms(3000);
        // FS1_set_level(true);
        // _delay_ms(500);
        // FS1_set_level(false);
        // FS2_set_level(false);
        // _delay_ms(1000);


        //test for all I/O minus input.
        // chn_ctrl_test_suite_run_all();
        // uint8_t cmd1[] = {0,1};
        // cmd_set_shutter_focus_process(cmd1);
        // printf("serial test\n");
        // _delay_ms(100);




        // cmd_set_shutter_focus_process(0);
        // cmd_set_shutter_focus_process(&led);


        //comm_test_suite_run_all();

        // This is the main loop, it should be very short. Cannot use printf in the loop.
        comm_handler_tick();

        //FIXME if there is a frame for ME to execute, do it
        if (comm_handler_this_has_frame()) {
            comm_frame frame = comm_handler_get_my_frame();

            Command* cmd = command_get_from_id(frame.cmd);
            cmd->process(frame.data);
        }
        
	}
 
	return 0;
}
