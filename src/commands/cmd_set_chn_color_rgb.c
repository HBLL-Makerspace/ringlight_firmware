#include<commands/cmd_set_chn_color_rgb.h>
#include<controllers/channel_ctrl.h>

uint8_t cmd_set_chn_color_rgb_process(uint8_t* data) {
    chn_ctrl_set_channel_color_rgb(data[0], data[1], data[2], data[3]);
    return 0;
}