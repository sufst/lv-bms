

#include <xc.h>
#include "stdbool.h"
#include "batt_checks.h"
#include "batt_properties.h"
#include "millis.h"

bool _check_condition_for_time(timer_t* timer, bool trigger_condition, time_t trigger_time) {
    if (!timer->configured) {
        timer_init_count_down(timer, trigger_time);
    }

    if (trigger_condition) {
        timer->duration = trigger_time;
        timer_start(timer);
    }
    else {
        timer_cancel(timer);
    }

    return timer_get_done(timer);
}


uint8_t check_condition(condition_t cond, unit_value_u value, timer_t *timer) {
    bool comparison_result = false;
    switch (cond.unit)
    {
    case UNIT_VOLTAGE:
        if(cond.bound == BOUND_UPPER) {
            comparison_result = value.v_value > cond.v.v_value;
        } else {
            comparison_result = value.v_value < cond.v.v_value;
        }
        break;
    case UNIT_CURRENT:
        if(cond.bound == BOUND_UPPER) {
            comparison_result = value.c_value > cond.v.c_value;
        } else {
            comparison_result = value.c_value < cond.v.c_value;
        }
    case UNIT_TEMP:
        if(cond.bound == BOUND_UPPER) {
            comparison_result = value.t_value > cond.v.t_value;
        } else {
            comparison_result = value.t_value < cond.v.t_value;
        }
    default:
        break;
    }
    return _check_condition_for_time(timer, comparison_result, cond.duration);
}

uint8_t check_condition_per_cell(condition_t cond, unit_value_u *values, timer_t *timers, uint8_t n_values) {
    for(uint8_t cell_i = 0; cell_i < n_values; cell_i++) {
        if(check_condition(cond, values[cell_i], &timers[cell_i])) {
            return cell_i;
        }
    }
    return 0;
}
