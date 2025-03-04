
/*
 * File:   bq79600 test main.c
 * Author: alexl
 *
 * Created on February 11, 2025, 8:51 PM
 */


#include <xc.h>
#include "indicator_lights.h"
#include "mcc.h"
#include "millis.h"
#include "stdio.h"
#include "can_interface.h"
#include "units.h"
#include "error_load_store.h"
#include "bq796xx.h"

void millis_hook (uint64_t uptime) {
    if ((uptime % 64) == 0) {
        disp_update();
        can_update();
    }
}

uint8_t SOC;
voltage_t voltages[3] ;
temp_t temps[3] ;
current_t current ;

void main(void) {
    SYSTEM_Initialize();
    millis_setup();
    disp_init();
    can_init();
    INTERRUPT_GlobalInterruptEnable();
    
    printf("\nCAN tests \n");
    disp_set_number(0xaa);
    delay(1000);
    
    // initialise can interface
    can_register_SOC(&SOC);
    can_register_voltages(voltages);
    can_register_temps(temps);
    can_register_current(&current);
    
    can_set_status(CAN_POWERED_DOWN);
    can_set_charge_cycles(123);
    can_set_lockout_count(6);
    can_set_shutdown_count(2093);
    
    can_sending_enable(true);
   
//    while(1){
//    while (!UART4_is_tx_ready()) {
//    }
//    UART4_Write(0xAB);
//    printf("sent\n");
//    CLRWDT();
//    }
    
    uint8_t part_id[30] = {0};
    
    Wake796XX(); 
//    StA796XX();
    
    
//  WriteRegUART(1, CONTROL1, 1<<3, 1, FRMWRT_SGL_W); 
    
//    WriteRegUART(1, 0x701, 1<<4, 1, FRMWRT_SGL_W); // works
    int read = ReadRegUART(1, PARTID, part_id, 8, 50, FRMWRT_SGL_R);
    
    
    printf("read %d bytes\n", read);
    
//    printf("partid: %02x\n", part_id[4]);
    for(int i=0; i<16; i++) {
        printf("read: %02d : %02x\n", i, part_id[i]);
    }
    
    
    delay(5000);
    SD796XX();
            
    while(1){
        CLRWDT();
        printf(".");
        delay(5000);
    }
    
    return;
    
}
