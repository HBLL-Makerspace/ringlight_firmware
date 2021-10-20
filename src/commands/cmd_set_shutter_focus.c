#include<commands/cmd_set_shutter_focus.h>
#include<controllers/focus_shutter_ctrl.h>


//current implementation just turns on/off Fs1 and fs2 LEDs
uint8_t cmd_set_shutter_focus_process(uint8_t* data) {

    focus_shutter_led_test(data[0], data[1]);
    return 0;
}