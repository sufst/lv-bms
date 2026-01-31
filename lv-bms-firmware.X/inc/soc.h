#include "units.h"
#include "soc_lookup.h"

#define SOC_VMAX V(SOC_DSVMAX)
#define SOC_VMIN V(SOC_DSVMIN)

/*
    State of charge in terms of pack voltage, current and temperature
    Returns an integer in the range 0 to `SOC_SCALE`
*/
state_of_charge_t state_of_charge(voltage_t V, current_t I, temp_t T);