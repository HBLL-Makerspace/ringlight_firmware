#include<drivers/usart.h>

#include<comm/frame.h>
#include<comm/comm_parser.h>
#include<comm/comm_handler.h>

uint8_t comm_handler_send_frame(comm_frame frame) {
    return 0;
}

uint8_t comm_handler_recieve_frame() {
    return 0;
}

comm_frame comm_handler_get_frame() {
    return comm_parser_get_frame();
}

uint8_t comm_handler_send_ack() {
    return 0;
}

uint8_t comm_handler_did_receive_frame() {
    return comm_parser_is_frame_available();
}

uint8_t comm_handler_tick() {
    if (USART_is_rx_ready())
        comm_parser_parse(USART_read());
    return 0;
}
