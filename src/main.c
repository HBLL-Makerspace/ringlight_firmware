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
