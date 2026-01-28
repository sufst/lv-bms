

#include <xc.h>
#include "stdbool.h"
#include "batt_checks.h"
#include "batt_properties.h"
#include "millis.h"
#include "logging.h"

bool _check_condition_for_time(timer_t* timer, bool trigger_condition, time_t trigger_time) {
    if (!timer->configured) {
        timer_init_count_down(timer, trigger_time);
    }

    if (trigger_condition) {
        if(!timer->running && !timer->done) {
            timer_start(timer);   
            timer->duration = trigger_time;
        } 
    }
    else {
        timer_cancel(timer);
    }

    return timer_get_done(timer);
}


uint8_t check_condition_voltage(voltage_condition_t cond, voltage_t value, timer_t *timer) {
    bool comparison_result = false;
    if(cond.bound == BOUND_UPPER) {
        comparison_result = value > cond.value;
    } else {
        comparison_result = value < cond.value;
    }
    return _check_condition_for_time(timer, comparison_result, cond.duration);
}

uint8_t check_condition_current(current_condition_t cond, current_t value, timer_t *timer) {
    bool comparison_result = false;
    if(cond.bound == BOUND_UPPER) {
        comparison_result = abs(value) > cond.value;
    } else {
        comparison_result = abs(value) < cond.value;
    }
    return _check_condition_for_time(timer, comparison_result, cond.duration);
}

uint8_t check_condition_temp(temp_condition_t cond, temp_t value, timer_t *timer) {
    bool comparison_result = false;
    if(cond.bound == BOUND_UPPER) {
        comparison_result = value > cond.value;
    } else {
        comparison_result = value < cond.value;
    }
    return _check_condition_for_time(timer, comparison_result, cond.duration);
}


uint8_t check_condition_voltage_per_cell(voltage_condition_t cond, voltage_t *values, timer_t *timers, uint8_t n_values) {
    for(uint8_t cell_i = 0; cell_i < n_values; cell_i++) {
        if(check_condition_voltage(cond, values[cell_i], &timers[cell_i])) {
            return cell_i+1;
        }
    }
    return 0;
}

uint8_t check_condition_current_per_cell(current_condition_t cond, current_t *values, timer_t *timers, uint8_t n_values) {
    for(uint8_t cell_i = 0; cell_i < n_values; cell_i++) {
        if(check_condition_current(cond, values[cell_i], &timers[cell_i])) {
            return cell_i+1;
        }
    }
    return 0;
}

uint8_t check_condition_temp_per_cell(temp_condition_t cond, temp_t *values, timer_t *timers, uint8_t n_values) {
    for(uint8_t cell_i = 0; cell_i < n_values; cell_i++) {
        if(check_condition_temp(cond, values[cell_i], &timers[cell_i])) {
            return cell_i+1;
        }
    }
    return 0;
}
