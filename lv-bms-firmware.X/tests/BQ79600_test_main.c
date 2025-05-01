
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
   
//    while(1){
//    while (!UART4_is_tx_ready()) {
//    }
//    UART4_Write(0xAB);
//    printf("sent\n");
//    CLRWDT();
//    }
    uint8_t rx_buff[30] = {0};
    
    Wake796XX(); 
//    StA796XX();
    
    // test comms and get ID
    int read = ReadRegUART(1, PARTID, rx_buff, 1, 100, FRMWRT_SGL_R);
    printf("partid: 0x%02x (should be 0x21 for bq79616)\n",  rx_buff[4]);
    
    read = ReadRegUART(1, TAPEOUT_REV, rx_buff, 1, 100, FRMWRT_SGL_R);
    printf("TAPEOUT_REV: 0x%02x\n",  rx_buff[4]);
    
//     try a read write read test
    int rx_size_1 = ReadRegUART(1, OTP_SPARE1, rx_buff, 1, 100, FRMWRT_SGL_R);
    int original = rx_buff[4];
    int tx_size_1 = WriteRegUART(1, OTP_SPARE1, 0xab, 1, FRMWRT_SGL_W);
    int rx_size_2 = ReadRegUART(1, OTP_SPARE1, rx_buff, 1, 100, FRMWRT_SGL_R);
    int new = rx_buff[4];
    printf("writting 0xab to the OTP_SPARE1, was before write: 0x%02x, is after write: 0x%02x\n", original, new);
    printf("rx1: %d, tx1: %d, rx2: %d\n", rx_size_1, tx_size_1, rx_size_2);
    
    delay(5000);
    SD796XX();
            
    while(1){
        CLRWDT();
        printf(".");
        delay(5000);
    }
    
    return;
    
}
