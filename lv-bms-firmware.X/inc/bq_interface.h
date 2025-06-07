/* 
 * File:   bq_interface.h
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
#define BB_CURRENT_SENSE_R 2e-3 // ohms


void bq_wake();
void bq_shutdown();
void bq_hw_reset();

bool bq_check_connection();
bool bq_check_measuring();
bool bq_setup();

// gathers the 3 voltages
void bq_get_voltages(voltage_t* voltages);

// gathers the 3 temps
void bq_get_temperatures(temp_t* temps);

// gathers the current
void bq_get_current(current_t* current);

#endif	/* BQ_INTERFACE_H */

