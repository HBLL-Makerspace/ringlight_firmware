#ifndef RTC_H__
#define RTC_H__

#include <compiler.h>
#include <utils_assert.h>

#ifdef __cplusplus
extern "C" {
#endif

int8_t RTC_init(void);
void RTC_enable_heartbeat(void);
void RTC_disable_heartbeat(void);

#ifdef __cplusplus
}
#endif

#endif