/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include <xc.h>
#include "mcc.h"
#include "units.h"
#include "batt_properties.h"
#include "therm_LUT.h"
#include "millis.h"
#include "error_load_store.h"

#define CELL_COUNT 3

#ifndef min
#define min(a,b) ((a<b) ?a:b)
#endif

// list of adc channels for each voltage tap: {Bat1V, Bat2V, Bat3V}
ADC_channel_t bat_v_channels[] = {channel_ANA0, channel_ANA1, channel_ANA2};

// list of adc channels for each temperature probe: {Therm1V, Therm2V, Therm3V}
ADC_channel_t bat_t_channels[] = {channel_ANB1, channel_ANB2, channel_ANB3};

ADC_channel_t bat_c_channels[] = {channel_ANB0};

static voltage_t cell_voltages[3]; // voltage * VOLTAGE_MULTIPLIER
static temp_t cell_temps[3]; // 'C
static current_t bat_current; // +ve is  charge -ve is discharge


bool locked_out = false;

static error_t error_type = FAULT_NONE;
static uint8_t error_param1 = 0;
static uint8_t error_param2 = 0;

void CAN_RX_ISR()
{
    CAN_MSG_OBJ rx_msg;
    CAN1_ReceiveFrom(FIFO1, &rx_msg);
    
    // TODO some kind of command interface - emulate the ORION BMS for the LV battery
}

temp_t adc_to_temp(adc_result_t reading) 
{
    // just read from thermistor curve LUT
	return therm_lut[reading];
}

voltage_t adc_to_cell_v(adc_result_t reading) 
{
    // get voltage at ADC input (in uint16 format)
    // 3.3 is ADC reference voltage
    // 4095 is ADC reading at reference voltage (it's 12 bit)
    voltage_t read_voltage = ((uint32_t)reading * VOLTAGE_MULTIPLIER * 3.3) / 4096;
    
	return read_voltage * 2; // step down divider in 10k:10k
}

current_t adc_to_current(adc_result_t reading)
{
    // input: 0 to 4096
    uint32_t scaled = ((uint32_t)reading * CURRENT_MULTIPLIER * 50) / 4096;
    // still unsigned but right range: 0 to 50*CURRENT_MULTIPLIER
    current_t current = (int32_t)scaled - (25 * CURRENT_MULTIPLIER);
    
    return current;
}

    void fault()
{
    // store error
    
    
    // shut down
    RelayCtrl_SetLow();
}

// milisecond timer hook  - runs the display
void millis_hook (uint64_t uptime) {
    if ((uptime % 64) == 0) {
        disp_update();
    }
}

    
/*
                         Main application
 */
