#include<commands/cmd_set_chn_color_w.h>
#include<controllers/channel_ctrl.h>

uint8_t cmd_set_chn_color_w_process(uint8_t* data) {
    chn_ctrl_set_channel_color_w(data[0], data[1]);
    return 0;
}