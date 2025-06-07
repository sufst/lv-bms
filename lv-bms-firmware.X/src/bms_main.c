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
#include "millis.h"
#include "error_load_store.h"
#include "indicator_lights.h"
#include "logging.h"
#include "bq_interface.h"
#include "can_interface.h"

#define CELL_COUNT 3

#ifndef min
#define min(a,b) ((a<b) ?a:b)
#endif

typedef enum {WAKEUP_DISCHARGE, WAKEUP_CHARGE} wakeup_mode_t;
wakeup_mode_t wakeup_mode;

voltage_t voltages[3];
temp_t temps[3];
current_t current;
int8_t SOC = 50;

// milli second timer hook
void millis_hook (uint64_t uptime) {

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


void locked_out_main() {
    log_info("locked out main");
    disp_set_critical(true);
    disp_set_soc(0); // enables animations
    can_set_status(CAN_LOCKED_OUT);
    can_set_lockdout(load_lockout_reason(), load_lockout_cell(), load_lockout_value());
    
    while(1){
        // send locked out message
        can_update();
        disp_update();
    
        if (get_lockout_clear_message_rxed()) {
            save_lockout_reason(LOCKOUT_NONE, 0, 0);
            delay(1);
            RESET();
        }
    }
}

void sleep_main() {
    log_info("sleep main");
    disp_set_number(0);
    can_sensor_sending_enable(false);
    
    uint64_t loop_count = 0;
    while(1){
        // exiting sleep
        uint64_t press = power_button_press_duration(1000);
        if (press == 0) {
            // nothing
        } else if(press < 1000) {
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
    can_set_status(CAN_CHARGING);
    can_sensor_sending_enable(true);
    // check pack is happy
    // close relay
    
    uint32_t loop_counter = 0;
    while(1) {
        //-------------------------- MEASURE --------------------------
        // check connection
        if(loop_counter % 100 == 0) {
            if (bq_check_connection() == false) {
                log_err("failed to communicate with BQ79616 BMS chip, locking out and going to sleep");
                hard_fault_handler(LOCKOUT_COMM_FAULT);
            }
            
            if(bq_check_measuring() == false) {
                log_err("a measurement system on the BQ79616 has failed, locking out and going to sleep");
                hard_fault_handler(LOCKOUT_MEASURE_FAULT);
            }
        }
        
        bq_get_voltages(voltages);
        bq_get_temperatures(temps);
        bq_get_current(&current);
        
        // check for going to sleep
        if (power_button_press_duration(2000) >= 2000) {
            break;
        }
        
        //-------------------------- ANALYSE --------------------------
        
        //-------------------------- ACT --------------------------   
        can_update();
        disp_update();
        
        bool charge_done = false;
        if(charge_done) {
            break;
        }
        
        if(power_button_press_duration(2000) >= 2000) {
            break;
        }
        
        loop_counter++;
    }
    
    // open relay
    
    // indicate charging done
    
    log_info("exiting charging");
    can_set_status(CAN_FULL);
    disp_set_charging(false);
}

void discharging_main() {
    log_info("discharging main");
    disp_set_soc(70);
    can_set_status(CAN_DISCHARGING);
    can_sensor_sending_enable(true);
    // check pack is happy
    // close relay
    
    uint32_t loop_counter = 0;
    while(1) {
        //-------------------------- MEASURE --------------------------
        // check connection
        if(loop_counter % 100 == 0) {
            if (bq_check_connection() == false) {
                log_err("failed to communicate with BQ79616 BMS chip, locking out and going to sleep");
                hard_fault_handler(LOCKOUT_COMM_FAULT);
            }
            
            if(bq_check_measuring() == false) {
                log_err("a measurement system on the BQ79616 has failed, locking out and going to sleep");
                hard_fault_handler(LOCKOUT_MEASURE_FAULT);
            }
        }
        
        bq_get_voltages(voltages);
        bq_get_temperatures(temps);
        bq_get_current(&current);
        
        // check for going to sleep
        if (power_button_press_duration(2000) >= 2000) {
            break;
        }
        
        //-------------------------- ANALYSE --------------------------
        
        //-------------------------- ACT --------------------------
        can_update();
        disp_update();
        loop_counter++;
    }
    
    log_info("exiting discharging");
    can_set_status(CAN_POWERED_DOWN);
}

void powered_on_main() {
    // power on setup
    log_info("powering on");
    
    // power up and check connection to BQ79616
    bq_wake();
    delay(10);
    if (bq_check_connection() == false) {
        log_err("failed to communicate with BQ79616 BMS chip, locking out and going to sleep");
        hard_fault_handler(LOCKOUT_COMM_FAULT);
    }
    
    bq_setup();
    
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
    
    can_sensor_sending_enable(false);
    bq_shutdown();
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
    
    can_register_voltages(voltages);
    can_register_temps(temps);
    can_register_current(&current);
    can_register_SOC(&SOC);
    
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