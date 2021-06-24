#include <driver_init.h>
#include <drivers/rtc.h>
#include<drivers/pwm.h>

void driver_init(void) {
    RTC_init();
    PWM_init();
}
