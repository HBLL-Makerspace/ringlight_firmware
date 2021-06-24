#ifndef SYSTEM_INIT_H__
#define SYSTEM_INIT_H__

#include <clock_config.h>
#include <port.h>
#include <ccp.h>

#ifdef __cplusplus
extern "C" {
#endif

void system_init(void);

void system_mcu_init(void);

#ifdef __cplusplus
}
#endif

#endif // __SYSTEM_INIT_H__