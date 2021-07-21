#ifndef COMMAND_PARSER_H__
#define COMMAND_PARSER_H__

#include <stdint.h>
#include <comm/frame.h>

#ifdef __cplusplus 
extern "C" {
#endif

uint8_t cp_parse(uint8_t);

uint8_t cp_get_frame();

uint8_t cp_is_frame_available();

uint8_t cp_send_ack();

#ifdef __cplusplus 
}
#endif

#endif