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

#include "mcc_generated_files/mcc.h"
#include "therm_LUT.h"

#define CELL_COUNT 3
#define VOLTAGE_MULTIPLIER 1024
#define CURRENT_MULTIPLIER 1024

#define V(volt) volt * VOLTAGE_MULTIPLIER
#define A(curr) curr * CURRENT_MULTIPLIER

#define DISCHARGE_CURRENT_MAX A(25) // TODO: replace with actual values
#define CHARGE_CURRENT_MAX A(25) // TODO: replace with actual values
#define CELL_VOLTAGE_MAX V(4.3) // TODO: replace with actual values
#define CELL_VOLTAGE_MIN V(3.2) // TODO: replace with actual values
#define CELL_TEMP_MAX 60 // TODO: replace with actual values
#define CELL_TEMP_MIN 0 // TODO: replace with actual values

#define BALANCE_START_TH V(0.2)
#define BALANCE_END_TH V(0.05)

#ifndef min
#define min(a,b) ((a<b) ?a:b)
#endif

ADC_channel_t bat_v_channels[] = {Bat1V, Bat2V, Bat3V};
ADC_channel_t bat_t_channels[] = {Therm1V, Therm2V, Therm3V};

static uint16_t cell_voltages[3]; // voltage * VOLTAGE_MULTIPLIER
static int8_t cell_temps[3]; // 'C
static int16_t bat_current; //

static enum error_e {  FAULT_NONE, 
                FAULT_V,
                FAULT_TEMP, 
                FAULT_CURR,
                FAULT_WDT,
            } error_type = FAULT_NONE;
static uint8_t error_param1 = 0;
static uint8_t error_param2 = 0;

void CAN_RX_ISR()
{
    CAN_MSG_OBJ rx_msg;
    CAN1_ReceiveFrom(FIFO1, &rx_msg);
    
    // some kind of command interface
}

int8_t adc_to_temp(adc_result_t reading) 
{
    // just read from thermistor curve LUT
	return therm_lut[reading];
}

uint16_t adc_to_cell_v(adc_result_t reading) 
{
    // get voltage at ADC input (in uint16 format)
    // 3.3 is ADC reference voltage
    // 4095 is ADC reading at reference voltage (it's 12 bit)
    uint16_t read_voltage = ((uint32_t)reading * VOLTAGE_MULTIPLIER * 3.3) / 4096;
    
	return read_voltage * 2; // step down divider in 10k:10k
}

int16_t adc_to_current(adc_result_t reading)
{
    // input: 0 to 4096
    uint32_t scaled = ((uint32_t)reading * CURRENT_MULTIPLIER * 50) / 4096;
    // still unsigned but right range: 0 to 50*CURRENT_MULTIPLIER
    
    return (int32_t)scaled - (25 * CURRENT_MULTIPLIER);
}

void fault()
{
    // store error
    
    
    // shut down
    RelayCtrl_SetLow();
    while(1){
        // transmit lvbms error message
    }
}
/*
                         Main application
 */
void main(void)
{
    CAN_MSG_OBJ msg;
    
    // Initialize the device
    SYSTEM_Initialize();
    
    
    RelayCtrl_SetHigh(); // closes isolation relay
    
    // can setup
    CAN1_SetFIFO1FullHandler(&CAN_RX_ISR);

    // load saved error
    
    // check for WDT error
    if (_BIT_ACCESS(PCON0, WDTWV)) {
    }
    
    // transmit saved error    
    ////msg = TODO: msg_getter_funct
    
 
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
        ADC_StartConversion(BatCur);
        while(!ADC_IsConversionDone());
        adc_result_t reading = ADC_GetConversionResult();
        bat_current = adc_to_current(reading);
        
        
        // checks
        for(uint8_t cell_i = 0; cell_i < CELL_COUNT; cell_i++) {
            if(cell_voltages[cell_i] < CELL_VOLTAGE_MIN) {
                // send error message over can
                error_type = FAULT_V;
                error_param1 = cell_i;
                error_param2 = cell_voltages[cell_i];
                fault();
            }
            if(cell_voltages[cell_i] > CELL_VOLTAGE_MAX) {
                // send error message over can
                error_type = FAULT_V;
                error_param1 = cell_i;
                error_param2 = cell_voltages[cell_i];
                fault();
            }
            if(cell_temps[cell_i] < CELL_TEMP_MIN) {
                // send error message over can
                error_type = FAULT_TEMP;
                error_param1 = cell_i;
                error_param2 = cell_temps[cell_i];
                fault();
            }
            if(cell_temps[cell_i] > CELL_TEMP_MAX) {
                // send error message over can
                error_type = FAULT_TEMP;
                error_param1 = cell_i;
                error_param2 = cell_temps[cell_i];
                fault();
            }
        }
        
        if(bat_current < -CHARGE_CURRENT_MAX) {
            // send error message
            error_type = FAULT_CURR;
            error_param2 = bat_current;
            fault();
        }
        if(bat_current > DISCHARGE_CURRENT_MAX) {
            // send error message
            error_type = FAULT_CURR;
            error_param2 = bat_current;
            fault();
        }
        
        
        // update balance signals
        uint16_t min_voltage = UINT16_MAX;
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
        
        
        // check power off button
        if (!PowerButton_GetValue()) {
            // power off
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