import os
import json
import typing

from datetime import datetime

format_version = "1.0.0"

# paths
input_path = "sim_results"
output_path = "soc_LUT.h"
output_old_path = "soc_LUT.old"
output_template_path = "output.template"
settings_path = "parameters.json"

# load parameters
f_settings = open(settings_path, "r")
parameters = json.load(f_settings)
f_settings.close();

v_min = parameters["voltage_min"]
v_max = parameters["voltage_max"]

soc_scale = parameters["soc_scale"]
soc_base_type = parameters["soc_base_type"]

point_count_curr = parameters["point_count_current"]
point_count_temp = parameters["point_count_temperature"]

curr_ref = parameters["reference_current_amps"]
temp_ref = parameters["reference_temperature_deg"]

pretty_print = parameters["make_pretty"]
save_old = parameters["save_old"]

input_dir = os.path.join(os.path.dirname(__file__), input_path)
input_list = os.listdir(input_dir)

# if no input files, throw
if len(input_list) == 0 :
    raise FileNotFoundError("No folders found inside {input_dir}")

# if more than one folder, ask user to select
data_path = input_list[0]
if len(input_list) > 1 :
    i = 0
    print("Select input folder:")
    for path in input_list :
        print(f'[{i}] {path}')
        i = i + 1
    while True :
        try :
            data_path = input_list[int(input(" > "))]
        except : pass
        break

# data
# these will be saved into the output file

soc_vi = []
soc_vt = []
soc_curr_key = []
soc_temp_key = []
t_ref_index = None # 20 amps
i_ref_index = None # 25 deg C

trace_count_curr = 0
trace_count_temp = 0

def read_csv(file : typing.TextIO) :
    values_array = []
    header = ""
    for line in file.readlines() :
        # ignore comments
        if line.startswith("#") :
            continue
        # the first line will be the header
        if not header :
            header = line.split(",")
            for head in header :
                values_array.append([head])
            continue
        # convert values to float and append to data
        for j, value in enumerate(line.split(",")) :
            values_array[j].append(float(value))
    return values_array

dependent_param = ["Current","Temperature"]
voltage = "Voltage"
soc = "SOC"

print("Generating LUT...\nThis may take a while")

for dependent in dependent_param :

    print(f'Creating lookup table for "{dependent}"')

    csv_file_paths = os.path.join(input_dir, data_path, dependent.lower())
    csv_list = os.listdir(csv_file_paths)

    csv_list.sort()

    point_count = None
    
    if dependent == "Current" :
        point_count = point_count_curr
    if dependent == "Temperature" :
        point_count = point_count_temp

    for csv in csv_list :
        # we are only reading the pack data, not individual cell data
        if "pack" in csv :

            print(f'Reading {csv}')
            f_csv = open(os.path.join(csv_file_paths, csv), "r")
            csv_lines = read_csv(f_csv)
            f_csv.close()

            indep_index = None
            voltage_index = None
            soc_index = None

            for i in range(len(csv_lines)) :
                header = csv_lines[i][0]
                if dependent in header:
                    dep_index = i
                if voltage in header:
                    voltage_index = i
                if soc in header:
                    soc_index = i
            
            if dependent == "Current" :
                trace_count_curr += 1
                soc_vi.append([])
                soc_curr_key.append(int(round(-csv_lines[dep_index][-1])))
            if dependent == "Temperature" :
                trace_count_temp += 1
                soc_vt.append([])
                soc_temp_key.append(int(round(csv_lines[dep_index][-1])))

            # generate look up table

            sweep_begin = 2

            for i in range(point_count) :
                
                target = (i*v_min + (point_count - i - 1)*v_max)/(point_count - 1)

                monotonic = False
                prev_reading = -1
                turning_point_i = None

                closest = prev_reading
                closest_i = sweep_begin

                for j, reading in enumerate(csv_lines[voltage_index]) :
                    if j < sweep_begin : continue

                    if abs(target-closest) > abs(reading-target) :
                        closest = reading
                        closest_i = j
                    
                    if not monotonic :
                        if prev_reading > reading and turning_point_i == None :
                            turning_point_i = j
                    else :
                        # if monotonic, we can stop the sweep after a crossover
                        if reading > target :
                            break

                if turning_point_i == None :
                    sweep_begin = closest_i
                    monotonic = True
                else :
                    sweep_begin = turning_point_i

                soc_value = int(round(csv_lines[soc_index][closest_i]*soc_scale/100.0))

                if soc_value > soc_scale :
                    soc_value = soc_scale
                if soc_value < 0 :
                    soc_value = 0
            
                if dependent == "Current" :
                    soc_vi[trace_count_curr-1].append(soc_value)
                if dependent == "Temperature" :
                    soc_vt[trace_count_temp-1].append(soc_value)



# sort keys

for j in range(trace_count_curr - 1) :
    for i in range(trace_count_curr - 1 - j) :
        if soc_curr_key[i] > soc_curr_key[i+1] :
            # swap
            swap1 = soc_curr_key[i]
            swap2 = soc_vi[i]

            soc_curr_key[i] = soc_curr_key[i+1]
            soc_curr_key[i+1] = swap1

            soc_vi[i] = soc_vi[i+1]
            soc_vi[i+1] = swap2

for j in range(trace_count_temp - 1) :
    for i in range(trace_count_temp - 1 - j) :
        if soc_temp_key[i] > soc_temp_key[i+1] :
            # swap
            swap1 = soc_temp_key[i]
            swap2 = soc_vt[i]

            soc_temp_key[i] = soc_temp_key[i+1]
            soc_temp_key[i+1] = swap1

            soc_vt[i] = soc_vt[i+1]
            soc_vt[i+1] = swap2

# use keys to update reference index based on values provided in properties
# we can guess the reference based on the dataset, but taking from properties
# ensures correct user intent

for i, key in enumerate(soc_curr_key) :
    if key == curr_ref :
        i_ref_index = i
if i_ref_index == None :
    raise KeyError(f'SOC data for a current of {curr_ref}A does not exist.')

for i, key in enumerate(soc_temp_key) :
    if key == temp_ref :
        t_ref_index = i
if t_ref_index == None :
    raise KeyError(f'SOC data for a current of {temp_ref}A does not exist.')



print("Building output file...")

# read template file

f_template = open(output_template_path, "r")

template = f_template.readlines()

f_template.close()

def conv_c_array(json) :
    return json.replace("[","{").replace("]","}");

def format_compact(array) :
    return conv_c_array(json.dumps(array, separators=(',',':')))

def format_pretty_1D(array) :
    return conv_c_array(json.dumps(array)).replace("{", "{\n\t").replace("}", "\n}")

def format_pretty_2D(array, v_len) :
    arr_string = ""
    for a in array :
        separator_arr = ",\n" if arr_string else ""
        values_string = ""
        for value in a :
            separator_val = ", " if values_string else ""
            values_string = f'{values_string}{separator_val}{value:<{v_len}}'
        arr_string = f'{arr_string}{separator_arr}\t{{ {values_string} }}'
    arr_string = f'{{\n{arr_string}\n}}'

    return arr_string

if pretty_print :
    soc_vi_str = format_pretty_2D(soc_vi, len(str(soc_scale)))
    soc_vt_str = format_pretty_2D(soc_vt, len(str(soc_scale)))
    soc_curr_key_str = format_pretty_1D(soc_curr_key)
    soc_temp_key_str = format_pretty_1D(soc_temp_key)
else :
    soc_vi_str = format_compact(soc_vi)
    soc_vt_str = format_compact(soc_vt)
    soc_curr_key_str = format_compact(soc_curr_key)
    soc_temp_key_str = format_compact(soc_temp_key)


template_dict = {
    "VMAX" : str(float(v_max)),
    "VMIN" : str(float(v_min)),
    "I_REF_INDEX" : str(int(i_ref_index)),
    "T_REF_INDEX" : str((t_ref_index)),
    "I_POINT_COUNT" : str(int(point_count_curr)),
    "T_POINT_COUNT" : str(int(point_count_temp)),
    "I_TRACE_COUNT" : str(int(trace_count_curr)),
    "T_TRACE_COUNT" : str(int(trace_count_temp)),

    "SOC_SCALE" : str(int(soc_scale)),
    "SOC_TYPE" : str(soc_base_type),

    "SOC_VI" : soc_vi_str,
    "SOC_VT" : soc_vt_str,
    "SOC_I_KEY" : soc_curr_key_str,
    "SOC_T_KEY" : soc_temp_key_str,

    "SCRIPT_PATH" : __file__,
    "TIMESTAMP" : datetime.now().strftime('%m-%d-%Y %H:%M'),
    "SCRIPT_VERSION" : str(format_version),
    "DATA_SOURCE" : f'{input_path}/{data_path}'
}

for i, line in enumerate(template) :
    for key in template_dict :
        line = line.replace(f'${{{key}}}',template_dict[key])
    if "${" in line :
        missing_key = line.split("${")[1].split("}")[0]
        raise KeyError(
            f'File "{output_template_path}", line {i}\n{line}\nNo reference to ${{{missing_key}}}'
        )
    template[i] = line


if os.path.exists(output_path) :
    # if enabled, rename the old output file
    if save_old :
        if os.path.exists(output_old_path) :
            os.remove(output_old_path)
        os.rename(output_path, output_old_path)
        print(f'Created {output_old_path}')

f_output = open(output_path, "w")
f_output.writelines(template)
f_output.close()

print("Complete!")
print(f'Output saved as `{output_path}`')
print("Please make a backup of the old LUT table before replacing it with the new one.")
print("Lookup table is located in `lv-bms-firmware.X/inc/`")