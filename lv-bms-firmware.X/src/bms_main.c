/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include <xc.h>
#include "mcc.h"
#include "units.h"
#include "batt_properties.h"
#include "therm_LUT.h"
#include "millis.h"
#include "error_load_store.h"
#include "indicator_lights.h"
#include "logging.h"
#include "bq796xx.h"
#include "can_interface.h"

#define CELL_COUNT 3

#ifndef min
#define min(a,b) ((a<b) ?a:b)
#endif

typedef enum {WAKEUP_DISCHARGE, WAKEUP_CHARGE} wakeup_mode_t;
wakeup_mode_t wakeup_mode;

// milli second timer hook  - runs the display
void millis_hook (uint64_t uptime) {
    if ((uptime % 64) == 0) {
        disp_update();
        can_update();
    }
}

// how long has the power button been pressed (capped at a maximum duration)
uint64_t power_button_press_duration(uint64_t max_duration){
    static uint64_t press_start_time;
    static bool pressed, prev_pulse_unfinished;
    
    uint64_t press_len = 0;
    uint64_t now = millis();
    
    // main button checking loop
    if(!PowerButton_GetValue()) { // pressed
        if (prev_pulse_unfinished) {
            // waiting for release to end the previous pulse
        }
        else if(!pressed) { // rising edge
            press_start_time = now;
            pressed = true;
            log_dbg("press start: %ums", press_start_time);
        } else { // press held
            // if we've reached the max duration end this press
            if (now - press_start_time > max_duration) {
                log_dbg("max duration of press reached: %u ms", max_duration);
                press_len = now - press_start_time;
                pressed = false;
                prev_pulse_unfinished = true; 
            }
        }
    } else { // unpressed
        if (pressed) { // falling edge
            press_len = now - press_start_time;
            pressed = false;
            log_dbg("press done : %ums", now);
            log_dbg("   len: %u ms", press_len);
        } else { // default state
            prev_pulse_unfinished = false;
        }
    }
    
    if(press_len < 10 && press_len > 0) {
        log_dbg("short pulse found, debouncing it");
        press_len = 0;
    }
    
    return press_len;
}

bool bq_connection_good() {
    int tries_left = 5;
    while (tries_left) {
        if(get_reg_value(1, PARTID) == 0x21) {
            break;
        }
        tries_left--;
    }
    
    return tries_left > 0;
}


void locked_out_main() {
    log_info("locked out main");
    disp_set_critical(true);
    
    while(1){
        // send locked out message
    
//        if (unlock command sent) {
//            // clear lock
//            // reset mcu
//        }
    }
}

void sleep_main() {
    log_info("sleep main");
    disp_set_number(0);
    can_sending_enable(false);
    
    uint64_t loop_count = 0;
    while(1){
        
        // exiting sleep
        uint64_t press = power_button_press_duration(1000);
        if (press == 0) {
            // nothing
        }
        else if(press < 1000) {
            wakeup_mode = WAKEUP_DISCHARGE;
            break;
        } else {
            wakeup_mode = WAKEUP_CHARGE;
            break;
        }
        
        if(loop_count % 1000 == 0) {
            log_info("sleeping");
        }
        
        loop_count++;
    }
    
    log_info("exiting sleep");
}

void charging_main() {
    log_info("charging main");
    disp_set_charging(true);
    disp_set_soc(70);
    // check pack is happy
    // close relay
    
    while(1) {
        // monitor charging
        
        bool charge_done = false;
        if(charge_done) {
            break;
        }
        
        if(power_button_press_duration(2000) >= 2000) {
            break;
        }
    }
    
    // open relay
    
    // indicate charging done
    
    log_info("exiting charging");
}

void discharging_main() {
    log_info("discharging main");
    disp_set_charging(false);
    disp_set_soc(70);
    // check pack is happy
    // close relay
    
    set_config(1, DEV_CONF_NO_ADJACENT_BALANCING | DEV_CONF_MULTIDROP_EN | DEV_CONF_NFAULT_EN);
    set_active_cells(1, 3);
    enable_LPF_cells(1, LPF_6_5Hz);
    set_gpio_conf(1, 1, GPIO_CONF_ADC_OTUT_INPUT);
    delay(1);
    main_ADC_start(1);
    aux_ADC_start(1);
    
    int16_t voltages[3];
    can_register_voltages(voltages);
    can_sending_enable(true);
    
    uint32_t loop_counter = 0;
    while(1) {
        // check connection
        if(loop_counter % 100 == 0) {
            
        }
        
        // monitor discharging
        for(int cell = 1; cell <= 3; cell++) {
            voltages[cell-1] = get_cell_voltage(1, cell) * V_LSB_ADC * 1024;
        }
        send_sensor_message();
        
        
        if (power_button_press_duration(2000) >= 2000) {
            break;
        }
        
        loop_counter++;
    }
    
    log_info("exiting discharging");
}

void powered_on_main() {
    // power on setup
    log_info("powering on");
    
    // power up and check connection to BQ79616
    Wake796XX();
    if (bq_connection_good() == false) {
        log_err("failed to communicate with BQ79616 BMS chip, locking out and going to sleep");
        hard_fault_handler(LOCKOUT_COMM_FAULT);
    }
    
    switch (wakeup_mode) {
        case WAKEUP_DISCHARGE:
            discharging_main();
            break;
        case WAKEUP_CHARGE:
            charging_main();
            break;
    }
    
    // power off shutdown
    log_info("powering off");
    
    SD796XX();
    
}

/*
                         Main application
 */
void bms_main(void) {   
    // Initialize the device
    SYSTEM_Initialize();
    bool button_pressed_on_start = PowerButton_GetValue();
    
    millis_setup();
    INTERRUPT_GlobalInterruptEnable();
    disp_init();
    can_init();
    
    // set default lockout value
    
    wakeup_mode = WAKEUP_DISCHARGE;
    log_info("bms powered up");
    
    // lockout takes precedence
    if(load_lockout_reason() != LOCKOUT_NONE) {
        locked_out_main();
    }
    
    while(1) {
        powered_on_main();
        sleep_main();
    }
    
}
/**
 End of File
*/