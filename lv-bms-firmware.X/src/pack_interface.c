/* 
 * File:   pack_interface.c
 * Author: alexl
 * 
 * functions to interface and work with the BQ79616 in the conext of the lv-Battery
 *
 * Created on 06 June 2025, 14:28
 */

#include "pack_interface.h"
#include "millis.h"
#include "logging.h"
#include "batt_properties.h"

bool nfault_handler_enabled = false;
bool balancing_enabled = false;
bool balancing_running = false;
bool balancing_cells[CELL_COUNT] = {false}; 
timer_t balancing_update_wait_timer; // allows the balancing currents to stop before determining which cells to balance next

fault_summary_t fs; // has to be global for some reason breaks inside nfault handler

void pack_nfault_handler(void) {
    // avoid handling the interrupt if it's not ready to yet eg masks not configured correctly
    if (!nfault_handler_enabled) return;

    log_info("nfault triggered");
    fs = get_fault_summary(1);
   
    if(fs.bits.fault_pwr) {
        log_info("pwr fault");
        get_pwr_faults(1);
    }
    if(fs.bits.fault_sys) {
        log_info("sys fault");
        get_sys_faults(1);
    }
    if(fs.bits.fault_ovuv) {
        log_info("ovuv fault");
        get_ovuv_faults(1);
    }
    if(fs.bits.fault_otut) {
        log_info("otut fault");
        get_otut_faults(1);
    }
    if(fs.bits.fault_comm) {
        log_info("comm_fault");
        get_comm_faults(1);
    }
    if(fs.bits.fault_otp) {
        log_info("otp fault");
        get_otp_faults(1); // these aren't a worry - the otp isn't programmed   
    }
    if(fs.bits.fault_comp_adc) {
        log_info("adc_fault");
        get_comp_adc_faults(1);
    }
    if(fs.bits.fault_prot) {
        log_info("prot fault");
        get_prot_faults(1);
    }

    // clear faults
    reset_faults(1, MSK_ALL);
    
}

temp_t get_temp_from_voltage_ratio(uint16_t voltage_ratio) {
    // binary search the thermistor LUT
    temp_t lower = INT8_MIN;
    temp_t upper = INT8_MAX;
    
    while(lower != (upper-1)) {
        temp_t middle = lower + (upper - lower) / 2;
        if (voltage_ratio < therm_lut[middle+128]) {
            lower = middle;
        } else {
            upper = middle;
        }
    }
    
    return upper;
}

uint16_t get_voltage_ratio_from_temp(temp_t temp) {
    return therm_lut[temp];
}

void pack_wake() {
    Wake796XX();
}

void pack_shutdown() {
    nfault_handler_enabled = false; // clear interrupt handler as masks are incorrect
    SD796XX();
}

void pack_hw_reset() {
    nfault_handler_enabled = false; // clear interrupt handler as masks are incorrect
    HWRST796XX();
}

bool pack_check_connection() {
    int tries_left = BQ_COMM_TEST_TRIES;
    while (tries_left) {
        if(get_reg_value(1, PARTID) == 0x21) {
            break;
        }
        tries_left--;
    }
    
    return tries_left > 0;
}

bool pack_check_measuring() {
    bool running = true;
    running &= get_main_ADC_running(1);
    running &= get_aux_ADC_running(1);
//    running &= get_OTUT_running(1);
//    running &= get_OVUV_running(1);
    return running;
}

bool pack_setup() {
    nfault_handler_enabled = false;
    set_config(1, DEV_CONF_NO_ADJACENT_BALANCING | DEV_CONF_MULTIDROP_EN | DEV_CONF_NFAULT_EN);
    set_long_comm_timeout(1, TIMEOUT_2S, LONG_T_O_SHUTDOWN);
    set_active_cells(1, CELL_COUNT);

    // setup nfault interrupt 
    set_fault_msk(1, MSK_OTP_CRC | MSK_OTP_DATA);
    nfault_handler_enabled = true;
    // the ISR needs retriggering - there may be error states that haven't been handled
    pack_nfault_handler();

    // enable filtering
    enable_LPF_cells(1, BQ_VOLTAGE_LPF_FREQ);
    enable_LPF_BB(1, BQ_CURRENT_LPF_FREQ);
    // enable thermistors
    for(uint8_t gpio = 1; gpio <= THERM_COUNT; gpio++) {
        set_gpio_conf(1, gpio, GPIO_CONF_ADC_OTUT_INPUT);
    }
    enable_tsref(1);

    // start measurements
    delay(1);
    main_ADC_start(1);
    aux_ADC_start(1);
    
    return true;
}

// gathers the 3 voltages
void pack_get_voltages(voltage_t* voltages) {
    for(uint8_t cell = 1; cell <= CELL_COUNT; cell++) {
        int16_t voltage = get_cell_voltage(1, cell);
        voltage = (voltage >= 0) ? voltage : 0; // it can do -ve voltages, but seeing as the voltage_t type can't handle that, limiting  it to 0 makes the most sense
        voltages[cell-1] = (voltage_t)(voltage * V_LSB_ADC * VOLTAGE_MULTIPLIER);
    }
}

// gathers the 3 temps
void pack_get_temperatures(temp_t* temps) {
    int16_t tsref = get_tsref_voltage(1);
    
    for(uint8_t therm = 1; therm <= THERM_COUNT; therm++) {       
        int16_t v = get_gpio_voltage(1, therm);
        uint16_t voltage_ratio = (uint16_t)((v * V_LSB_GPIO * UINT16_MAX) / (tsref * V_LSB_TSREF));
        temps[therm-1] = get_temp_from_voltage_ratio(voltage_ratio);
    }
}

// gathers the current
void pack_get_current(current_t* current) {
    int16_t shunt_voltage = (int16_t)((get_BB_voltage(1) + CURRENT_OFFSET) * CURRENT_CAL_RATIO); // doesn't seem to quite work as expected in the datasheet, so linearly transform to adjust
    *current = (int16_t)(CURRENT_MULTIPLIER * (shunt_voltage * V_LSB_BB) / BB_CURRENT_SENSE_R);

}

// balancing
void update_balancing_timers(voltage_t* voltages) {
    
    // find minimum voltage of the pack
    voltage_t min_cell_voltage = V(5);
    for(uint8_t cell_i=0; cell_i < CELL_COUNT; cell_i++) {
        min_cell_voltage = min(min_cell_voltage, voltages[cell_i]);
    }
    
    // choose cells that need to be balanced
    for (uint8_t cell_i = 0; cell_i < CELL_COUNT; cell_i++) {
        if(voltages[cell_i] - min_cell_voltage > BALANCE_START_TH) {
            balancing_cells[cell_i] = true;
            balancing_running = true;
        }
        else if(voltages[cell_i] - min_cell_voltage < BALANCE_END_TH) {
            balancing_cells[cell_i] = false;
        }
    }
    
    if(balancing_running) {
        log_info("updating balancing");
        balancing_running = false;
        for (uint8_t cell_i = 0; cell_i < CELL_COUNT; cell_i++) { 
            if (balancing_cells[cell_i]) {
                set_balancing_timer(1, cell_i + 1, BAL_TIME_10S);
                balancing_running = true;
            } else {
                set_balancing_timer(1, cell_i + 1, BAL_TIME_STOP);
            }
            log_info("  cell %d: %d", cell_i+1, balancing_cells[cell_i]);
        }
        
        balancing_start(1);
    }
}

void pack_start_balancing(voltage_t* voltages) {
    log_info("starting balancing");
    timer_init_count_down(&balancing_update_wait_timer, BALANCE_UPDATE_WAIT_TIME);
    balancing_enabled = true;
    // enable auto balancing (switching between odd and even cells)
    enable_auto_balancing(1, BAL_DUTY_10S);
    // setup and start balancing with the update function
    update_balancing_timers(voltages);
}

void pack_stop_balancing(voltage_t* voltages) {
    log_info("stopping balancing");
    balancing_enabled = false;
    balancing_running = false;
    for(uint8_t cell_i = 0; cell_i < CELL_COUNT; cell_i++) {
        balancing_cells[cell_i] = false;
    }
    balancing_stop(1);
}

void pack_balancing_update(voltage_t* voltages) {
    if(!balancing_enabled) return;

    // always be looking to start balancing
    if (!balancing_running) {
        update_balancing_timers(voltages);
        log_info("starting balancing immediately");
    } else {
        // if one round of balancing has just finished, wait for the balancing current to go before re-evalutating the cell voltages
        if(get_balancing_done(1) && !timer_get_running(&balancing_update_wait_timer)) {
            timer_start(&balancing_update_wait_timer);
            log_info("balancing round finished, waiting");
        }
        
        // the timer has finished, start another round of balancing
        if(timer_get_done(&balancing_update_wait_timer)) {
            update_balancing_timers(voltages);
            timer_cancel(&balancing_update_wait_timer);
        }
    } 
}