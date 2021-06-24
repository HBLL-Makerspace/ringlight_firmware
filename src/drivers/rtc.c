#include <drivers/rtc.h>
#include <compiler.h>
#include <port.h>

static uint8_t heartbeat_en = 0;

int8_t RTC_init(void) {
    while (RTC.STATUS > 0) {}

    RTC.CTRLA = RTC_PRESCALER_DIV1_gc | 0 << RTC_RTCEN_bp | 0 << RTC_RUNSTDBY_bp;
    RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
    RTC.PITCTRLA = RTC_PERIOD_CYC512_gc | RTC_PITEN_bm;
    RTC.PITINTCTRL = 1 << RTC_PI_bp;

    return 0;
}

void RTC_enable_heartbeat(void) {
    heartbeat_en = 1;
}


void RTC_disable_heartbeat(void) {
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
