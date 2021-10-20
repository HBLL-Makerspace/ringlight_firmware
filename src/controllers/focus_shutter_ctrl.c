#include<math.h>
#include<stdint.h>

#include<port.h>
#include<controllers/focus_shutter_ctrl.h>
#include<util/delay.h>

void focus_shutter_led_test(uint8_t focus, uint8_t shutter){
    
    //sets both pins as outputs if true
    FS1_set_dir(1);
    FS2_set_dir(1);

    //turns on both LEDs
    FS1_set_level(focus);
    FS2_set_level(shutter);
    
    
    //Test
    /*FS1_set_level(true);

    uint8_t num_hundreds = shutter / 100;

    for(uint8_t i = 0; i < num_hundreds; i++){
        FS2_set_level(true);
        
        _delay_ms(500);

        FS2_set_level(false);

        _delay_ms(500);

    }

    _delay_ms(3000);

    uint8_t num_tens = (shutter % 100) / 10;

    for(uint8_t i = 0; i < num_tens; i++){
        FS2_set_level(true);
        
        _delay_ms(500);

        FS2_set_level(false);

        _delay_ms(500);

    }

    uint8_t modulus = shutter % 10;

    _delay_ms(3000);

    for(uint8_t i = 0; i < modulus; i++){
        FS2_set_level(true);
        
        _delay_ms(500);

        FS2_set_level(false);

        _delay_ms(500);

    }

    FS1_set_level(false);*/
    
}
