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

/**
 * \brief Gets the last received frame.
 */
comm_frame comm_parser_get_frame();

/**
 * \brief Checks if there is a recent frame that was received.
 */
uint8_t comm_parser_is_frame_available();

/**
 * \brief Free's the frames data so that we don't run out of RAM.
 */
void destory_frame(comm_frame frame);

/**
 * \brief Tells the parser that there was a timeout.
 * 
 * \warning Not currently working. Should be implemented using a timer.
 */
void comm_parser_timeout();

#ifdef __cplusplus 
}
#endif

#endif