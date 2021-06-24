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

#ifndef PWM_H__
#define PWM_H__

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initializes the pwm driver.
 * 
 * The driver is initialized by setting up the TCA timer/counter up in split mode.
 * All the pwm channels are disabled and the frequency is set to max speed while
 * still allowing a full byte (256 levels) for the duty cycle.
 * 
 * \warning This function does not enable the PWM driver, to do that you must call PWM_enable();
 * \see PWM_enable
 * \return \c void
 */
void PWM_init(void);


/**
 * \brief Enables the PWM timer/counter
 * \return \c void
 */
void PWM_enable(void);

/**
 * \brief Disables the PWM timer/counter
 * 
 * \return \c void
 */
void PWM_disable(void);

/**
 * \brief Enables channel 0
 * 
 * The wave form associated with this pwm output is waveform 0. Please consult 
 * the documentation to figure out which pin this signal will be output on.
 * 
 * \return \c void
 */
void PWM_enable_ch0(void);

/**
 * \brief Enables channel 1
 * 
 * The wave form associated with this pwm output is waveform 1. Please consult 
 * the documentation to figure out which pin this signal will be output on.
 * 
 * \return \c void
 */
void PWM_enable_ch1(void);

/**
 * \brief Enables channel 2
 * 
 * The wave form associated with this pwm output is waveform 2. Please consult 
 * the documentation to figure out which pin this signal will be output on.
 * 
 * \return \c void
 */
void PWM_enable_ch2(void);

/**
 * \brief Enables channel 3
 * 
 * The wave form associated with this pwm output is waveform 3. Please consult 
 * the documentation to figure out which pin this signal will be output on.
 * 
 * \return \c void
 */
void PWM_enable_ch3(void);

/**
 * \brief Enables channel 4
 * 
 * The wave form associated with this pwm output is waveform 4. Please consult 
 * the documentation to figure out which pin this signal will be output on.
 * 
 * \return \c void
 */
void PWM_enable_ch4(void);

/**
 * \brief Enables channel 5
 * 
 * The wave form associated with this pwm output is waveform 5. Please consult 
 * the documentation to figure out which pin this signal will be output on.
 * 
 * \return \c void
 */
void PWM_enable_ch5(void);

/**
 * \brief Disables channel 0
 * 
 * \return \c void
 */
void PWM_disable_ch0(void);

/**
 * \brief Disables channel 1
 * 
 * \return \c void
 */
void PWM_disbale_ch1(void);

/**
 * \brief Disables channel 2
 * 
 * \return \c void
 */
void PWM_disable_ch2(void);

/**
 * \brief Disables channel 3
 * 
 * \return \c void
 */
void PWM_disable_ch3(void);

/**
 * \brief Disables channel 4
 * 
 * \return \c void
 */
void PWM_disbale_ch4(void);

/**
 * \brief Disables channel 5
 * 
 * \return \c void
 */
void PWM_disable_ch5(void);

/**
 * \brief Set the duty cycle for channel 0.
 * 
 * \param duty The duty cycle of the pwm signal.
 * \return \c void
 * 
 * \note The max value here is determined by the period/frequency of the pwm driver.
 * Currently the period is set to be the max register value for an 8 bit counter, which
 * gives a whole byte to select pwm duty.
 * \warning May not work as intented if period is not at 0xff.
 */
void PWM_set_duty_cycle_ch0(uint8_t duty);

/**
 * \brief Set the duty cycle for channel 1.
 * 
 * \param duty The duty cycle of the pwm signal.
 * \return \c void
 * 
 * \note The max value here is determined by the period/frequency of the pwm driver.
 * Currently the period is set to be the max register value for an 8 bit counter, which
 * gives a whole byte to select pwm duty.
 * \warning May not work as intented if period is not at 0xff.
 */
void PWM_set_duty_cycle_ch1(uint8_t duty);

/**
 * \brief Set the duty cycle for channel 2.
 * 
 * \param duty The duty cycle of the pwm signal.
 * \return \c void
 * 
 * \note The max value here is determined by the period/frequency of the pwm driver.
 * Currently the period is set to be the max register value for an 8 bit counter, which
 * gives a whole byte to select pwm duty.
 * \warning May not work as intented if period is not at 0xff.
 */
void PWM_set_duty_cycle_ch2(uint8_t duty);

/**
 * \brief Set the duty cycle for channel 3.
 * 
 * \param duty The duty cycle of the pwm signal.
 * \return \c void
 * 
 * \note The max value here is determined by the period/frequency of the pwm driver.
 * Currently the period is set to be the max register value for an 8 bit counter, which
 * gives a whole byte to select pwm duty.
 * \warning May not work as intented if period is not at 0xff.
 */
void PWM_set_duty_cycle_ch3(uint8_t duty);

/**
 * \brief Set the duty cycle for channel 4.
 * 
 * \param duty The duty cycle of the pwm signal.
 * \return \c void
 * 
 * \note The max value here is determined by the period/frequency of the pwm driver.
 * Currently the period is set to be the max register value for an 8 bit counter, which
 * gives a whole byte to select pwm duty.
 * \warning May not work as intented if period is not at 0xff.
 */
void PWM_set_duty_cycle_ch4(uint8_t duty);

/**
 * \brief Set the duty cycle for channel 5.
 * 
 * \param duty The duty cycle of the pwm signal.
 * \return \c void
 * 
 * \note The max value here is determined by the period/frequency of the pwm driver.
 * Currently the period is set to be the max register value for an 8 bit counter, which
 * gives a whole byte to select pwm duty.
 * \warning May not work as intented if period is not at 0xff.
 */
void PWM_set_duty_cycle_ch5(uint8_t duty);

#ifdef __cplusplus
}
#endif

#endif