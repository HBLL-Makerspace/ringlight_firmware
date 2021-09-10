#include<commands/cmd_set_shutter_focus.h>
#include<controllers/channel_ctrl.h>


//current implementation just turns on an LED
uint8_t cmd_set_shutter_focus_process(uint8_t* data) {
    chn_ctrl_set_channel_color_w(data[0], data[1]);
    return 0;
}