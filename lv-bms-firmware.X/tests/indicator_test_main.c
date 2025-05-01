/*
 * File:   indicator_test_main.c
 * Author: alexl
 *
 * Created on February 11, 2025, 8:51 PM
 */


#include <xc.h>
#include "indicator_lights.h"
#include "mcc.h"
#include "millis.h"
#include "stdio.h"

void millis_hook (uint64_t uptime) {
    if ((uptime % 64) == 0) {
        disp_update();
    }
}

void bms_main(void) {
    SYSTEM_Initialize();
    millis_setup();
    disp_init();
    INTERRUPT_GlobalInterruptEnable();
    

    printf("indicator test starting\n");
    disp_set_number(0xaa);
       
    delay(1000);
    
//     number test
    printf("number test\n");
    for (int i = 0; i<32; i++ ){
        disp_set_number(i);
        printf("    number: %d\n", i);
        delay(100);
    }
    
    // soc test
    printf("discharge test:\n");
    disp_set_critical(false);
    disp_set_charging(false);
    disp_set_empty(false);
    for (int soc = 100; soc >= 0; soc -= 10) {
        disp_set_soc((uint8_t)soc);
        printf("    discharge: %d%%\n", soc);
        delay(1500);
    }
    
    // charge test
    printf("charge test:\n");
    disp_set_critical(false);
    disp_set_charging(true);
    disp_set_empty(false);
    for (int soc = 0; soc <= 100; soc += 10) {
        disp_set_soc((uint8_t)soc);
        printf("    charge: %d\n", soc);
        delay(2000);
    }
    delay(1000);
    
    // empty anim test
    printf("empty anim test:\n");
    disp_set_critical(false);
    disp_set_charging(false);
    disp_set_empty(true);
    for (int soc = 100; soc >= 0; soc -= 10) {
        disp_set_soc((uint8_t)soc);
        printf("    empty: %d\n", soc);
        delay(1000);
    }
    
    // critical anim test
    printf("empty anim test:\n");
    disp_set_critical(true);
    disp_set_charging(false);
    disp_set_empty(false);
    for (int soc = 100; soc >= 0; soc -= 10) {
        disp_set_soc((uint8_t)soc);
        printf("    critical: %d\n", soc);
        delay(1000);
    }
    
    while(1){
        CLRWDT();
        printf("no more tests\n");
        delay(1000);
    }
    
    return;
    
}
