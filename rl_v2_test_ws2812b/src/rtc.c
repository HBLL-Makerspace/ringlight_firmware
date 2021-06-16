/**
 * \file
 *
 * \brief RTC related functionality implementation.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \defgroup doc_driver_rtc_init RTC Init Driver
 * \ingroup doc_driver_rtc
 *
 * \section doc_driver_rtc_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <rtc.h>
#include <atmel_start_pins.h>

static uint8_t heartbeat_en = 0;

/**
 * \brief Initialize rtc interface
 * \return Initialization status.
 */
int8_t RTC_0_init()
{

	while (RTC.STATUS > 0) { /* Wait for all register to be synchronized */
	}

	// RTC.CMP = 0x0; /* Compare: 0x0 */

	// RTC.CNT = 0x0; /* Counter: 0x0 */

	RTC.CTRLA = RTC_PRESCALER_DIV1_gc   /* 1 */
	            | 0 << RTC_RTCEN_bp     /* Enable: enabled */
	            | 0 << RTC_RUNSTDBY_bp; /* Run In Standby: disabled */

	// RTC.PER = 0xffff; /* Period: 0xffff */

	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc; /* 32KHz Internal Ultra Low Power Oscillator (OSCULP32K) */

	// RTC.DBGCTRL = 0 << RTC_DBGRUN_bp; /* Run in debug: disabled */

	// RTC.INTCTRL = 0 << RTC_CMP_bp /* Compare Match Interrupt enable: disabled */
	//		 | 0 << RTC_OVF_bp; /* Overflow Interrupt enable: disabled */

	 RTC.PITCTRLA = RTC_PERIOD_CYC512_gc /* Period */
			 | RTC_PITEN_bm; /* Enabled */

	// RTC.PITDBGCTRL = 0 << RTC_DBGRUN_bp; /* Run in debug: disabled */

	RTC.PITINTCTRL = 1 << RTC_PI_bp; /* Periodic Interrupt: enabled */

	return 0;
}

void RTC_0_enable_heartbeat() {
	heartbeat_en = 1;
}

	
void RTC_0_disable_heartbeat() {
	heartbeat_en = 0;
}

static void __heart_beat_update() {
	static uint16_t _count = 0;
	static uint8_t _state = 0;
	
	switch(_state) {
		case 0:
			if (_count >= 63) {
				_count = 0;
				_state = 1;
				INDICATOR_set_level(true);
			}
			_count++;
			break;
		case 1:
			if (_count >= 5) {
				_count = 0;
				_state = 2;
				INDICATOR_set_level(false);
			}			
			_count++;
			break;
		case 2:
			if (_count >= 10) {
				_count = 0;
				_state = 3;
				INDICATOR_set_level(true);
			}
			_count++;
			break;
		case 3:
			if (_count >= 5) {
				_count = 0;
				_state = 0;
				INDICATOR_set_level(false);
			}
			_count++;
			break;
		default:
			_state = 0;
			_count = 0;
			break;
	}
}


ISR(RTC_PIT_vect) {
	if (heartbeat_en) {
		__heart_beat_update();
	}
	RTC.PITINTFLAGS = RTC_PI_bm;
}