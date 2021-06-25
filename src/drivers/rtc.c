#include <drivers/rtc.h>
#include <compiler.h>
#include <port.h>

static uint8_t heartbeat_en = 0;

void RTC_init(void) {
	// Wait for RTC to start up.
    while (RTC.STATUS > 0) {}

	// Setup the heartbeat stuff
    RTC.CTRLA = RTC_PRESCALER_DIV1_gc | 0 << RTC_RTCEN_bp | 0 << RTC_RUNSTDBY_bp;
    RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
    RTC.PITCTRLA = RTC_PERIOD_CYC512_gc | RTC_PITEN_bm;
    RTC.PITINTCTRL = 1 << RTC_PI_bp;
}

void RTC_enable_heartbeat(void) {
    heartbeat_en = 1;
}


void RTC_disable_heartbeat(void) {
    heartbeat_en = 0;
}

/**
 * \brief Simple state machines update function.
 * 
 * This basic state machine controls the heartbeat function of the driver.
 * Every call will udpate the internal tick counts and move to various 
 * stages based on the count value. This gives the appearance of a heartbeat.
 * 
 * \return \c void
 */
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


/**
 * \brief Interept service routine for the RTC PIT
 * 
 * The RTC PIT controls the heartbeat function. This ISR calls the heartbeat updated function
 * and resets the interrupt flags in the controller.
 * 
 * \see __heart_beat_update()
 */
ISR(RTC_PIT_vect) {
	if (heartbeat_en) {
		__heart_beat_update();
	}
	RTC.PITINTFLAGS = RTC_PI_bm;
}
