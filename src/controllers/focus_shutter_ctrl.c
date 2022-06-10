#include<math.h>
#include<stdint.h>
#include <stdio.h>

#include<port.h>
#include<controllers/focus_shutter_ctrl.h>
#include<util/delay.h>
#include<commands/cmd_set_chn_color_rgb.h>




static void Focus(bool delay){
    //turn on LED.Be sure to correct for each channel.
    uint8_t focusColor[] = {1,200,65,20};
    cmd_set_chn_color_rgb_process(focusColor);
    FS1_set_level(1);
    if(true){
        _delay_ms(1000);
    }
    else{
        _delay_ms(600);
    }
    FS1_set_level(0);
    uint8_t lightsOff[] = {1,0,0,0};
    cmd_set_chn_color_rgb_process(lightsOff);
}

static void Shutter(){
    FS2_set_level(1);
    _delay_ms(300);
    FS2_set_level(0);
}

void focus_shutter_led_test(uint8_t focus, uint8_t shutter){
    
    //sets both pins as outputs if trues
    FS1_set_dir(1);
    FS2_set_dir(1);

    // printf("Shutter: %d\n", shutter);
    // _delay_ms(100);
    // printf("Focus: %d\n", focus);
    // _delay_ms(100);

     //focus only
    if(focus && !shutter){
        Focus(true);
        // printf("Focusing\n");
        // _delay_ms(100);

    }

    //take a photo immediately
    else if(shutter && !focus){
        Focus(false);
        Shutter();
    }

    //autofocus and take a photo
    else if(shutter && focus){
        Focus(false);
        Shutter();
    } 

    // //turns on both LEDs
    // FS2_set_level(focus);
    // FS1_set_level(shutter);
    
    
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
