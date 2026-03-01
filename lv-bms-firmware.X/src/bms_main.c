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

/*****************************************************************************
__        ___    ____  _   _ ___ _   _  ____ _
\ \      / / \  |  _ \| \ | |_ _| \ | |/ ___| |
 \ \ /\ / / _ \ | |_) |  \| || ||  \| | |  _| |
  \ V  V / ___ \|  _ <| |\  || || |\  | |_| |_|
   \_/\_/_/   \_\_| \_\_| \_|___|_| \_|\____(_)
 * cell and thermistor indexes are 1 based - beware when indexing the voltages
 * and temps arrays
******************************************************************************/

#include <xc.h>
#include "mcc.h"
#include "units.h"
#include "millis.h"
#include "batt_checks.h"
#include "error_load_store.h"
#include "indicator_lights.h"
#include "logging.h"
#include "pack_interface.h"
#include "can_interface.h"


#include "batt_properties.h" // properties for the battery

#define CRITICAL_WARNING_INTERVAL 64

#define LOG_CRITICAL_WARNINGS 0
#define LOG_SHUTDOWN 1

typedef enum { NONE, CUTOFF, LOCKOUT } cell_issue_level_t;

voltage_t voltages[CELL_COUNT];
temp_t temps[THERM_COUNT];
current_t current;
int8_t SOC = 50;

bool charging = false;

// timers to track time based changes
timer_t charge_start_timer;
timer_t charge_end_timer;
timer_t sleep_timer;


// typedef struct {
//     cell_issue_level_t issue_level;
//     shutdown_reason_t shutdown_reason;
//     lockout_reason_t lockout_reason;
// } cell_report;

// cell_report get_cell_report(uint8_t cell_index) {
//     voltage_t cell_v = voltages[cell_index];
//     temp_t cell_t = temps[cell_index];

//     cell_report r;

//     // function will return after an error is reached
//     // so it's important that lockouts are checked first
//     // as they are higher priority

//     if (cell_v > OVERVOLT_LOCKOUT) {
//         r.issue_level = LOCKOUT;
//         r.lockout_reason = LOCKOUT_OVERVOLT;
//         return r;
//     }
//     if (cell_v < UNDERVOLT_LOCKOUT) {
//         r.issue_level = LOCKOUT;
//         r.lockout_reason = LOCKOUT_UNDERVOLT;
//         return r;
//     }
//     if (cell_t > OVERTEMP_LOCKOUT) {
//         r.issue_level = LOCKOUT;
//         r.lockout_reason = LOCKOUT_OVERTEMP;
//         return r;
//     }

//     // check cutoff conditions

//     if (cell_v > CELL_VOLTAGE_MAX_CUTOFF) {
//         r.issue_level = CUTOFF;
//         r.shutdown_reason = SHUTDOWN_REASON_OVER_V;
//         return r;
//     }
//     if (cell_v < CELL_VOLTAGE_MIN_CUTOFF) {
//         r.issue_level = CUTOFF;
//         r.shutdown_reason = SHUTDOWN_REASON_UNDER_V;
//         return r;
//     }
//     if (cell_t > CELL_TEMP_MAX_CUTOFF) {
//         r.issue_level = CUTOFF;
//         r.shutdown_reason = SHUTDOWN_REASON_OVER_TEMP;
//         return r;
//     }
//     if (cell_t < CELL_TEMP_MIN_CUTOFF) {
//         r.issue_level = CUTOFF;
//         r.shutdown_reason = SHUTDOWN_REASON_UNDER_TEMP;
//         return r;
//     }

//     // if (mode == WAKEUP_CHARGE && current > CHARGE_CURRENT_MAX_CUTOFF) { // for charging
//     //     r.issue_level = CUTOFF;
//     //     r.shutdown_reason = SHUTDOWN_REASON_OVER_CURR;
//     //     return r;
//     // }
//     // if (mode == WAKEUP_DISCHARGE && current > DISCHARGE_CURRENT_MAX_CUTOFF) { // for discharging
//     //     r.issue_level = CUTOFF;
//     //     r.shutdown_reason = SHUTDOWN_REASON_OVER_CURR;
//     //     return r;
//     // }

//     // control flow reaches here if no errors

//     r.issue_level = NONE;
//     return r;
// }

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
void millis_hook(time_t uptime) {

}



// how long has the power button been pressed (capped at a maximum duration)
uint64_t power_button_press_duration(uint64_t max_duration) {
    static uint64_t press_start_time;
    static bool pressed, prev_pulse_unfinished;

    uint64_t press_len = 0;
    uint64_t now = millis();

    // main button checking loop
    if (MasterSwitch_GetValue()) { // pressed
        if (prev_pulse_unfinished) {
            // waiting for release to end the previous pulse
        }
        else if (!pressed) { // rising edge
            press_start_time = now;
            pressed = true;
            log_dbg("press start: %ums", press_start_time);
        }
        else { // press held
            // if we've reached the max duration end this press
            if (now - press_start_time > max_duration) {
                log_dbg("max duration of press reached: %u ms", max_duration);
                press_len = now - press_start_time;
                pressed = false;
                prev_pulse_unfinished = true;
            }
        }
    }
    else { // unpressed
        if (pressed) { // falling edge
            press_len = now - press_start_time;
            pressed = false;
            log_dbg("press done : %ums", now);
            log_dbg("   len: %u ms", press_len);
        }
        else { // default state
            prev_pulse_unfinished = false;
        }
    }

    if (press_len < 10 && press_len > 0) {
        log_dbg("short pulse found, debouncing it");
        press_len = 0;
    }

    return press_len;
}


void initialise_pack() {
    pack_hw_reset(); // TODO: double check the consequences of this - this seems to be needed for the communication to start back up correctly.
    pack_wake();
    delay(10);
    if (pack_check_connection() == false) {
        log_err("failed to communicate with BQ79616 BMS chip, locking out and going to sleep");
        hard_fault_handler(LOCKOUT_REASON_COMM_FAULT);
    }
    pack_setup();
}


bool check_critical_warnings() {
    static timer_t crit_msgs_timer; // tracks when to next check values and output critical warnings

    static timer_t  crit_OC_timer,
        crit_OV_timers[CELL_COUNT], crit_UV_timers[CELL_COUNT],
        crit_OT_timers[THERM_COUNT], crit_UT_timers[THERM_COUNT];

    // initialise and start timer if it's not already
    if (!crit_msgs_timer.configured) {
        timer_init_count_down(&crit_msgs_timer, CRITICAL_WARNING_INTERVAL);
        timer_start(&crit_msgs_timer);
    }

    bool warnings = false;
    if (timer_get_done(&crit_msgs_timer)) {
        log_dbg("Testing for critical values");
        uint8_t crit_cell = 0;
        // critical warning timers
        // check OC
        if ((crit_cell = check_condition_current(CRITICAL_OVER_CURRENT, current, &crit_OC_timer))) {
            if (LOG_CRITICAL_WARNINGS) log_warn("Critical Over Current: %fA", (float)current / CURRENT_MULTIPLIER);
            warnings = true;
            can_send_critical_warning(CAN_CRITICAL_CURRENT, 0, current);
        }
        // check OV
        if ((crit_cell = check_condition_voltage_per_cell(CRITICAL_OVER_VOLTAGE, voltages, crit_OV_timers, CELL_COUNT))) {
            if (LOG_CRITICAL_WARNINGS) log_warn("Critical Over Voltage: cell %d, %fV", crit_cell, (float)voltages[crit_cell - 1] / VOLTAGE_MULTIPLIER);
            warnings = true;
            can_send_critical_warning(CAN_CRITICAL_VOLTAGE, crit_cell, (int16_t)voltages[crit_cell - 1]);
        }
        // check UV 
        if ((crit_cell = check_condition_voltage_per_cell(CRITICAL_UNDER_VOLTAGE, voltages, crit_UV_timers, CELL_COUNT))) {
            if (LOG_CRITICAL_WARNINGS) log_warn("Critical Under Voltage: cell %d, %fV", crit_cell, (float)voltages[crit_cell - 1] / VOLTAGE_MULTIPLIER);
            warnings = true;
            can_send_critical_warning(CAN_CRITICAL_VOLTAGE, crit_cell, (int16_t)voltages[crit_cell - 1]);
        }
        // check OT
        temp_condition_t ot_cond = charging ? CRITICAL_CHARGING_OVER_TEMP : CRITICAL_DISCHARGING_OVER_TEMP;
        if ((crit_cell = check_condition_temp_per_cell(ot_cond, temps, crit_OT_timers, THERM_COUNT))) {
            if (LOG_CRITICAL_WARNINGS) log_warn("Critical Over Temp: cell %d, %f'C", crit_cell, (float)temps[crit_cell - 1] / TEMP_MULTIPLIER);
            warnings = true;
            can_send_critical_warning(CAN_CRITICAL_TEMP, crit_cell, temps[crit_cell - 1]);
        }
        // check UT
        temp_condition_t ut_cond = charging ? CRITICAL_CHARGING_UNDER_TEMP : CRITICAL_DISCHARGING_UNDER_TEMP;
        if ((crit_cell = check_condition_temp_per_cell(ut_cond, temps, crit_UT_timers, THERM_COUNT))) {
            if (LOG_CRITICAL_WARNINGS) log_warn("Critical Under Temp: cell %d, %f'C", crit_cell, (float)temps[crit_cell - 1] / TEMP_MULTIPLIER);
            warnings = true;
            can_send_critical_warning(CAN_CRITICAL_TEMP, crit_cell, temps[crit_cell - 1]);
        }

        timer_start(&crit_msgs_timer);
    }
    return warnings;
}

void enter_shutdown(shutdown_reason_t reason, uint8_t cell, int16_t fault_value) {
    log_info("Entering Shutdown, reason: %d", reason);
    save_shutdown_reason(reason, cell, fault_value);
    pack_shutdown();
    can_sensor_sending_enable(false);
    disp_set_number(0);
    RelayCtrl_SetLow();
    while (1) { // may lose power in this loop, if battery is power source
        // if it doesn't loose power (ie charging), then reset using the power button
        if (power_button_press_duration(UINT64_MAX)) {
            Reset();
        }
    }
}

void check_shutdown_conditions() {
    static timer_t  sd_OC_timer,
        sd_OV_timers[CELL_COUNT], sd_UV_timers[CELL_COUNT],
        sd_OT_timers[THERM_COUNT], sd_UT_timers[THERM_COUNT];

    log_dbg("Testing for shutdown conditions");
    uint8_t sd_cell = 0;
    // check OC
    if ((sd_cell = check_condition_current(SHUTDOWN_OVER_CURRENT, current, &sd_OC_timer))) {
        if (LOG_SHUTDOWN) log_err("Shutdown Over Current: %fA", (float)current / CURRENT_MULTIPLIER);
        enter_shutdown(SHUTDOWN_REASON_OVER_CURR, 0, current);
    }
    // check OV
    if ((sd_cell = check_condition_voltage_per_cell(SHUTDOWN_OVER_VOLTAGE, voltages, sd_OV_timers, CELL_COUNT))) {
        if (LOG_SHUTDOWN) log_err("Shutdown Over Voltage: cell %d, %fV", sd_cell, (float)voltages[sd_cell - 1] / VOLTAGE_MULTIPLIER);
        enter_shutdown(SHUTDOWN_REASON_OVER_V, sd_cell, (int16_t)voltages[sd_cell - 1]);
    }
    // check UV 
    if ((sd_cell = check_condition_voltage_per_cell(SHUTDOWN_UNDER_VOLTAGE, voltages, sd_UV_timers, CELL_COUNT))) {
        if (LOG_SHUTDOWN) log_err("Shutdown Under Voltage: cell %d, %fV", sd_cell, (float)voltages[sd_cell - 1] / VOLTAGE_MULTIPLIER);
        enter_shutdown(SHUTDOWN_REASON_UNDER_V, sd_cell, (int16_t)voltages[sd_cell - 1]);
    }
    // check OT
    temp_condition_t ot_cond = charging ? SHUTDOWN_CHARGING_OVER_TEMP : SHUTDOWN_DISCHARGING_OVER_TEMP;
    if ((sd_cell = check_condition_temp_per_cell(ot_cond, temps, sd_OT_timers, THERM_COUNT))) {
        if (LOG_SHUTDOWN) log_err("Shutdown Over Temp: cell %d, %f'C", sd_cell, (float)temps[sd_cell - 1] / TEMP_MULTIPLIER);
        enter_shutdown(SHUTDOWN_REASON_OVER_TEMP, sd_cell, temps[sd_cell - 1]);
    }
    // check UT
    temp_condition_t ut_cond = charging ? SHUTDOWN_CHARGING_UNDER_TEMP : SHUTDOWN_DISCHARGING_UNDER_TEMP;
    if ((sd_cell = check_condition_temp_per_cell(ut_cond, temps, sd_UT_timers, THERM_COUNT))) {
        if (LOG_SHUTDOWN) log_err("Shutdown Under Temp: cell %d, %f'C", sd_cell, (float)temps[sd_cell - 1] / TEMP_MULTIPLIER);
        enter_shutdown(SHUTDOWN_REASON_UNDER_TEMP, sd_cell, temps[sd_cell - 1]);
    }
}

void check_lockouts() {
    for (uint8_t cell_i = 0; cell_i < CELL_COUNT; cell_i++) {
        // check OV
        if (voltages[cell_i] > LOCKOUT_OVER_VOLTAGE) {
            hard_fault_handler_2(LOCKOUT_REASON_OVERVOLT, cell_i + 1, (int16_t)voltages[cell_i]);
        }
        // check UV 
        if (voltages[cell_i] < LOCKOUT_UNDER_VOLTAGE) {
            hard_fault_handler_2(LOCKOUT_REASON_UNDERVOLT, cell_i + 1, (int16_t)voltages[cell_i]);
        }
    }
    for (uint8_t therm_i = 0; therm_i < THERM_COUNT; therm_i++) {
        if (charging) {
            // check OT
            if (temps[therm_i] > LOCKOUT_CHARGING_OVER_TEMP) {
                hard_fault_handler_2(LOCKOUT_REASON_OVERTEMP, therm_i + 1, temps[therm_i]);
            }
            // check UT
            if (temps[therm_i] < LOCKOUT_CHARGING_UNDER_TEMP) {
                hard_fault_handler_2(LOCKOUT_REASON_UNDERTEMP, therm_i + 1, temps[therm_i]);
            }
        }
        else {
            // check OT
            if (temps[therm_i] > LOCKOUT_DISCHARGING_OVER_TEMP) {
                hard_fault_handler_2(LOCKOUT_REASON_OVERTEMP, therm_i + 1, temps[therm_i]);
            }
            // check UT
            if (temps[therm_i] < LOCKOUT_DISCHARGING_UNDER_TEMP) {
                hard_fault_handler_2(LOCKOUT_REASON_UNDERTEMP, therm_i + 1, temps[therm_i]);
            }
        }
    }
}

void start_charging() {
    charging = true;
    disp_set_charging(true);
    can_set_status(CAN_CHARGING);
    pack_start_balancing(voltages);
}

void stop_charging() {
    charging = false;
    disp_set_charging(false);
    can_set_status(CAN_DISCHARGING);
    pack_stop_balancing(voltages);
}

//======================================================================================================================
// ---------------------------------------------- Main functions -------------------------------------------------------
//======================================================================================================================
void locked_out_main() {
    lockout_reason_t lo_reason = load_lockout_reason();
    uint8_t lo_cell = load_lockout_cell();
    int16_t lo_value = load_lockout_value();

    log_info("locked out main, reason: %d, cell: %d, value: %d", lo_reason, lo_cell, lo_value);
    disp_set_critical(true);
    disp_set_soc(0); // enables animations
    can_set_status(CAN_LOCKED_OUT);
    can_set_lockdout(lo_reason, lo_cell, lo_value);
    can_sensor_sending_enable(true);

    initialise_pack();

    while (1) {
        // measure
        if(pack_check_connection() && pack_check_measuring()) {
            pack_get_voltages(voltages);
            pack_get_temperatures(temps);
            pack_get_current(&current);
        }
        
        // act
        can_update();
        disp_update();
        
        if (can_get_lockout_clear_message_rxed()) {
            save_lockout_reason(LOCKOUT_REASON_NONE, 0, 0);
            delay(1);
            RESET();
        }
    }
}

void sleep_main() {
    log_info("sleep main");
    disp_set_number(0);
    can_sensor_sending_enable(false);

    // low power mode config use doze 256 to run cpu at 1/256 speed
    CPUDOZEbits.DOZE = 0b111;
    // and run the millis ISR at full speed
    CPUDOZEbits.ROI = 1;

    // enter low power mode that:
    // - still checks power button
    // - periodically powers up for a split second to check for current

    uint64_t loop_count = 0;
    while (1) {
        CPUDOZEbits.DOZEN = 1;
        for(uint8_t i = 0; i < 100; i++) {
            // check for exiting sleep - skip the timer
            if (MasterSwitch_GetValue()) {
                i = 100;
            }
            
            delay(10);
        }
        CPUDOZEbits.DOZEN = 0;   

        // actually exit sleep
        if (MasterSwitch_GetValue()) {
            break;
        }

         // wake if there is current
        initialise_pack();
        pack_get_current(&current);
        log_dbg("sleep wakeup - checking for current: %f A", (float)current / CURRENT_MULTIPLIER);
        if(abs(current) > SLEEP_EXIT_CURRENT) {
            break;
        }
        pack_shutdown();

        log_info("sleeping");
        loop_count++;
    }
    
    log_info("exiting sleep");
}

void powered_on_main() {
    uint32_t loop_counter = 0;

    // power on setup
    log_info("powering on");

    // power up and check connection to BQ79616
    initialise_pack();

    disp_set_charging(false);
    disp_set_soc(70);
    can_set_status(CAN_DISCHARGING);
    can_sensor_sending_enable(true);

    //==================================================================================================================
    //  Main LOOP
    //==================================================================================================================
    while (1) {
        uint64_t now = millis();
        //---------------------------------------------------- MEASURE -------------------------------------------------
        // check connection
        if (loop_counter % 100 == 0) {
            if (pack_check_connection() == false) {
                log_err("failed to communicate with BQ79616 BMS chip, resetting and retrying");
                initialise_pack();
            }

            if (pack_check_measuring() == false) {
                log_err("a measurement system on the BQ79616 has failed, resetting and retrying");
                initialise_pack();
                if (pack_check_measuring() == false) {
                    log_err("a measurement system on the BQ79616 is still broken after reset, locking out and going to sleep");
                    hard_fault_handler(LOCKOUT_MEASURE_FAULT);
                }
            }
        }

        pack_get_voltages(voltages);
        pack_get_temperatures(temps);
        pack_get_current(&current);

        log_dbg("voltages: %d, %d, %d", voltages[0], voltages[1], voltages[2]);
        log_dbg("temps: %d, %d, %d", temps[0], temps[1], temps[2]);
        log_dbg("current: %d", current);

        //---------------------------------------------------- ANALYSE -------------------------------------------------
        // estimate current through the cells
        current_t relay_current = ((float)((voltages[0] + voltages[1] + voltages[2]) / VOLTAGE_MULTIPLIER) / RELAY_COIL_R) * CURRENT_MULTIPLIER;
        current_t cells_current = current - relay_current - IDLE_CURRENT;
        log_dbg("relay current: %fA, cell current: %fA", (float)relay_current / CURRENT_MULTIPLIER, (float)cells_current / CURRENT_MULTIPLIER);

        //---------------------------------------------------- ACT -----------------------------------------------------
        disp_set_soc(SOC);
        can_update();
        disp_update();
        if(charging) {
            pack_balancing_update(voltages);
        }
        
        // charge / discharge timers to tell if the pack is charging or discharging
        if(current < 0) {
            timer_cancel(&charge_start_timer);
            // stop charging
            if(charging) {
                if(timer_get_done(&charge_end_timer)) {
                   stop_charging(); 
                } else if(!timer_get_running(&charge_end_timer)) {
                    timer_start(&charge_end_timer);
                }
            }
        } else {
            timer_cancel(&charge_end_timer);
            // start charging
            if(!charging) {
                if(timer_get_done(&charge_start_timer)) {
                    start_charging();
                } else if(!timer_get_running(&charge_start_timer)) {
                    timer_start(&charge_start_timer);
                }
            }
        }
        
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

        check_critical_warnings();

        check_shutdown_conditions();

        check_lockouts();

        // check for sleep
        if (!charging && (abs(current) < SLEEP_CURRENT_THRESH)) {
            if (!timer_get_running(&sleep_timer)) {
                timer_start(&sleep_timer);
            }

            if (timer_get_done(&sleep_timer)) {
                break; // go to sleep
            }
        }
        else {
            timer_cancel(&sleep_timer);
        }

        // check for shutdown by button press
        if (power_button_press_duration(2000) >= 2000 || !OffButton_GetValue()) {
            log_info("Shutting down");
            enter_shutdown(SHUTDOWN_REASON_NONE, 0, 0);
        }

        loop_counter++;

        // time_t loop_duration = millis() - now;
        // log_warn("loop duration: %d ms", loop_duration);
    }

    // power off to go to sleep
    log_info("Going to sleep");

    can_set_status(CAN_POWERED_DOWN);
    can_sensor_sending_enable(false);
    pack_shutdown(); // disable bq to minimise power in sleep mode
}

/*
                         Main application
 */
void bms_main(void) {
    // Initialise the device
    SYSTEM_Initialize();

    // disable unused peripherals for power efficiency - change these if changing MCC configuration
    PMD0bits.CRCMD = 1;
    PMD1 = 0b11111110; // disable all timers but tmr0
    PMD2 = 0b00000011; // disable TU16
    PMD3 = 0b11100111; // disable all analogue stuff
    PMD4 = 0b01111111; // disable all waveform generators
    PMD5 = 0b11110111; // disable all pwm and capture compare
    PMD6 = 0b10101111; // disable UARTs 1,3,5, spi and i2c
    PMD7 = 0b11111111; // disable all clcs
    PMD8 = 0b11111110; // disable all DMA but channel 1;
    
    // setups
    millis_setup();
    IOCAF5_SetInterruptHandler(&pack_nfault_handler); // 
    INTERRUPT_GlobalInterruptEnable();
    disp_init();
    can_init();

    // timer setup
    timer_init_count_down(&charge_start_timer, CHARGING_STATE_CHANGE_TIME);
    timer_init_count_down(&charge_end_timer, CHARGING_STATE_CHANGE_TIME);
    timer_init_count_down(&sleep_timer, SLEEP_WAIT_TIME);

    // register can data
    can_register_voltages(voltages);
    can_register_temps(temps);
    can_register_current(&current);
    can_register_SOC(&SOC);
    can_set_shutdown_count(load_shutdown_count());
    can_set_lockout_count(load_lockout_count());
    can_set_charge_cycles(load_charge_cycles());

    log_info("bms powered up");

    // lockout takes precedence
    if (load_lockout_reason() != LOCKOUT_REASON_NONE) {
        locked_out_main();
    }

    RelayCtrl_SetHigh();

    // check if there was a shutdown reason
    shutdown_reason_t sd_reason = load_shutdown_reason();
    if (sd_reason != SHUTDOWN_REASON_NONE) {
        uint8_t sd_cell = load_shutdown_cell();
        int16_t sd_value = load_shutdown_value();

        log_info("Previous shutdown reason: %d - cell: %d - value: %d", sd_reason, sd_cell, sd_value);
        can_send_shutdown_reason(sd_reason, sd_cell, sd_value);
        save_shutdown_reason(SHUTDOWN_REASON_NONE, 0, 0);
    }

    while (1) {
        powered_on_main();
        sleep_main();
    }

}
/**
 End of File
*/