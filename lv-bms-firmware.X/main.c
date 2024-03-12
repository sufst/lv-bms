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

ADC_channel_t bat_v_channels[] = {Bat1V, Bat2V, Bat3V};
ADC_channel_t bat_t_channels[] = {Therm1V, Therm2V, Therm3V};

static uint16_t cell_voltages[3];
static int8_t cell_temps[3]; // 'C

void CAN_RX_ISR()
{
    CAN_MSG_OBJ rx_msg;
    CAN1_ReceiveFrom(FIFO1, &rx_msg);
}


int8_t adc_to_temp(adc_result_t reading) 
{
    // just read from thermistor curve LUT
	return therm_lut[reading];
}

uint16_t adc_to_cell_v(adc_result_t reading) 
{
    // just read from thermistor curve LUT
	return maths(reading);
}



/*
                         Main application
 */
void main(void)
{

    CAN_MSG_OBJ msg;
    
    // Initialize the device
    SYSTEM_Initialize();
    CAN1_SetFIFO1FullHandler(&CAN_RX_ISR);

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

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
        
        // fault detection
            // fault: isolate battery
        
        // update balance signals
        
        // send out state over can
    }
}
/**
 End of File
*/