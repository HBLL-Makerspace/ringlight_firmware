/**
 * \file
 * \brief Handles communication for the ringlights.
 * 
 * Handles the sending and receiving of frames. The tick function must be called periodically
 * to receive data. Will be a polled function and not a blocking function.
 * 
 * \author Ben Brenkman and Jack Damiano
 * \date 12 August 2021
 */

#ifndef COMM_HANDLER_H__
#define COMM_HANDLER_H__

#include <stdint.h>
#include <comm/frame.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief initializes the comm handler, specifically the transmission buffers.
 * Only needs to be called once at beginning of main
 */
void comm_handler_init();

/**
 * \brief Sends a single frame. must destroy frame after it is sent
 * 
 * 
 */
uint8_t comm_handler_send_frame();

/**
 * \brief Sends the current transmission frame byte by byte. returns a bool true if transmission is ongoing.
 * returns false when the current frame has finished sending
 * 
 */
bool comm_handler_byte_sender();

/**
 * \brief Stores a single frame for transmitting later.
 * 
 * \param [in] frame Frame that is being sent. Must destroy frame after it has been sent.
 */
void comm_handler_store_frame(comm_frame frame);

/**
 * \brief Gets the received frame from comm_parser.
 */
comm_frame comm_handler_get_frame();

/**
 * \brief Gets the received frame if we have one for this ringlight.
 */
comm_frame comm_handler_get_my_frame();

/**
 * \brief returns if we have a frame ready for this ringlight
 */
bool comm_handler_this_has_frame();

/**
 * \brief Not implemented
 */
uint8_t comm_handler_send_ack();

/**
 * \brief Polls the communication port and runs a tick.
 * 
 * During the tick the usart port is polled, if there is data it is read and
 * passed to the comm_parser. If there is a received frame it will update the flags
 * and allow the frame to be returned by the comm_handler_did_receive_frame function.
 */
uint8_t comm_handler_tick();

/**
 * \brief If a frame has been received since the last frame was.
 */
uint8_t comm_handler_did_receive_frame();

#ifdef __cplusplus
}
#endif

#endif