#ifndef COMM_HANDLER_H__
#define COMM_HANDLER_H__

#include <stdint.h>
#include <comm/frame.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t comm_handler_send_frame(comm_frame frame);
uint8_t comm_handler_recieve_frame();
comm_frame comm_handler_get_frame();
uint8_t comm_handler_send_ack();
uint8_t comm_handler_tick();
uint8_t comm_handler_did_receive_frame();

#ifdef __cplusplus
}
#endif

#endif