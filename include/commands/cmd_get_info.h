#ifndef CMD_GET_INFO_H__
#define CMD_GET_INFO_H__

#include<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_GET_INFO_LEN 0x00

uint8_t cmd_get_info_process(uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif