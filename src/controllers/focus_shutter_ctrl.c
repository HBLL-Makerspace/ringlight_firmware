#include<math.h>
#include<stdint.h>

#include<port.h>
#include<controllers/focus_shutter_ctrl.h>

void focus_shutter_led_test(bool isOn){
    
    //sets both pins as outputs
    FS1_set_dir(1);
    FS2_set_dir(1);

    //turns on both LEDs
    FS1_set_level(isOn);
    FS2_set_level(isOn);

}
