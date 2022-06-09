#include<utils/comm_test_suit.h>
#include<comm/protocol.h>
#include<comm/comm_parser.h>
#include<util/delay.h>
#include<commands/commands.h>
#include<stdio.h>

void comm_test_suite_run_all() {
    uint8_t cmds[] = {PROTOCOL_START_BYTE, 0, CMD_SET_CHN_COLOR_RGB_ID, 0, 255, 255, 255, PROTOCOL_END_BYTE};

    for (uint8_t i = 0; i < 8; i++) {
        comm_parser_parse(cmds[i]);
        _delay_ms(100);
    }

    // if (comm_parser_is_frame_available()) {
    //     printf("Received frame\n");
    // }
}