void main(void)
{
    CAN_MSG_OBJ msg;
    error_t saved_fault;
    
    // Initialize the device
    SYSTEM_Initialize();
    
    // start 1 ms timer
    millis_setup();
    
    // check if a watchdog reset just happened
    if (WDTWV_bit) {
        saved_fault = FAULT_WDT;
    } else{
        saved_fault = load_fault_code(void);   
    }
    
    
    // can setup
    CAN1_SetFIFO1FullHandler(&CAN_RX_ISR);

    // load saved error
    
    // TODO load from NVM the fault code
    
    
    RelayCtrl_SetHigh(); // closes isolation relay
    
    
    
    
    ADC_SelectContext(CONTEXT_1);

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    while (1)
    {
        // read cell voltages
        for (uint8_t cell_i = 0; cell_i < CELL_COUNT; cell_i++)
		{
            ADC_StartConversion(bat_v_channels[cell_i]);
            while(!ADC_IsConversionDone());
            adc_result_t reading = ADC_GetConversionResult();
            cell_voltages[cell_i] = adc_to_cell_v(reading);
        }
        
        // read cell temperatures
        for (uint8_t cell_i = 0; cell_i < CELL_COUNT; cell_i++)
		{
            ADC_StartConversion(bat_t_channels[cell_i]);
            while(!ADC_IsConversionDone());
            adc_result_t reading = ADC_GetConversionResult();
            cell_temps[cell_i] = adc_to_temp(reading);
        }
        
        // read current
        ADC_StartConversion(bat_c_channels[0]);
        while(!ADC_IsConversionDone());
        adc_result_t reading = ADC_GetConversionResult();
        bat_current = adc_to_current(reading);
        
        if(bat_current < A(0)){
            charge_mode = DISCHARGING;
        } else {
            charge_mode = CHARGING;
        }
        
        
        // checks
        for(uint8_t cell_i = 0; cell_i < CELL_COUNT; cell_i++) {
            if(check_over_volt(cell_voltages[cell_i])) {
                // send error message over can
                error_type = FAULT_V;
                error_param1 = cell_i;
                error_param2 = cell_voltages[cell_i];
                fault();
            }
            if(check_under_volt(cell_voltages[cell_i])) {
                // send error message over can
                error_type = FAULT_V;
                error_param1 = cell_i;
                error_param2 = cell_voltages[cell_i];
                fault();
            }
            if(check_under_temp(cell_voltages[cell_i])) {
                // send error message over can
                error_type = FAULT_TEMP;
                error_param1 = cell_i;
                error_param2 = cell_temps[cell_i];
                fault();
            }
            if(check_over_temp(cell_temps[cell_i])) {
                // send error message over can
                error_type = FAULT_TEMP;
                error_param1 = cell_i;
                error_param2 = cell_temps[cell_i];
                fault();
            }
        }
        
        if((CHARGING == charge_mode) && check_over_current_charge(bat_current)) {
            // send error message
            error_type = FAULT_CURR;
            error_param2 = bat_current;
            fault();
        }
        if((DISCHARGING == charge_mode) && check_over_current_discharge(bat_current)) {
            // send error message
            error_type = FAULT_CURR;
            error_param2 = bat_current;
            fault();
        }
        
        if(CHARGING == charge_mode) {
            // update balance signals
            voltage_t min_voltage = UINT16_MAX;
            for (int i=0; i<3; i++) {
                min_voltage = min(cell_voltages[i], min_voltage);
            }
            // if a cell is too far above min voltage, discharge it
            if((cell_voltages[0] - min_voltage) > BALANCE_START_TH) {
                Bat1Ctrl_SetHigh();
            }
            if((cell_voltages[1] - min_voltage) > BALANCE_START_TH) {
                Bat2Ctrl_SetHigh();
            }
            if((cell_voltages[2] - min_voltage) > BALANCE_START_TH) {
                Bat3Ctrl_SetHigh();
            }

            // stop if close enough
            if((cell_voltages[0] - min_voltage) < BALANCE_END_TH) {
                Bat1Ctrl_SetLow();
            }
            if((cell_voltages[1] - min_voltage) < BALANCE_END_TH) {
                Bat2Ctrl_SetLow();
            }
            if((cell_voltages[2] - min_voltage) < BALANCE_END_TH) {
                Bat3Ctrl_SetLow();
            }
        } else{
            Bat1Ctrl_SetLow();
            Bat2Ctrl_SetLow();
            Bat3Ctrl_SetLow();
        }
        
        // send out state over can
        switch (CAN1_OperationModeGet()){
            case CAN_CONFIGURATION_MODE:
                CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);
                break;
            case CAN_NORMAL_2_0_MODE:
                if(CAN1_TransmitFIFOStatusGet(CAN1_TX_TXQ) == CAN_TX_FIFO_AVAILABLE) {
                    if (msg) {
                        CAN1_Transmit(CAN1_TX_TXQ, &msg);
                        msg = 0; // resets msg so it isn't repeat transmitted
                    }
                }
                break;
        }
        
        // set indicator LEDs
        if (CHARGING == charge_mode){
            
        }
        
        
        // check power off button
        if (!PowerButton_GetValue()) {
            // power off
            CLRWDT();
            RelayCtrl_SetLow();
            while(1);
        }
        
        // clear WDT
        CLRWDT();
    }
}
/**
 End of File
*/