#include<commands/commands.h>
#include<commands/cmd_set_chn_color.h>

static const Command cmd_set_chn_color =
{
    .cmd = CMD_SET_CHN_COLOR_RGB_ID,
    .len = CMD_SET_CHN_COLOR_RGB_LEN,
    .process = &cmd_set_chn_color_process
};

const Command* command_get_from_id(uint8_t id) {
    switch (id)
    {
    case 0x00:
        return NULL;
    case 0x02:
        return &cmd_set_chn_color;
    default:
        return NULL;
    }
}