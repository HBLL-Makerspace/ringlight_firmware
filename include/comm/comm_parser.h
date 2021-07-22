#ifndef COMMAND_PARSER_H__
#define COMMAND_PARSER_H__

#include <stdint.h>
#include <comm/frame.h>

#ifdef __cplusplus 
extern "C" {
#endif

uint8_t comm_parser_parse(uint8_t);

comm_frame comm_parser_get_frame();

uint8_t comm_parser_is_frame_available();

#ifdef __cplusplus 
}
#endif

#endif