#include<comm/frame.h>
#include<comm/protocol.h>
#include<comm/comm_parser.h>
#include<commands/commands.h>

#include<stdio.h>

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
    static uint8_t len_counter = 0;

    printf("state: 0x%x, ", parser_state);
    printf("in: 0x%x\n", in);

    switch(parser_state) {
        case S_START:
            printf("START ");
            if (in == PROTOCOL_START_BYTE)
                parser_state = S_ID;
            break;
        case S_ID:
            printf("ID: 0x%x ", in);
            curr.id = in;
            parser_state = S_CMD;
            break;
        case S_CMD:
            printf("CMD: 0x%x ", in);
            curr.cmd = in;
            parser_state = S_DATA;
            len = command_get_from_id(curr.cmd)->len;
            len_counter = 0;
            printf("%d\n", len);
            curr.data = (uint8_t*)malloc(sizeof(uint8_t) * len);
            break;
        case S_DATA:
            printf("DATA: 0x%x, ", in);
            if (len_counter < len) {
                curr.data[len_counter] = in;
                len_counter++;
            }

            if (len_counter == len)
                parser_state = S_END;
            break;
        case S_END:
            printf("] END\n");
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

void destory_frame(comm_frame frame) {
    free(frame.data);
}

void comm_parser_timeout() {
    parser_state = S_START;
}

