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

/*
    Translate the SOC such that 0% reported SOC is the cutoff SOC
    Cutoff SOC is defined as SOC_CUTOFF
*/
state_of_charge_t reported_soc(state_of_charge_t measured_soc);