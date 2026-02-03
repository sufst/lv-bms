
#include "soc.h"

// return SOC assuming a constant temperature
state_of_charge_t soc_vi_const_temp(voltage_t V, current_t I) {
    uint16_t V_index;
    if (V < SOC_VMIN)
        V_index = SOC_CURR_POINT_COUNT - 1;
    else if (V >= SOC_VMAX)
        V_index = 0;
    else
        V_index = ((V-SOC_VMAX)*(SOC_CURR_POINT_COUNT-1))/(SOC_VMIN-SOC_VMAX);
    
    // get the nearest two current traces
    uint16_t I_index;
    for (I_index = 0; I_index < SOC_CURR_TRACE_COUNT; I_index++) {
        // break at cross-over
        if (A(soc_current_key[I_index]) > I)
            break;
    }
    // if current is lower/higher than trace range, we can't interpolate
    // so instead return directly from the closest trace
    if (I_index == 0) return soc_vi[0][V_index];
    if (I_index == SOC_CURR_TRACE_COUNT) return soc_vi[SOC_CURR_TRACE_COUNT - 1][V_index];

    // interpolate

    state_of_charge_t soc1, soc2;
    current_t i1, i2;
    i1 = A(soc_current_key[I_index-1]); // upper trace
    i2 = A(soc_current_key[I_index]); // lower trace
    soc1 = soc_vi[I_index-1][V_index]; // soc for upper trace 
    soc2 = soc_vi[I_index][V_index]; // soc for lower trace

    state_of_charge_t soc_interp = ( ((I-i2) * soc1) + ((i1-I) * soc2) ) / (i1 - i2);
    return soc_interp;
}

// transform SOC based on how the reference temperature compares to the actual temperature
state_of_charge_t soc_temperature_correct(state_of_charge_t soc_ref, temp_t T) {

    // find closest SOC at reference temperature
    // don't assume that the V(SOC) is monotonic

    uint16_t closest_i = 0;
    uint16_t closest_difference = 65535; // (uint16_t)-1

    for (uint16_t i = 1; i < SOC_TEMP_POINT_COUNT; i++) {
        state_of_charge_t new_value = soc_vt[SOC_TEMP_REF_INDEX][i];
        uint16_t new_difference;
        if (new_value < soc_ref)
            new_difference = soc_ref - new_value;
        else
            new_difference = new_value - soc_ref;
        
        if (new_difference < closest_difference) {
            closest_difference = new_difference;
            closest_i = i;
        }
        if (closest_difference == 0) break;
    }
    uint16_t V_index = closest_i;

    // get the nearest two temperature traces
    uint16_t T_index;
    for (T_index = 0; T_index < SOC_TEMP_TRACE_COUNT; T_index++) {
        // break at cross-over
        if (C(soc_temperature_key[T_index]) > T)
            break;
    }

    if (T_index == 0) return soc_vt[0][V_index];
    if (T_index == SOC_TEMP_TRACE_COUNT) return soc_vi[SOC_TEMP_TRACE_COUNT - 1][V_index];

    // interpolate

    state_of_charge_t soc1, soc2;
    temp_t t1, t2;

    t1 = C(soc_temperature_key[T_index-1]); // upper trace
    t2 = C(soc_temperature_key[T_index]); // lower trace
    soc1 = soc_vt[T_index-1][V_index]; // soc for upper trace 
    soc2 = soc_vt[T_index][V_index]; // soc for lower trace

    state_of_charge_t soc_interp = ( ((T-t2) * soc1) + ((t1-T) * soc2) ) / (t1 - t2);

    return soc_interp;
}

state_of_charge_t state_of_charge(voltage_t V, current_t I, temp_t T) {
    return soc_temperature_correct(soc_vi_const_temp(V, I), T);
}