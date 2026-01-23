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
#define RELAY_COIL_R 160 // ohms
#define IDLE_CURRENT -A(0)

#define CHARGING_END_CURRENT A(0.03)
#define CHARGING_END_TIME_MS 300e3 // seconds

#ifndef min
#define min(a,b) ((a<b) ?a:b)
#endif


typedef enum {NONE, CUTOFF, LOCKOUT} cell_issue_level_t;

voltage_t voltages[CELL_COUNT];
temp_t temps[CELL_COUNT];
current_t current;
int8_t SOC = 50;

typedef struct {
    cell_issue_level_t issue_level;
    shutdown_reason_t shutdown_reason;
    lockout_reason_t lockout_reason;
} cell_report;

cell_report get_cell_report(uint8_t cell_index) {
    voltage_t cell_v = voltages[cell_index];
    temp_t cell_t = temps[cell_index];
    
    cell_report r;

    // function will return after an error is reached
    // so it's important that lockouts are checked first
    // as they are higher priority

    if (cell_v > OVERVOLT_LOCKOUT) {
        r.issue_level = LOCKOUT;
        r.lockout_reason = LOCKOUT_OVERVOLT;
        return r;
    }
    if (cell_v < UNDERVOLT_LOCKOUT) {
        r.issue_level = LOCKOUT;
        r.lockout_reason = LOCKOUT_UNDERVOLT;
        return r;
    }
    if (cell_t > OVERTEMP_LOCKOUT) {
        r.issue_level = LOCKOUT;
        r.lockout_reason = LOCKOUT_OVERTEMP;
        return r;
    }

    // check cutoff conditions

    if (cell_v > CELL_VOLTAGE_MAX_CUTOFF) {
        r.issue_level = CUTOFF;
        r.shutdown_reason = SHUTDOWN_REASON_OVER_V;
        return r;
    }
    if (cell_v < CELL_VOLTAGE_MIN_CUTOFF) {
        r.issue_level = CUTOFF;
        r.shutdown_reason = SHUTDOWN_REASON_UNDER_V;
        return r;
    }
    if (cell_t > CELL_TEMP_MAX_CUTOFF) {
        r.issue_level = CUTOFF;
        r.shutdown_reason = SHUTDOWN_REASON_OVER_TEMP;
        return r;
    }
    if (cell_t < CELL_TEMP_MIN_CUTOFF) {
        r.issue_level = CUTOFF;
        r.shutdown_reason = SHUTDOWN_REASON_UNDER_TEMP;
        return r;
    }

    // if (mode == WAKEUP_CHARGE && current > CHARGE_CURRENT_MAX_CUTOFF) { // for charging
    //     r.issue_level = CUTOFF;
    //     r.shutdown_reason = SHUTDOWN_REASON_OVER_CURR;
    //     return r;
    // }
    // if (mode == WAKEUP_DISCHARGE && current > DISCHARGE_CURRENT_MAX_CUTOFF) { // for discharging
    //     r.issue_level = CUTOFF;
    //     r.shutdown_reason = SHUTDOWN_REASON_OVER_CURR;
    //     return r;
    // }
    
    // control flow reaches here if no errors

    r.issue_level = NONE;
    return r;
}

// void send_cell_warnings(uint8_t cell_index, wakeup_mode_t mode) {
//    // check current
//    if (mode == WAKEUP_CHARGE && check_over_current_charge(current)) { // for charging
//        can_send_critical_warning(CAN_CRITICAL_CURRENT, i, cell_v);
//    }
//    if (mode == WAKEUP_DISCHARGE && check_over_current_discharge(current)) { // for discharging
//        can_send_critical_warning(CAN_CRITICAL_CURRENT, i, cell_v);
//    }

//    // check voltage
//    if (check_over_volt(cell_v)) can_send_critical_warning(CAN_CRITICAL_VOLTAGE, i, cell_v);
//    if (check_under_volt(cell_v)) can_send_critical_warning(CAN_CRITICAL_VOLTAGE, i, cell_v);

//    // check temp
//    if (check_over_temp(cell_t)) can_send_critical_warning(CAN_CRITICAL_TEMP, i, cell_t);
//    if (check_under_temp(cell_t)) can_send_critical_warning(CAN_CRITICAL_TEMP, i, cell_t);
// }

// milli second timer hook
void millis_hook (time_t uptime) {

}

// how long has the power button been pressed (capped at a maximum duration)
uint64_t power_button_press_duration(uint64_t max_duration){
    static uint64_t press_start_time;
    static bool pressed, prev_pulse_unfinished;
    
    uint64_t press_len = 0;
    uint64_t now = millis();
    
    // main button checking loop
    if(MasterSwitch_GetValue()) { // pressed
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
    
        if (can_get_lockout_clear_message_rxed()) {
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
        uint64_t press = power_button_press_duration(UINT64_MAX);
        if (press == 0) {
            // nothing
        } else {
            break;
        }
        
        if(loop_count % 1000 == 0) {
            log_info("sleeping");
        }
        
        loop_count++;
    }
    
    log_info("exiting sleep");
}


void powered_on_main() {
    uint64_t charge_end_timer_start_time = 0; // time at which the current first drops below the charge end current
    uint32_t loop_counter = 0;

    // power on setup
    log_info("powering on");
    
    RelayCtrl_SetHigh();
    
    // power up and check connection to BQ79616
    bq_hw_reset(); // TODO: double check the consequences of this - this seems to be needed for the communication to start back up correctly.
    bq_wake();
    delay(100);
    if (bq_check_connection() == false) {
        log_err("failed to communicate with BQ79616 BMS chip, locking out and going to sleep");
        hard_fault_handler(LOCKOUT_COMM_FAULT);
    }
    bq_setup();
    
    disp_set_charging(false);
    disp_set_soc(70);
    can_set_status(CAN_DISCHARGING);
    can_sensor_sending_enable(true);
    
    //==================================================================================================================
    //  Main LOOP
    //==================================================================================================================
    while(1) {
        uint64_t now = millis();
        //---------------------------------------------------- MEASURE -------------------------------------------------
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
        
        log_dbg("voltages: %d, %d, %d", voltages[0], voltages[1], voltages[2]);
        log_dbg("temps: %d, %d, %d", temps[0], temps[1], temps[2]);
        log_dbg("current: %d", current);
        
        //---------------------------------------------------- ANALYSE -------------------------------------------------
        // estimate current through the cells
        current_t relay_current =  ((float)((voltages[0] + voltages[1] + voltages[2]) / VOLTAGE_MULTIPLIER ) / RELAY_COIL_R ) * CURRENT_MULTIPLIER;
        current_t cells_current = current - relay_current - IDLE_CURRENT ;
        log_dbg("relay current: %fA, cell current: %fA", (float)relay_current/CURRENT_MULTIPLIER, (float)cells_current/CURRENT_MULTIPLIER);


        // bool lockout = false;
        // bool cutoff = false;

        // for (uint8_t i = 0; i < CELL_COUNT; i++) {
        //     send_cell_warnings(i, WAKEUP_CHARGE);
        //     cell_report r get_cell_report(i, WAKEUP_CHARGE);
        //     if (r.issue_level == LOCKOUT) {
        //         save_lockout_reason(r.lockout_reason);
        //         lockout = true;
        //         break;
        //     }
        //     else if (r.issue_level == CUTOFF) {
        //         save_shutdown_reason(r.shutdown_reason);
        //         cutoff = true;
        //         break;
        //     }
        // }
        // if (lockout) {
        //     locked_out_main();
        //     // locked_out_main should be blocking, but just in case, enter shutdown if the function terminates
        //     break;
        // }
        // else if (cutoff) {
        //     // break to shutdown
        //     break;
        // }

        // check for going to sleep
        if (power_button_press_duration(2000) >= 2000 || !OffButton_GetValue()) {
            break;
        }
        
        //---------------------------------------------------- ACT -----------------------------------------------------
        disp_set_soc(SOC);
        can_update();
        disp_update();
        
        // // charge done detection; the cell current drops below the end current threshold for a set time
        // if (cells_current < CHARGING_END_CURRENT) {
        //     if(charge_end_timer_start_time == 0){
        //         charge_end_timer_start_time = now;
        //     }
        //     if((now - charge_end_timer_start_time) > CHARGING_END_TIME_MS) {
        //         break;
        //     }
        // } else {
        //     charge_end_timer_start_time = 0;
        // }
        
        
        // check for going to sleep by button
        if (power_button_press_duration(2000) >= 2000 || !OffButton_GetValue()) {
            break;
        }

        loop_counter++;
    }

    // power off shutdown
    log_info("powering off");
    
    can_set_status(CAN_POWERED_DOWN);
    can_sensor_sending_enable(false);
    bq_shutdown();
    
    // open relay
    RelayCtrl_SetLow();
}

/*
                         Main application
 */
void bms_main(void) {   
    // Initialize the device
    SYSTEM_Initialize();
    bool button_pressed_on_start = MasterSwitch_GetValue();
    
    millis_setup();
    INTERRUPT_GlobalInterruptEnable();
    disp_init();
    can_init();
    
    can_register_voltages(voltages);
    can_register_temps(temps);
    can_register_current(&current);
    can_register_SOC(&SOC);
    
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