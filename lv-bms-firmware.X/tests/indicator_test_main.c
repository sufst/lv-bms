/*
 * File:   indicator_test_main.c
 * Author: alexl
 *
 * Created on February 11, 2025, 8:51 PM
 */


#include <xc.h>
#include "../indicator_lights.h"
#include "../mcc_generated_files/mcc.h"
#include "../millis.h"



void main(void) {
    SYSTEM_Initialize();
    millis_setup();
    disp_init();
    
    
    INTERRUPT_GlobalInterruptEnable();
            
    disp_set_number(0xaa);
    
    
    uint64_t last = millis();
    uint8_t n = 0;
    
    while(1){
        disp_update();
        CLRWDT();
        
        if (millis() - last > 1000) {
            disp_set_number(++n);
            last = millis();
        }
    }
    
    return;
    
}
