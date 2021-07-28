#include <driver_init.h>

#include <drivers/rtc.h>
#include<drivers/pwm.h>
#include<drivers/usart.h>
#include<drivers/ws2812.h>

void driver_init(void) {
    RTC_init(); // Initialize the RTC driver.
    PWM_init(); // Initialize the PWM driver.
    USART_init(); // Initialize the UART driver.
    // WS2812_init(); // Initialize the WS2812 LED driver.
}
