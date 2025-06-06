# generates the thermistor LUT for the LVBMS firmware - am9g22@soton.ac.uk
from math import log, exp # natural log
import datetime
from pathlib import Path
import re

# parameters of the circuit
therm_r25 = 10e3
therm_beta = 3380
pull_up_r = 10e3

# relative file paths
therm_lut_path = "lv-bms-firmware.X/inc/therm_LUT.h"
units_path = "lv-bms-firmware.X/inc/units.h"

# get file constants
root_folder = Path(__file__).parent
print(f"working in root folder: {root_folder}")


# generate lut
def voltage_ratio_from_temp(temp) -> int:
	R_t = therm_r25 * exp(therm_beta * (1/(temp+273) - 1/(25+273)))
	return (R_t/ (R_t + pull_up_r))

temp_range = range(-128, 128)
lut = {}
for temp in temp_range:
	lut[temp] = voltage_ratio_from_temp(temp)


# write file
lut_file_text = f"""
// auto generated on {datetime.datetime.now()} by {__file__}
// therm_r25 = {therm_r25}
// therm_beta = {therm_beta}
// pull_up_r = {pull_up_r}

#include "units.h"

// LUT converts temperatures to the voltage ratio between the thermistor and TSREF
uint16_t therm_lut[] = {{
"""

for temp, voltage_ratio in lut.items():
	# lut_file_text += f"{voltage_ratio:06f} * UINT16_MAX, // {temp}'C\n"
	lut_file_text += f"{round(voltage_ratio * 0xffff)}, // {temp}'C\n"

lut_file_text += "};"

lut_file = (root_folder / therm_lut_path).open("w") # this file's location in the repo is important
lut_file.write(lut_file_text)
lut_file.close()
