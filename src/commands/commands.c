#include <commands/commands.h>
#include <commands/cmd_set_chn_color_rgb.h>
#include <commands/cmd_set_chn_color_w.h>
#include <commands/cmd_set_shutter_focus.h>

static const Command cmd_set_chn_color_rgb = {
    .cmd = CMD_SET_CHN_COLOR_RGB_ID,
    .len = CMD_SET_CHN_COLOR_RGB_LEN,
#ifndef UNIT_TEST
    .process = &cmd_set_chn_color_rgb_process,
#endif
};

static const Command cmd_set_chn_color_w = {
    .cmd = CMD_SET_CHN_COLOR_W_ID,
    .len = CMD_SET_CHN_COLOR_W_LEN,
#ifndef UNIT_TEST
    .process = &cmd_set_chn_color_w_process,
#endif
};

// command to control focus and shutter
static const Command cmd_set_shutter_focus = {
    .cmd = CMD_SET_SHUTTER_FOCUS_ID,
    .len = CMD_SET_SHUTTER_FOCUS_LEN,
#ifndef UNIT_TEST
    .process = &cmd_set_shutter_focus_process,
#endif
};

const Command* command_get_from_id(uint8_t id) {
  switch (id) {
    case 0x00:
      return &cmd_set_chn_color_rgb;
    case CMD_SET_CHN_COLOR_RGB_ID:
      return &cmd_set_chn_color_rgb;
    case CMD_SET_CHN_COLOR_W_ID:
      return &cmd_set_chn_color_w;
    case CMD_SET_SHUTTER_FOCUS_ID:
      return &cmd_set_shutter_focus;
    default:
      return &cmd_set_chn_color_rgb;
  }
};