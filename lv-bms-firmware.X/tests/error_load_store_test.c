
/*
 * File:   error_load_store_test.c
 * Author: alexl
 *
 */


#include <xc.h>
#include "mcc.h"
#include "millis.h"
#include "stdio.h"
#include "error_load_store.h"

void millis_hook (uint64_t uptime) {

}

void store_reasons() {
    save_lockout_reason(LOCKOUT_COMM_FAULT, 2, 12345);
    save_shutdown_reason(SHUTDOWN_REASON_CHARGE_DONE);
    printf("saved lockout and shutdown reasons\n");
}

void read_reasons() {
    shutdown_reason_t sd_r = load_shutdown_reason();
    lockout_reason_t lo_r = load_lockout_reason();
    int16_t lo_val = load_lockout_value();
    uint8_t lo_cell = load_lockout_cell();
    printf("loaded fault reasons:  shutdown: 0x%02x, lockout: 0x%02x, cell: %d value: %d \n", sd_r, lo_r, lo_cell, lo_val);
}

void bms_main(void) {
    SYSTEM_Initialize();
    millis_setup();
    INTERRUPT_GlobalInterruptEnable();
    
    printf("load store test\n");
    
    read_reasons();

    if (false) {
        store_reasons();
    }

    
    while(1){
        CLRWDT();
        printf(".");
        delay(5000);
    }
    
    return;
}
