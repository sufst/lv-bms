/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BATT_PROPERTIES_H
#define	BATT_PROPERTIES_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "units.h"

#define DISCHARGE_CURRENT_MAX A(25) // TODO: replace with actual values
#define CHARGE_CURRENT_MAX A(25) // TODO: replace with actual values
#define CELL_VOLTAGE_MAX V(4.3) // TODO: replace with actual values
#define CELL_VOLTAGE_MIN V(3.2) // TODO: replace with actual values
#define CELL_TEMP_MAX 60 // TODO: replace with actual values
#define CELL_TEMP_MIN 0 // TODO: replace with actual values

#define BALANCE_START_TH V(0.2)
#define BALANCE_END_TH V(0.05)

/**
 * checks if a cell isn't over-volted
 * @param cell_voltage
 * @return true if over-volted else false
 */
bool check_over_volt(voltage_t cell_voltage);

/**
 * checks if a cell isn't under-volted
 * @param cell_voltage
 * @return true if the cell is under-volted else false
 */
bool check_under_volt(voltage_t cell_voltage);

/**
 * checks if the cell is overheated
 * @param cell_temp
 * @return true if overheated false otherwise
 */
bool check_over_temp(temp_t cell_temp);

/**
 * checks if a cell is too cold
 * @param cell_temp
 * @return true if too cold else false
 */
bool check_under_temp(temp_t cell_temp);

/**
 * checks if the a cell current is a reasonable charging rate 
 * should be +ve and magnitude less than the CHARGE_CURRENT_MAX
 * @param cell_curr
 * @return true if too much charge current false otherwise
 */
bool check_over_current_charge(current_t cell_curr);

/**
 * checks if the cell current is a reasonable discharging rate
 * should be -ve and magnitude smaller than DISCHARGE_CURRENT_MAX
 * @param cell_curr
 * @return true if too much discharge current else false
 */
bool check_over_current_discharge(current_t cell_curr);

/**
 * returns the cell's state of charge from the measured cell voltage
 * @param cell_voltage
 * @return 
 */
uint8_t get_SOC(voltage_t cell_voltage);


#endif	/* BATT_PROPERTIES_H */

