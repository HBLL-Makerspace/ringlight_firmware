#ifndef COMMAND_PARSER_H__
#define COMMAND_PARSER_H__

#include <stdint.h>
#include <comm/frame.h>

#ifdef __cplusplus 
extern "C" {
#endif


/**
 * \brief parses an input byte and assembles a frame.
 * 
 * Uses the defined protocol to parse a frame from the input stream.
 * A simple state machine keeps track of what part of the protol
 * the stream is in and will trigger a flag once a whole frame has been
 * received.
 */
uint8_t comm_parser_parse(uint8_t);

comm_frame comm_parser_get_frame();

uint8_t comm_parser_is_frame_available();

#ifdef __cplusplus 
}
#endif

#endif