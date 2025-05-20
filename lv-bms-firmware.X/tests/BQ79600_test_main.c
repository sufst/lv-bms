
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
    
    HWRST796XX();
    
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
    enable_LPF_cells(1, LPF_6_5Hz);
    set_gpio_conf(1, 1, GPIO_CONF_ADC_OTUT_INPUT);
    delay(1);
    main_ADC_start(1);
    aux_ADC_start(1);
            
    enable_tsref(1);
    delay(1);
    get_tsref_voltage(1);
    
    OVUV_config(1, OV_THRESH_4200mV, UV_THRESH_3100mV, 3);
    OVUV_start(1);
    get_OVUV_running(1);
    get_ovuv_faults(1);
    
    OTUT_config(1, 10, 80);
    OTUT_start(1);
    get_otut_faults(1);
    get_OTUT_running(1);
    
    
//    for(int i = 100; i ; i--) {
////        int v0 = get_cell_voltage(1,1);
////        get_cell_voltage_aux(1, 1);
//        get_tsref_voltage(1);
//        get_gpio_voltage(1, 1);
////        get_die_temp_1(1);
////        get_die_temp_2(1);
//        delay(500);
//        CLRWDT();
//    }
//    while(1){
//        reset_faults(1, MSK_ALL);
        
//        printf("V1: %fV, V2: %fV, V3: %fV\n", get_cell_voltage(1,1) * V_LSB_ADC, get_cell_voltage(1,2) * V_LSB_ADC, get_cell_voltage(1,3) * V_LSB_ADC );
//        get_gpio_voltage(1, 1);
//        get_otut_faults(1);
//    }
    
//    set_balancing_timer(1, 1, BAL_TIME_600MIN);
//    enable_OTCB(1, 10, 4);
//    balancing_start(1);
//
//    while(!get_balancing_done(1)) {
//        get_balancing_timer(1, 1);
//        get_gpio_voltage(1, 1);
//        get_bal_OT(1);
//    }


    delay(5000);
    SD796XX();
            
    while(1){
        CLRWDT();
        printf(".");
        delay(5000);
    }
    
    return;
    
}
