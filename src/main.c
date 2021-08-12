/**
 * \file
 */
#include<stdio.h>
#include<util/delay.h>

#include<drivers/rtc.h>
#include<system_init.h>
#include<comm/comm_handler.h>
#include<comm/frame.h>

#define NUMPIXELS      12
#define LEDS_PER_GROUP 4
#define NUMBER_GROUPS 3
#define DEGREE_SPACING 43


int main(void) {
	system_init();
	RTC_enable_heartbeat();

	while(1) {
        // This is the main loop, it should be very short. Cannot use printf in the loop.
        comm_handler_tick();

        if (comm_handler_did_receive_frame()) {
            comm_frame frame = comm_handler_get_frame();
            print("received frame");
        }
	}

	return 0;
}
