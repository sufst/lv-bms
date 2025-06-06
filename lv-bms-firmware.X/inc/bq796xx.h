/*
 *  @file bq796xx.h
 *
 *  @author Vince Toledo - Texas Instruments Inc.
 *  @date February 2020
 *  @version 1.2
 *  @note Built with CCS for Hercules Version: 8.1.0.00011
 *  @note Built for TMS570LS1224 (LAUNCH XL2)
 */

/*****************************************************************************
**
**  Copyright (c) 2011-2019 Texas Instruments
**
******************************************************************************/

/*****************************************************************************
__        ___    ____  _   _ ___ _   _  ____ _  
\ \      / / \  |  _ \| \ | |_ _| \ | |/ ___| | 
 \ \ /\ / / _ \ | |_) |  \| || ||  \| | |  _| | 
  \ V  V / ___ \|  _ <| |\  || || |\  | |_| |_| 
   \_/\_/_/   \_\_| \_\_| \_|___|_| \_|\____(_) 
 * cell and thermistor indexes are 1 based
******************************************************************************/


#ifndef BQ796xx_H_
#define BQ796xx_H_

#include "string.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
//****************************************************
// ***Register defines, choose one of the following***
// ***based on your device silicon revision:       ***
//****************************************************
#include "BQ796XXA0_reg.h"

#include "bq796xx_enums.h"
#include "bq796xx_fault_types.h"

#define TRUE 1
#define FALSE 0
#define UART_COMM     TRUE
#define TOTALBOARDS 1       //boards in stack
#define V_LSB_ADC       190.73f*0.000001 // V/LSB
#define V_LSB_BB         30.52f*0.000001 // V/LSB
#define V_LSB_GPIO      152.59f*0.000001 // V/LSB
#define V_LSB_TSREF     169.54f*0.000001 // V/LSB
#define V_LSB_DIAG      152.59f*0.000001 // V/LSB
#define V_LSB_AUX_BAT     3.05f*0.001    // V/LSB
#define V_LSB_DIETEMP 0.025 // *C/LSB

#define MAXBYTES (16*2)     //maximum number of bytes to be read from the devices (for array creation)
#define BAUDRATE 1000000    //device + uC baudrate

// debug logging - split into levels
typedef enum {BQ_LOG_ERR, BQ_LOG_WARN, BQ_LOG_INFO, BQ_LOG_DBG, BQ_LOG_REG} bq796xx_log_level_t;
//bq796xx_log_level_t bq796xx_log_level = BQ_LOG_WARN;
bq796xx_log_level_t bq796xx_log_level = BQ_LOG_WARN; 

// power state control
void Wake796XX(void);
void StA796XX(void);        // sleep to active
void SD796XX(void);     // shutdown
void HWRST796XX(void);  // hardware reset

// generic config - datasheet (P128 - DEV_CONF)
void set_config(uint8_t bID, dev_conf_t conf);
void set_active_cells(uint8_t bID, uint8_t cell_count);
void set_bb_loc(uint8_t bID, uint8_t loc); // BBP_LOC register - datasheet P130

// communication 
void COM_CLR_796XX(void);
int AutoAddress(COMM_DIR_t dir); // returns the number of devices
void ReverseAddressing();
void enable_daisy_chain(bool en);
void daisy_chain_dll_sync(); // needed before using the daisy chain (datasheet p69)
void set_long_comm_timeout(uint8_t bID, TIMEOUT_t time, LONG_TIMEOUT_ACTION_t action);

// OTP programming


// register interfacing
uint8_t get_reg_value(uint8_t bID, uint16_t addr);
void set_reg_value(uint8_t bID, uint16_t addr, uint8_t data);

//  utilities
uint16_t CRC16(uint8_t *pBuf, int nLen);

// fault handing
bool GetFaultStat();
void set_fault_msk(uint8_t bID, FAULT_MASK_t mask);
void reset_faults(uint8_t bID, FAULT_MASK_t mask);

fault_summary_t get_fault_summary(uint8_t bID);         
pwr_faults_t get_pwr_faults(uint8_t bID);               
sys_faults_t get_sys_faults(uint8_t bID);               
ovuv_faults_t get_ovuv_faults(uint8_t bID);             
otut_faults_t get_otut_faults(uint8_t bID);             
comm_faults_t get_comm_faults(uint8_t bID);             
otp_faults_t get_otp_faults(uint8_t bID);               
comp_adc_faults_t get_comp_adc_faults(uint8_t bID);     
prot_fault_t get_prot_faults(uint8_t bID);              

