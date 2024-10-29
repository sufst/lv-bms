

#include <xc.h>
#include "stdbool.h"
#include "batt_properties.h"

bool check_over_volt(voltage_t cell_voltage) {
    if (cell_voltage > CELL_TEMP_MAX){
        return true;
    }
    return false;
}

bool check_under_volt(voltage_t cell_voltage) {
    if (cell_voltage < CELL_VOLTAGE_MIN){
        return true;
    }
    return false;
}

bool check_over_temp(temp_t cell_temp) {
    if (cell_temp > CELL_TEMP_MAX){
        return true;
    }
    return false;
}

bool check_under_temp(temp_t cell_temp) {
    if (cell_temp < CELL_TEMP_MIN){
        return true;
    }
    return false;
}

bool check_over_current_charge(current_t cell_curr) {
    if (cell_curr < 0 || abs(cell_curr) > CHARGE_CURRENT_MAX){
        return true;
    }
    return false;
}

bool check_over_current_discharge(current_t cell_curr) {
    if (cell_curr > 0 || abs(cell_curr) > DISCHARGE_CURRENT_MAX){
        return true;
    }
    
    return false;
}

uint8_t get_SOC(voltage_t cell_voltage) {
    // TODO cell discharge curve LUT (or approximation thereof) here please
}
