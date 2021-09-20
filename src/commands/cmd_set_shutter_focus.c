#include<commands/cmd_set_shutter_focus.h>
#include<controllers/focus_shutter_ctrl.h>


//current implementation just turns on/off Fs1 and fs2 LEDs
uint8_t cmd_set_shutter_focus_process(uint8_t* data) {
    
    bool isOn = true;
    /* if(!&data){
        isOn = false;
    }  */

    focus_shutter_led_test(isOn);

    return 0;
}