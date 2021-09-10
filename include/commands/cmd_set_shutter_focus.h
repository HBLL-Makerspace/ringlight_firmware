#ifndef CMD_SET_SHUTTER_FOCUS__
#define CMD_SET_SHUTTER_FOCUS__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_SET_SHUTTER_FOCUS_LEN 0x01

uint8_t cmd_set_shutter_focus_process(uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif