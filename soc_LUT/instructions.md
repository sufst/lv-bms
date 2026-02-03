
## To update LUT with new data
 - Run `update_lookup_table.py`
 - Select dataset folder from list
 - Make a cuppa tea (optional)
 - When complete, open the output file `soc_lookup.h` and verify that there are no obvious errors
 - Make a backup of the old LUT
 - Copy the new LUT into `lv-mbs-firmware.X/inc/`


## To generate new data

Recommended to generate .csv files from:
    https://voltt.aboutenergy.io/simulate

### Battery Properties:
    Cell:           Molicel P45B
    Datasheet:      https://voltt.aboutenergy.io/cell-library/Molicel_INR-21700-
    Configuration:  3s-4p

### Simulation:
 - Simulate a current sweep at constant temperature
 - Simulate a temperature sweep at constant current

 - To ensure constant temperature during current sweep, set `Heat Transfer Coefficient [W/m^2 K] = 10000`

 - A "sweep" should be a set of simulations at incremental current/temperature steps

 - Put the current sweeps inside ./sim_results/<identifier>/current/
 - Put the temperature sweeps inside ./sim_results/<identifier>/temperature/

 - A .csv file must contain "pack" in its filename to be included in the LUT

### Parameters: "parameters.json"

###### `voltage_min`

The lowest pack voltage to include in LUT. This should be ~0% SOC

###### `voltage_max`

The highest pack voltage to include in LUT. This should be ~100% SOC

###### `soc_scale`

The integer value corresponding to 100% SOC
    
###### `soc_base_type`

Datatype of `state_of_charge_t`

###### `point_count_current`

Number of LUT sample points per current sweep
 
###### `point_count_temperature`
 
Number of LUT sample points per current sweep

###### `reference_temperature_deg`
    
The temperature the pack was held at during during current sweep. This temperature MUST also be included in the temperature sweep

###### `reference_current_amps`

The current held during temperature sweep. This current MUST also be included in the current sweep

###### `make_pretty`

Set to `true` for nicely formatted arrays
Otherwise use a more compact format

###### `save_old`

If set to `true`, any existing already existing output will be saved as output.old

## Maintenance

    Authored by T M Lansdown
    Contact: tml1g25@soton.ac.uk
    (Feel free to add your contact here if you contributed)

### Format version
If you significantly modify the source code (including the template file), please change the format version appropriately (located at the top of update_lookup_table.py)