int snprint_fault_summary(char * s, size_t n, fault_summary_t fs);      // len ~~ 85 chars
int snprint_pwr_faults(char * s, size_t n, pwr_faults_t pf);            // len ~~ 205 chars
int snprint_sys_faults(char * s, size_t n, sys_faults_t sf);            // len ~~ 70 chars
int snprint_ovuv_faults(char * s, size_t n, ovuv_faults_t vf);          // len ~~ 255 chars
int snprint_otut_faults(char * s, size_t n, otut_faults_t tf);          // len ~~ 130 chars
int snprint_comm_faults(char * s, size_t n, comm_faults_t cf);          // len ~~ 210 chars
int snprint_otp_faults(char * s, size_t n, otp_faults_t of);            // len ~~ 100 chars
int snprint_adc_comp_faults(char * s, size_t n, comp_adc_faults_t cf);  // len ~~ 780 chars
int snprint_prot_faults(char * s, size_t n, prot_fault_t pf);           // len ~~ 150 chars

// gpio
void set_gpio_conf(uint8_t bID, uint8_t gpioNum, gpio_conf_t conf);
gpio_conf_t get_gpio_conf(uint8_t bID, uint8_t gpioNum);

// thermistor config
void enable_tsref(uint8_t bID);
void disable_tsref(uint8_t bID);

// ADC control - always start it AFTER configuring everything
void main_ADC_start(uint8_t bID); 
void main_ADC_run_once(uint8_t bID);
void main_ADC_stop(uint8_t bID);
void aux_ADC_start(uint8_t bID);
void aux_ADC_run_once(uint8_t bID);
void aux_ADC_stop(uint8_t bID);
bool get_main_ADC_running(uint8_t bID); 
bool get_main_ADC_RR_complete(uint8_t bID); // has the main ADC read each channel at least once
bool get_aux_ADC_running(uint8_t bID); 
bool get_aux_ADC_RR_complete(uint8_t bID); // has the aux ADC read each channel at least once

// require the ADC be restarted to take affect
void enable_LPF_cells(uint8_t bID, LPF_CUTOFF_t freq); 
void enable_LPF_BB(uint8_t bID, LPF_CUTOFF_t freq); 
void disable_LPF_cells(uint8_t bID);
void disable_LPF_BB(uint8_t bID);

// voltage comparators
void OVUV_config(uint8_t bID, OV_THRESH_t OV_thresh, UV_THRESH_t UV_thresh, uint8_t cell_count);
void OVUV_start(uint8_t bID); // configure before starting!!
void OVUV_stop(uint8_t bID);
bool get_OVUV_running(uint8_t bID);

// Temp comparators
// UT_thresh:
//  Range from 66% to 80% in steps of 2% sourced from TSREF regulator voltage.
// OT_thresh:
//  Range from 10% to 39% in steps of 1% sourced from TSREF regulator voltage.
void OTUT_config(uint8_t bID, uint8_t OT_thr_percent, uint8_t UT_thr_percent);
void OTUT_start(uint8_t bID); // configure before starting!!
void OTUT_stop(uint8_t bID);
bool get_OTUT_running(uint8_t bID);

// balancing
bool balancing_start(uint8_t bID); // configure before starting!!
void balancing_stop(uint8_t bID);
void balancing_pause(uint8_t bID, bool paused);
bool get_balancing_running(uint8_t bID);
bool get_balancing_done(uint8_t bID);
bool get_bal_OT(uint8_t bID);
bool get_bal_paused(uint8_t bID); // can be paused either by choice or by OT


void enable_auto_balancing(uint8_t bID, BAL_DUTY_t duty_cycle); // automatically cycle between odd and even cells so they can all be set to balance at once
void disable_auto_balancing(uint8_t bID);

void set_balancing_timer(uint8_t bID, uint8_t cell_number, BAL_TIME_t time); 
uint16_t get_balancing_timer(uint8_t bID, uint8_t cell_number); // in seconds left
bool get_balancing_cell_done(uint8_t bID, uint8_t cell_number);

// stop balancing based on cell voltage
void enable_VCB_stop_thresh(uint8_t bID, CB_DONE_THRESH_t vcb_thr); // (also starts OVUV - see 9.3.3.1.4 p42)
void enable_CB_stop_on_fault(uint8_t bID, bool stop);

// balancing can be paused if the chip or external thermistors get too hot
//  requires OTUT detector is running
void enable_OTCB(uint8_t bID, uint8_t OT_thr_percent, uint8_t cooloff_thr_percent); // restarts OTUT 
bool get_OTCB_enabled(uint8_t bID);

// reading voltages and temperatures
int16_t get_cell_voltage(uint8_t bID, uint8_t cell_number);
int16_t get_cell_voltage_aux(uint8_t bID, uint8_t cell_number);
int16_t get_bat_voltage(uint8_t bID); // whole stack voltage
int16_t get_BB_voltage(uint8_t bID); // bus bar (current measurement)
int16_t get_BB_voltage_aux(uint8_t bID); // bus bar (current measurement)
int16_t get_gpio_voltage(uint8_t bID, uint8_t gpio_number);
int16_t get_tsref_voltage(uint8_t bID);
int16_t get_die_temp_1(uint8_t bID);
int16_t get_die_temp_2(uint8_t bID);

#endif /* BQ796xx_H_ */
//EOF
