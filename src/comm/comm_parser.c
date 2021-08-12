#include<comm/frame.h>
#include<comm/protocol.h>
#include<comm/comm_parser.h>
#include<commands/commands.h>

typedef enum {
    S_START,
    S_ID,
    S_CMD,
    S_DATA,
    S_END
} comm_parser_state;

static comm_parser_state parser_state = S_START;
static comm_frame frame;
static uint8_t got_frame = 0;

uint8_t comm_parser_parse(uint8_t in) {
    static comm_frame curr;
    static uint8_t len = 0;

    switch(parser_state) {
        case S_START:
            if (in == PROTOCOL_START_BYTE)
                parser_state = S_ID;
        case S_ID:
            curr.id = in;
            parser_state = S_CMD;
            break;
        case S_CMD:
            curr.cmd = in;
            parser_state = S_DATA;
            len = command_get_from_id(curr.id)->len;
            curr.data = (uint8_t*)malloc(sizeof(uint8_t) * len);
            break;
        case S_DATA:
            if (len > 0) {
                len--;
                curr.data[len] = in;
            } else
                parser_state = S_END;

            break;
        case S_END:
            if (in == PROTOCOL_END_BYTE) {
                parser_state = S_START;
                frame = curr;
                got_frame = 1;
            }
            break;
        
        default:
            return 0;
    }
    return 0;
}

comm_frame comm_parser_get_frame() {
    got_frame = 0;
    return frame;
}

uint8_t comm_parser_is_frame_available() {
    return got_frame;
}