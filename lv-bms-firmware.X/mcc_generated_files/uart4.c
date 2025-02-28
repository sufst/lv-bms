/**
  UART4 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    uart4.c

  @Summary
    This is the generated driver implementation file for the UART4 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for UART4.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  2.4.1
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB             :  MPLAB X 6.00
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

/**
  Section: Included Files
*/
#include <xc.h>
#include "uart4.h"

static volatile uart4_status_t uart4RxLastError;

/**
  Section: UART4 APIs
*/
void (*UART4_FramingErrorHandler)(void);
void (*UART4_OverrunErrorHandler)(void);
void (*UART4_ErrorHandler)(void);

void UART4_DefaultFramingErrorHandler(void);
void UART4_DefaultOverrunErrorHandler(void);
void UART4_DefaultErrorHandler(void);

void UART4_Initialize(void)
{
    // Disable interrupts before changing states

    // Set the UART4 module to the options selected in the user interface.

    // P1L 0; 
    U4P1L = 0x00;

    // P2L 0; 
    U4P2L = 0x00;

    // P3L 0; 
    U4P3L = 0x00;

    // BRGS high speed; MODE Asynchronous 8-bit mode; RXEN enabled; TXEN enabled; ABDEN disabled; 
    U4CON0 = 0xB0;

    // RXBIMD Set RXBKIF on rising RX input; BRKOVR disabled; WUE disabled; SENDB disabled; ON enabled; 
    U4CON1 = 0x80;

    // TXPOL not inverted; FLO off; RXPOL not inverted; RUNOVF RX input shifter stops all activity; STP Transmit 1Stop bit, receiver verifies first Stop bit; 
    U4CON2 = 0x00;

    // BRGL 4; 
    U4BRGL = 0x04;

    // BRGH 0; 
    U4BRGH = 0x00;

    // STPMD in middle of first Stop bit; TXWRE No error; 
    U4FIFO = 0x00;

    // ABDIF Auto-baud not enabled or not complete; WUIF WUE not enabled by software; ABDIE disabled; 
    U4UIR = 0x00;

    // ABDOVF Not overflowed; TXCIF 0; RXBKIF No Break detected; RXFOIF not overflowed; CERIF No Checksum error; 
    U4ERRIR = 0x00;

    // TXCIE disabled; FERIE disabled; TXMTIE disabled; ABDOVE disabled; CERIE disabled; RXFOIE disabled; PERIE disabled; RXBKIE disabled; 
    U4ERRIE = 0x00;


    UART4_SetFramingErrorHandler(UART4_DefaultFramingErrorHandler);
    UART4_SetOverrunErrorHandler(UART4_DefaultOverrunErrorHandler);
    UART4_SetErrorHandler(UART4_DefaultErrorHandler);

    uart4RxLastError.status = 0;

}

bool UART4_is_rx_ready(void)
{
    return (bool)(PIR12bits.U4RXIF);
}

bool UART4_is_tx_ready(void)
{
    return (bool)(PIR12bits.U4TXIF && U4CON0bits.TXEN);
}

bool UART4_is_tx_done(void)
{
    return U4ERRIRbits.TXMTIF;
}

uart4_status_t UART4_get_last_status(void){
    return uart4RxLastError;
}

uint8_t UART4_Read(void)
{
    while(!PIR12bits.U4RXIF)
    {
    }

    uart4RxLastError.status = 0;

    if(U4ERRIRbits.FERIF){
        uart4RxLastError.ferr = 1;
        UART4_FramingErrorHandler();
    }

    if(U4ERRIRbits.RXFOIF){
        uart4RxLastError.oerr = 1;
        UART4_OverrunErrorHandler();
    }

    if(uart4RxLastError.status){
        UART4_ErrorHandler();
    }

    return U4RXB;
}

void UART4_Write(uint8_t txData)
{
    while(0 == PIR12bits.U4TXIF)
    {
    }

    U4TXB = txData;    // Write the data byte to the USART.
}





void UART4_DefaultFramingErrorHandler(void){}

void UART4_DefaultOverrunErrorHandler(void){}

void UART4_DefaultErrorHandler(void){
}

void UART4_SetFramingErrorHandler(void (* interruptHandler)(void)){
    UART4_FramingErrorHandler = interruptHandler;
}

void UART4_SetOverrunErrorHandler(void (* interruptHandler)(void)){
    UART4_OverrunErrorHandler = interruptHandler;
}

void UART4_SetErrorHandler(void (* interruptHandler)(void)){
    UART4_ErrorHandler = interruptHandler;
}






/**
  End of File
*/
