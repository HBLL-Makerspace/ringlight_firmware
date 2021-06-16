#include <atmel_start.h>
#include <ws2812b_test_suite.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24
#define LEDS_PER_GROUP 4
#define NUMBER_GROUPS 6
#define DEGREE_SPACING 43

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	ENABLE_INTERRUPTS();
	RTC_0_enable_heartbeat();
	WS2812_init(WS2812_GRB, NUMBER_GROUPS * LEDS_PER_GROUP, &PORTA.OUT, PIN2_bp);
	
	/* Replace with your application code */
	while (1) {
		ws2812_run_all_tests(NUMBER_GROUPS, LEDS_PER_GROUP, DEGREE_SPACING);
	}
}
