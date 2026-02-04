#include "units.h"
#include "soc_LUT.h"

#define SOC_VMAX V(SOC_DSVMAX)
#define SOC_VMIN V(SOC_DSVMIN)

// convert soc[%] to state_of_charge_t
#define SOC(S) (state_of_charge_t)(S * (SOC_SCALE/100))

// convert state_of_charge_t into integer percentage
#define SOC_PERCENT(S) (uint8_t)(S / (SOC_SCALE/100))

/*
    State of charge in terms of pack voltage, current and temperature
    Returns an integer in the range 0 to `SOC_SCALE`
*/
state_of_charge_t state_of_charge(voltage_t V, current_t I, temp_t T);