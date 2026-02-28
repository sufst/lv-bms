/* 
 * File:   pack_interface.h
 * Author: alexl
 * 
 * Defines the functions to interface and work with the BQ79616 in the context of the lv-Battery
 *
 * Created on 06 June 2025, 14:28
 */

#ifndef BQ_INTERFACE_H
#define	BQ_INTERFACE_H

#include "bq796xx.h"
#include "therm_LUT.h"
#include "units.h"

#define BQ_COMM_TEST_TRIES 5
#define BQ_VOLTAGE_LPF_FREQ LPF_6_5Hz
#define BQ_CURRENT_LPF_FREQ LPF_6_5Hz

#define BB_CURRENT_SENSE_R 4e-3 // ohms
// derived by testing
#define CURRENT_OFFSET -41 // in stepssteps; * V_LSB_BB is volts
#define CURRENT_CAL_RATIO  0.85 // ratio multiplied to make it correct

void pack_nfault_handler();
void pack_wake();
void pack_shutdown();
void pack_hw_reset();

bool pack_check_connection();
bool pack_check_measuring();
bool pack_setup();

// gathers the 3 voltages
void pack_get_voltages(voltage_t* voltages);

// gathers the 3 temps
void pack_get_temperatures(temp_t* temps);

// gathers the current
void pack_get_current(current_t* current);

// balancing
void pack_start_balancing(voltage_t* voltages);
void pack_stop_balancing(voltage_t* voltages);
void pack_balancing_update(voltage_t* voltages);

#endif	/* BQ_INTERFACE_H */

