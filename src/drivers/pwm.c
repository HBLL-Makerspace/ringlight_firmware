/**
 * \file
 *
 * \brief Pulse Width Modulation driver using the TCA timer/counter
 * 
 * The PWM driver uses the TCA timer/counter in split mode. This gives access to 6
 * pwm channels. Each of these channels are tied to a specific pin that it can directly
 * drive. To make this driver directly drive the pin make sure that the pin has been
 * configured as an output.
 * 
 * To use the PMW driver follow the below code. The below code shows the steps neede to
 * initialize the driver, enable the pwm driver and then create a pwm signal with a 50%
 * duty cycle.
 * \code
 * ENABLE_INTERRUPTS();
 * PWM_init();
 * PWM_enable();
 * PWM_enable_ch0();
 * PWM_set_duty_cycle_ch0(127);
 * \endcode
 * 
 * \author Ben Brenkman
 * \date 24 June 2021
 * 
 * \see https://github.com/MicrochipTech/TB3217_Getting_Started_with_TCA
 *
 */

#include <drivers/pwm.h>
#include <compiler.h>

static uint8_t UPDATE_PWM_LOW = 0;
static uint8_t UPDATE_PWM_HIGH = 0;

static uint8_t PWM_0 = 0;
static uint8_t PWM_1 = 0;
static uint8_t PWM_2 = 0;
static uint8_t PWM_3 = 0;
static uint8_t PWM_4 = 0;
static uint8_t PWM_5 = 0;

void PWM_init(void) {
    // Put TCA into split mode. Gives us 6 sepereate 8-bit timers.
    TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;
    
    // Disable all the PWM channels by default.
    // TCA0.SPLIT.CTRLB = ~TCA_SPLIT_HCMP0EN_bm 
    //                 | ~TCA_SPLIT_HCMP1EN_bm 
    //                 | ~TCA_SPLIT_HCMP2EN_bm 
    //                 | ~TCA_SPLIT_LCMP0EN_bm 
    //                 | ~TCA_SPLIT_LCMP1EN_bm 
    //                 | ~TCA_SPLIT_LCMP2EN_bm;

    // Set the period to be max.
    TCA0.SPLIT.LPER = 0x80;
    TCA0.SPLIT.HPER = 0x80;
    
    // Set the clock division to be 1 oer CPU cycle and disable the TCA.
    TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV1_gc | ~TCA_SPLIT_ENABLE_bm;
    PORTMUX_CTRLC = PORTMUX_TCA03_bm | PORTMUX_TCA04_bm | PORTMUX_TCA05_bm;

    TCA0.SPLIT.INTCTRL = TCA_SPLIT_LUNF_bm | TCA_SPLIT_HUNF_bm;
}

void PWM_enable(void) {
    // Enable the TCA.
    TCA0.SPLIT.CTRLA |= TCA_SPLIT_ENABLE_bm;
}

void PWM_disable(void) {
    // Disable the TCA.
    TCA0.SPLIT.CTRLA &= ~TCA_SPLIT_ENABLE_bm;
}

void PWM_enable_ch0(void) {
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_LCMP0EN_bm;
}

void PWM_enable_ch1(void) {
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_LCMP1EN_bm;
}

void PWM_enable_ch2(void) {
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_LCMP2EN_bm;
}

void PWM_enable_ch3(void) {
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP0EN_bm;
}

void PWM_enable_ch4(void) {
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP1EN_bm;
}

void PWM_enable_ch5(void) {
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP2EN_bm;
}

void PWM_disable_ch0(void) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP0EN_bm;
}

void PWM_disbale_ch1(void) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP1EN_bm;
}

void PWM_disable_ch2(void) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_LCMP2EN_bm;
}

void PWM_disable_ch3(void) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP0EN_bm;
}

void PWM_disbale_ch4(void) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP1EN_bm;
}

void PWM_disable_ch5(void) {
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP2EN_bm;
}

void PWM_set_duty_cycle_ch0(uint8_t duty) {
    UPDATE_PWM_LOW = 1;
    PWM_0 = duty;
}

void PWM_set_duty_cycle_ch1(uint8_t duty) {
    UPDATE_PWM_LOW = 1;
    PWM_1 = duty;
}

void PWM_set_duty_cycle_ch2(uint8_t duty) {
    UPDATE_PWM_LOW = 1;
    PWM_2 = duty;
}

void PWM_set_duty_cycle_ch3(uint8_t duty) {
    UPDATE_PWM_HIGH = 1;
    PWM_3 = duty;
}

void PWM_set_duty_cycle_ch4(uint8_t duty) {
    UPDATE_PWM_HIGH = 1;
    PWM_4 = duty;
}

void PWM_set_duty_cycle_ch5(uint8_t duty) {
    UPDATE_PWM_HIGH = 1;
    PWM_5 = duty;
}

ISR(TCA0_LUNF_vect) {
    if (UPDATE_PWM_LOW) {
        TCA0.SPLIT.LCMP0 = PWM_0 >> 1;
        TCA0.SPLIT.LCMP1 = PWM_1 >> 1;
        TCA0.SPLIT.LCMP2 = PWM_2 >> 1;
        UPDATE_PWM_LOW = 0;
    }
    TCA0.SPLIT.INTFLAGS = TCA_SPLIT_LUNF_bm;
}

ISR(TCA0_HUNF_vect) {
    if (UPDATE_PWM_HIGH) {
        TCA0.SPLIT.HCMP0 = PWM_3 >> 1;
        TCA0.SPLIT.HCMP1 = PWM_4 >> 1;
        TCA0.SPLIT.HCMP2 = PWM_5 >> 1;
        UPDATE_PWM_HIGH = 0;
    }
    TCA0.SPLIT.INTFLAGS = TCA_SPLIT_HUNF_bm;
}
