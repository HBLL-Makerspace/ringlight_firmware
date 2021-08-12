#ifndef CMD_SET_CHN_COLOR_W__
#define CMD_SET_CHN_COLOR_W__

#include<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_SET_CHN_COLOR_W_LEN 0x02

uint8_t cmd_set_chn_color_w_process(uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif