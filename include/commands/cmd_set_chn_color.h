#ifndef CMD_SET_CHN_COLOR__
#define CMD_SET_CHN_COLOR__

#include<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_SET_CHN_COLOR_LEN 0x04

uint8_t cmd_set_chn_color_process(uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif