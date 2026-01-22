/* 
 * File:   bq_interface.c
 * Author: alexl
 * 
 * functions to interface and work with the BQ79616 in the conext of the lv-Battery
 *
 * Created on 06 June 2025, 14:28
 */

#include "bq_interface.h"
#include "millis.h"
#include "logging.h"

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

void bq_wake() {
    Wake796XX();
}

void bq_shutdown() {
    SD796XX();
}

void bq_hw_reset() {
    HWRST796XX();
}

bool bq_check_connection() {
    int tries_left = BQ_COMM_TEST_TRIES;
    while (tries_left) {
        if(get_reg_value(1, PARTID) == 0x21) {
            break;
        }
        tries_left--;
    }
    
    return tries_left > 0;
}

bool bq_check_measuring() {
    bool running = true;
    running &= get_main_ADC_running(1);
    running &= get_aux_ADC_running(1);
//    running &= get_OTUT_running(1);
//    running &= get_OVUV_running(1);
    return running;
}

bool bq_setup() {
    set_config(1, DEV_CONF_NO_ADJACENT_BALANCING | DEV_CONF_MULTIDROP_EN | DEV_CONF_NFAULT_EN);
    set_long_comm_timeout(1, TIMEOUT_2S, LONG_T_O_SHUTDOWN);
    set_active_cells(1, 3);
    enable_LPF_cells(1, BQ_VOLTAGE_LPF_FREQ);
    enable_LPF_BB(1, BQ_CURRENT_LPF_FREQ);
    for(uint8_t gpio = 1; gpio <= 3; gpio++) {
        set_gpio_conf(1, gpio, GPIO_CONF_ADC_OTUT_INPUT);
    }
    enable_tsref(1);
    delay(1);
    main_ADC_start(1);
    aux_ADC_start(1);
    
    return true;
}

// gathers the 3 voltages
void bq_get_voltages(voltage_t* voltages) {
    for(uint8_t cell = 1; cell <= 3; cell++) {
        int16_t voltage = (voltage_t)(get_cell_voltage(1, cell) * V_LSB_ADC * VOLTAGE_MULTIPLIER);
        voltages[cell-1] = (voltage >= 0) ? voltage : 0; // it can do -ve voltages, but seeing as the voltage_t type can't handle that, limiting  it to 0 makes the most sense
    }
}

// gathers the 3 temps
void bq_get_temperatures(temp_t* temps) {
    int16_t tsref = get_tsref_voltage(1);
    
    for(uint8_t therm = 1; therm <= 3; therm++) {       
        int16_t v = get_gpio_voltage(1, therm);
        uint16_t voltage_ratio = (uint16_t)((v * V_LSB_GPIO * UINT16_MAX) / (tsref * V_LSB_TSREF));
        temps[therm-1] = get_temp_from_voltage_ratio(voltage_ratio);
    }
}

// gathers the current
void bq_get_current(current_t* current) {
    int16_t shunt_voltage = (get_BB_voltage(1) + CURRENT_OFFSET) * CURRENT_CAL_RATIO; // doesn't seem to quite work as expected in the datasheet, so linearly transform to adjust
    *current = CURRENT_MULTIPLIER * (shunt_voltage * V_LSB_BB) / BB_CURRENT_SENSE_R;

}