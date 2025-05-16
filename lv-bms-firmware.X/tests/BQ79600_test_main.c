
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

void millis_hook(uint64_t uptime) {
    if ((uptime % 64) == 0) {
        disp_update();
        can_update();
    }
}

uint8_t SOC;
voltage_t voltages[3] ;
temp_t temps[3] ;
current_t current ;

void bms_main(void) {
    SYSTEM_Initialize();
    millis_setup();
    disp_init();
    can_init();
    INTERRUPT_GlobalInterruptEnable();
    
    printf("\nCAN tests \n");
    disp_set_number(0x55);
//    delay(1000);
    
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
    
    Wake796XX(); 
    //StA796XX();
    
    // test comms and get ID
    printf("partid: 0x%02x (should be 0x21 for bq79616)\n",  get_reg_value(1, PARTID));
    
    printf("DIE_ID1: 0x%02x\n",  get_reg_value(1, DIE_ID1));
    
    // try a read write read test
    int original = get_reg_value(1, OTP_SPARE1);
    set_reg_value(1, OTP_SPARE1, 0xab);
    int new = get_reg_value(1, OTP_SPARE1);
    printf("writting 0xab to the OTP_SPARE1, was before write: 0x%02x, is after write: 0x%02x\n", original, new);
    
    // initial test configuration
    set_config(1, DEV_CONF_NO_ADJACENT_BALANCING | DEV_CONF_MULTIDROP_EN | DEV_CONF_NFAULT_EN);
    set_active_cells(1, 3);
    main_ADC_start(1);
            
    delay(1);
    
    int i = 10;
    
    while(i--) {
        int v0 = get_cell_voltage(1,1);
        delay(500);
        CLRWDT();
    }
    
    i = 10;
    main_ADC_stop(1);
    
    while(i--) {
        int v0 = get_cell_voltage(1,1);
        delay(500);
        CLRWDT();
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
