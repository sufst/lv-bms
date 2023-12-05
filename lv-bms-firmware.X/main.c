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

volatile uint8_t count = 0;

void CAN_RX_ISR()
{
    CAN_MSG_OBJ rx_msg;
    CAN1_ReceiveFrom(FIFO1, &rx_msg);
    
    if(rx_msg.data[0] == 0x42)
    {
        count = 0;
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
    CAN1_SetFIFO1FullHandler(&CAN_RX_ISR);

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    while (1)
    {
        // Add your application code
        __delay_ms(1000);        

        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        { 
            CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);
        }

        if(CAN_NORMAL_2_0_MODE == CAN1_OperationModeGet())
        {
            msg.msgId = 0x00001;
            msg.field.formatType = CAN_2_0_FORMAT;
            msg.field.brs = CAN_NON_BRS_MODE;
            msg.field.dlc = DLC_1;
            msg.field.frameType = CAN_FRAME_DATA;
            msg.field.idType = CAN_FRAME_EXT;
            msg.data = &count;

            if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_TXQ)))
            {
                IO_RC6_SetHigh();
                CAN1_Transmit(CAN1_TX_TXQ, &msg);

                while(CAN1_TransmitFIFOStatusGet(CAN1_TX_TXQ) == CAN_TX_FIFO_FULL);

                IO_RC6_SetLow();
            }
        }
        
        count++;
    }
}
/**
 End of File
*/