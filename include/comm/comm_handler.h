/**
 * \file
 * \brief Handles communication for the ringlights.
 * 
 * Handles the sending and receiving of frames. The tick function must be called periodically
 * to receive data. Will be a polled function and not a blocking function.
 * 
 * \author Ben Brenkman
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
 * \brief Sends a single frame.
 * 
 * \param [in] frame Frame that is being sent. Must destroy frame after it has been sent.
 */
uint8_t comm_handler_send_frame(comm_frame frame);

/**
 * \brief Gets the received frame.
 */
comm_frame comm_handler_get_frame();

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