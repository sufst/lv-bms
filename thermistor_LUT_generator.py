# generates the thermistor LUT for the TEM firmware - am9g22@soton.ac.uk
from math import log # natural log
import datetime
from pathlib import Path


therm_r25 = 10e3
therm_beta = 3380

adc_res = 12
vcc = 3.3
pull_up_r = 10e3

# generate lut
temp_from_R_t = lambda R_t: 1/((1/(25+271)) + (1/therm_beta)*log(R_t/therm_r25 + 0.0000000001))

lut_len = 2**adc_res
lut = []
for adc_reading in range(lut_len):
	read_voltage = vcc*adc_reading/lut_len
	therm_r = therm_r25 * read_voltage / (vcc - read_voltage)
	lut.append(temp_from_R_t(therm_r)-271)

# write file
lut_file_text = f"""
// auto generated on {datetime.datetime.now()} by {__file__}
// therm_r25 = {therm_r25}
// therm_beta = {therm_beta}
// 
// adc_res = {adc_res}
// vcc = {vcc}
// pull_up_r = {pull_up_r}

int8_t therm_lut[] = {{
"""

for lut_entry in lut:
	if -128 <= lut_entry <= 127:
		lut_file_text += f"{round(lut_entry)},\n"
	else:
		clipped = min(127, max(-128, lut_entry))
		lut_file_text += f"{round(clipped)}, // clipped to one end by int8 size\n"

lut_file_text += "};"

print(Path(__file__).parent)
lut_file = (Path(__file__).parent / "lv-bms-firmware.X/therm_LUT.h").open("w") # this file's location in the repo is important
lut_file.write(lut_file_text)
lut_file.close()
