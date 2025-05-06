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

#define MAXBYTES (16*2)     //maximum number of bytes to be read from the devices (for array creation)
#define BAUDRATE 1000000    //device + uC baudrate

// debug logging - split into levels
typedef enum {BQ_LOG_ERR, BQ_LOG_WARN, BQ_LOG_INFO, BQ_LOG_DBG} bq796xx_log_level_t;
bq796xx_log_level_t bq796xx_log_level = BQ_LOG_INFO; // all the logging by default

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
void ResetAllFaults(uint8_t bID, FRMWRT_RW_TYPE_t bWriteType);
void MaskAllFaults(uint8_t bID, FRMWRT_RW_TYPE_t bWriteType);
uint8_t GetFaultStat();

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

// odds
void ConfigureOverCurrent(uint8_t bID);                             // untested
void set_SW_CTRL(uint8_t bID, uint8_t pinNum, uint8_t value);       // untested
void set_GPIO_Out_Val(uint8_t bID, uint8_t gpioNum, uint8_t val);   // untested
void set_GPIO_As_PWM(uint8_t bID, uint8_t gpioNum, uint8_t val);    // untested

// thermistor config
void enable_therm_power(uint8_t bID);
void disable_therm_power(uint8_t bID);
void set_GPIO_as_therm(uint8_t bID, uint8_t gpioNum);

// ADC control
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
void enable_LPF_cells(uint8_t bID, LPF_CUTOFF_t freq);
void enable_LPF_BB(uint8_t bID, LPF_CUTOFF_t freq);
void disable_LPF_cells(uint8_t bID);
void disable_LPF_BB(uint8_t bID);

// voltage comparators
void OVUV_config(OV_THRESH_t OV_thresh, UV_THRESH_t UV_thresh, int cell_count);
void OVUV_start(); // configure before starting!!
void OVUV_stop();
bool get_OVUV_running();

// Temp comparators
// UT_thresh:
//  Range from 66% to 80% in steps of 2% sourced from TSREF regulator voltage.
// OT_thresh:
//  Range from 10% to 39% in steps of 1% sourced from TSREF regulator voltage.
void OTUT_config(uint8_t OT_thr_percent, uint8_t UT_thr_percent);
void OTUT_start(); // configure before starting!!
void OTUT_stop();
bool get_OTUT_running();

// balancing
void balancing_start(uint8_t bID); // configure before starting!!
void balancing_stop(uint8_t bID);
void balancing_pause(bool paused);
bool get_bal_paused();

void enable_auto_balancing(uint8_t bID, BAL_DUTY_t duty_cycle); // automatically cycle between odd and even cells so they can all be set to balance at once
void disable_auto_balancing(uint8_t bID);
bool get_balancing_running(uint8_t bID);

void set_balancing_timer(uint8_t bID, uint8_t cell_number, BAL_TIME_t time); 
BAL_TIME_t get_balancing_timer(uint8_t bID, uint8_t cell_number); 
bool get_balancing_done(uint8_t bID, uint8_t cell_number);

// uses GPIO3 for balancing the whole module
void set_module_balancing_timer(uint8_t bID, BAL_TIME_t time);
BAL_TIME_t get_module_balancing_timer(uint8_t bID);
bool get_module_balancing_done(uint8_t bID);

// stop balancing based on cell voltage
//  requires OVUV detector is running
void enable_VCB_stop_thresh(uint8_t bID, CB_DONE_THRESH_t vcb_thr);
void enable_VMB_stop_thres(uint8_t vmb_thr); // thr in volts (18V to 65V)

// balancing can be paused if the chip or external thermistors get too hot
//  requires OTUT detector is running
void enable_OTCB(uint8_t bID, uint8_t OT_thr_percent, uint8_t cooloff_thr_percent);
bool get_OTCB_running(uint8_t bID);

// reading voltages and temperatures
int16_t get_cell_voltage(uint8_t bID, uint8_t cell_number);
int16_t get_cell_voltage_aux(uint8_t bID, uint8_t cell_number);
int16_t get_BB_voltage(uint8_t bID); // bus bar (current measurement)
int16_t get_BB_voltage_aux(uint8_t bID); // bus bar (current measurement)
int16_t get_temp(uint8_t bID, uint8_t therm_number);
int16_t get_die_temp_1(uint8_t bID);
int16_t get_die_temp_2(uint8_t bID);

void RunCB();

int ReadDeviceStat2(uint8_t *response_frame);
void ReadCoulumbCount(uint8_t *response_frame, uint8_t rshunt_u8);

#endif /* BQ796xx_H_ */
//EOF
