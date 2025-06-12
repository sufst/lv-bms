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
#include "can_interface.h"
#include "units.h"
#include "error_load_store.h"

void millis_hook (uint64_t uptime) {
}

uint8_t SOC;
voltage_t voltages[3] = {V(3.6), V(3.7), V(3.8)};
temp_t temps[3] = {25,26,27};
current_t current = A(-5);

void bms_main(void) {
    SYSTEM_Initialize();
    millis_setup();
    disp_init();
    can_init();
    INTERRUPT_GlobalInterruptEnable();
    
    printf("\nCAN tests \n");
    disp_set_number(0xaa);
    
    can_register_SOC(&SOC);
    can_register_voltages(voltages);
    can_register_temps(temps);
    can_register_current(&current);
    
    
    can_set_status(CAN_POWERED_DOWN);
    can_set_charge_cycles(123);
    can_set_lockout_count(6);
    can_set_shutdown_count(2093);
    
    can_set_lockdout(LOCKOUT_OVERVOLT, 3, V(2.8));
    
    can_sensor_sending_enable(true);
    disp_update();
    can_update();
    
    uint64_t last_update = millis();
    while(1) {
        uint64_t now = millis();
        if ((now - last_update > 100)){
            last_update = now;
            SOC = (SOC < 100) ? SOC+1 : 0;
            disp_set_soc(SOC);
            printf("SOC: %d\n", SOC);
            can_send_critical_warning(CAN_CRITICAL_TEMP, 2, 70);
            disp_update();
            can_update();
        }
        CLRWDT();
    }
    
    while(1){
        CLRWDT();
        printf("no more tests\n");
        delay(1000);
    }
    
    return;
    
}
