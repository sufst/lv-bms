/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set channel_ANA0 aliases
#define channel_ANA0_TRIS                 TRISAbits.TRISA0
#define channel_ANA0_LAT                  LATAbits.LATA0
#define channel_ANA0_PORT                 PORTAbits.RA0
#define channel_ANA0_WPU                  WPUAbits.WPUA0
#define channel_ANA0_OD                   ODCONAbits.ODCA0
#define channel_ANA0_ANS                  ANSELAbits.ANSELA0
#define channel_ANA0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define channel_ANA0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define channel_ANA0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define channel_ANA0_GetValue()           PORTAbits.RA0
#define channel_ANA0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define channel_ANA0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define channel_ANA0_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define channel_ANA0_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define channel_ANA0_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define channel_ANA0_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define channel_ANA0_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define channel_ANA0_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set channel_ANA1 aliases
#define channel_ANA1_TRIS                 TRISAbits.TRISA1
#define channel_ANA1_LAT                  LATAbits.LATA1
#define channel_ANA1_PORT                 PORTAbits.RA1
#define channel_ANA1_WPU                  WPUAbits.WPUA1
#define channel_ANA1_OD                   ODCONAbits.ODCA1
#define channel_ANA1_ANS                  ANSELAbits.ANSELA1
#define channel_ANA1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define channel_ANA1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define channel_ANA1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define channel_ANA1_GetValue()           PORTAbits.RA1
#define channel_ANA1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define channel_ANA1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define channel_ANA1_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define channel_ANA1_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define channel_ANA1_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define channel_ANA1_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define channel_ANA1_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define channel_ANA1_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set channel_ANA2 aliases
#define channel_ANA2_TRIS                 TRISAbits.TRISA2
#define channel_ANA2_LAT                  LATAbits.LATA2
#define channel_ANA2_PORT                 PORTAbits.RA2
#define channel_ANA2_WPU                  WPUAbits.WPUA2
#define channel_ANA2_OD                   ODCONAbits.ODCA2
#define channel_ANA2_ANS                  ANSELAbits.ANSELA2
#define channel_ANA2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define channel_ANA2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define channel_ANA2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define channel_ANA2_GetValue()           PORTAbits.RA2
#define channel_ANA2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define channel_ANA2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define channel_ANA2_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define channel_ANA2_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define channel_ANA2_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define channel_ANA2_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define channel_ANA2_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define channel_ANA2_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set Bat1Ctrl aliases
#define Bat1Ctrl_TRIS                 TRISAbits.TRISA3
#define Bat1Ctrl_LAT                  LATAbits.LATA3
#define Bat1Ctrl_PORT                 PORTAbits.RA3
#define Bat1Ctrl_WPU                  WPUAbits.WPUA3
#define Bat1Ctrl_OD                   ODCONAbits.ODCA3
#define Bat1Ctrl_ANS                  ANSELAbits.ANSELA3
#define Bat1Ctrl_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define Bat1Ctrl_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define Bat1Ctrl_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define Bat1Ctrl_GetValue()           PORTAbits.RA3
#define Bat1Ctrl_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define Bat1Ctrl_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define Bat1Ctrl_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define Bat1Ctrl_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define Bat1Ctrl_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define Bat1Ctrl_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define Bat1Ctrl_SetAnalogMode()      do { ANSELAbits.ANSELA3 = 1; } while(0)
#define Bat1Ctrl_SetDigitalMode()     do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set Bat2Ctrl aliases
#define Bat2Ctrl_TRIS                 TRISAbits.TRISA4
#define Bat2Ctrl_LAT                  LATAbits.LATA4
#define Bat2Ctrl_PORT                 PORTAbits.RA4
#define Bat2Ctrl_WPU                  WPUAbits.WPUA4
#define Bat2Ctrl_OD                   ODCONAbits.ODCA4
#define Bat2Ctrl_ANS                  ANSELAbits.ANSELA4
#define Bat2Ctrl_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define Bat2Ctrl_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define Bat2Ctrl_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define Bat2Ctrl_GetValue()           PORTAbits.RA4
#define Bat2Ctrl_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define Bat2Ctrl_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define Bat2Ctrl_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define Bat2Ctrl_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define Bat2Ctrl_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define Bat2Ctrl_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define Bat2Ctrl_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define Bat2Ctrl_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set Bat3Ctrl aliases
#define Bat3Ctrl_TRIS                 TRISAbits.TRISA5
#define Bat3Ctrl_LAT                  LATAbits.LATA5
#define Bat3Ctrl_PORT                 PORTAbits.RA5
#define Bat3Ctrl_WPU                  WPUAbits.WPUA5
#define Bat3Ctrl_OD                   ODCONAbits.ODCA5
#define Bat3Ctrl_ANS                  ANSELAbits.ANSELA5
#define Bat3Ctrl_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define Bat3Ctrl_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define Bat3Ctrl_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define Bat3Ctrl_GetValue()           PORTAbits.RA5
#define Bat3Ctrl_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define Bat3Ctrl_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define Bat3Ctrl_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define Bat3Ctrl_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define Bat3Ctrl_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define Bat3Ctrl_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define Bat3Ctrl_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define Bat3Ctrl_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set channel_ANB0 aliases
#define channel_ANB0_TRIS                 TRISBbits.TRISB0
#define channel_ANB0_LAT                  LATBbits.LATB0
#define channel_ANB0_PORT                 PORTBbits.RB0
#define channel_ANB0_WPU                  WPUBbits.WPUB0
#define channel_ANB0_OD                   ODCONBbits.ODCB0
#define channel_ANB0_ANS                  ANSELBbits.ANSELB0
#define channel_ANB0_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define channel_ANB0_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define channel_ANB0_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define channel_ANB0_GetValue()           PORTBbits.RB0
#define channel_ANB0_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define channel_ANB0_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define channel_ANB0_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define channel_ANB0_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define channel_ANB0_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define channel_ANB0_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define channel_ANB0_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define channel_ANB0_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set channel_ANB1 aliases
#define channel_ANB1_TRIS                 TRISBbits.TRISB1
#define channel_ANB1_LAT                  LATBbits.LATB1
#define channel_ANB1_PORT                 PORTBbits.RB1
#define channel_ANB1_WPU                  WPUBbits.WPUB1
#define channel_ANB1_OD                   ODCONBbits.ODCB1
#define channel_ANB1_ANS                  ANSELBbits.ANSELB1
#define channel_ANB1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define channel_ANB1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define channel_ANB1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define channel_ANB1_GetValue()           PORTBbits.RB1
#define channel_ANB1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define channel_ANB1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define channel_ANB1_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define channel_ANB1_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define channel_ANB1_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define channel_ANB1_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define channel_ANB1_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define channel_ANB1_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set channel_ANB2 aliases
#define channel_ANB2_TRIS                 TRISBbits.TRISB2
#define channel_ANB2_LAT                  LATBbits.LATB2
#define channel_ANB2_PORT                 PORTBbits.RB2
#define channel_ANB2_WPU                  WPUBbits.WPUB2
#define channel_ANB2_OD                   ODCONBbits.ODCB2
#define channel_ANB2_ANS                  ANSELBbits.ANSELB2
#define channel_ANB2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define channel_ANB2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define channel_ANB2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define channel_ANB2_GetValue()           PORTBbits.RB2
#define channel_ANB2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define channel_ANB2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define channel_ANB2_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define channel_ANB2_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define channel_ANB2_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define channel_ANB2_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define channel_ANB2_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define channel_ANB2_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set channel_ANB3 aliases
#define channel_ANB3_TRIS                 TRISBbits.TRISB3
#define channel_ANB3_LAT                  LATBbits.LATB3
#define channel_ANB3_PORT                 PORTBbits.RB3
#define channel_ANB3_WPU                  WPUBbits.WPUB3
#define channel_ANB3_OD                   ODCONBbits.ODCB3
#define channel_ANB3_ANS                  ANSELBbits.ANSELB3
#define channel_ANB3_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define channel_ANB3_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define channel_ANB3_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define channel_ANB3_GetValue()           PORTBbits.RB3
#define channel_ANB3_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define channel_ANB3_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define channel_ANB3_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define channel_ANB3_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define channel_ANB3_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define channel_ANB3_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define channel_ANB3_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define channel_ANB3_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set RelayCtrl aliases
#define RelayCtrl_TRIS                 TRISBbits.TRISB4
#define RelayCtrl_LAT                  LATBbits.LATB4
#define RelayCtrl_PORT                 PORTBbits.RB4
#define RelayCtrl_WPU                  WPUBbits.WPUB4
#define RelayCtrl_OD                   ODCONBbits.ODCB4
#define RelayCtrl_ANS                  ANSELBbits.ANSELB4
#define RelayCtrl_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RelayCtrl_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RelayCtrl_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RelayCtrl_GetValue()           PORTBbits.RB4
#define RelayCtrl_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RelayCtrl_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RelayCtrl_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define RelayCtrl_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define RelayCtrl_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define RelayCtrl_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define RelayCtrl_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define RelayCtrl_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set PowerButton aliases
#define PowerButton_TRIS                 TRISBbits.TRISB5
#define PowerButton_LAT                  LATBbits.LATB5
#define PowerButton_PORT                 PORTBbits.RB5
#define PowerButton_WPU                  WPUBbits.WPUB5
#define PowerButton_OD                   ODCONBbits.ODCB5
#define PowerButton_ANS                  ANSELBbits.ANSELB5
#define PowerButton_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define PowerButton_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define PowerButton_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define PowerButton_GetValue()           PORTBbits.RB5
#define PowerButton_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define PowerButton_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define PowerButton_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define PowerButton_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define PowerButton_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define PowerButton_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define PowerButton_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define PowerButton_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()              PORTBbits.RB6
#define RB6_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()             do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()           do { WPUBbits.WPUB6 = 0; } while(0)
#define RB6_SetAnalogMode()         do { ANSELBbits.ANSELB6 = 1; } while(0)
#define RB6_SetDigitalMode()        do { ANSELBbits.ANSELB6 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)
#define RB7_SetAnalogMode()         do { ANSELBbits.ANSELB7 = 1; } while(0)
#define RB7_SetDigitalMode()        do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set LED1 aliases
#define LED1_TRIS                 TRISCbits.TRISC0
#define LED1_LAT                  LATCbits.LATC0
#define LED1_PORT                 PORTCbits.RC0
#define LED1_WPU                  WPUCbits.WPUC0
#define LED1_OD                   ODCONCbits.ODCC0
#define LED1_ANS                  ANSELCbits.ANSELC0
#define LED1_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define LED1_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define LED1_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define LED1_GetValue()           PORTCbits.RC0
#define LED1_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define LED1_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define LED1_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define LED1_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define LED1_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define LED1_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define LED1_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define LED1_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set SOC1 aliases
#define SOC1_TRIS                 TRISCbits.TRISC1
#define SOC1_LAT                  LATCbits.LATC1
#define SOC1_PORT                 PORTCbits.RC1
#define SOC1_WPU                  WPUCbits.WPUC1
#define SOC1_OD                   ODCONCbits.ODCC1
#define SOC1_ANS                  ANSELCbits.ANSELC1
#define SOC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define SOC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define SOC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define SOC1_GetValue()           PORTCbits.RC1
#define SOC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define SOC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define SOC1_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define SOC1_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define SOC1_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define SOC1_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define SOC1_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define SOC1_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set SOC2 aliases
#define SOC2_TRIS                 TRISCbits.TRISC2
#define SOC2_LAT                  LATCbits.LATC2
#define SOC2_PORT                 PORTCbits.RC2
#define SOC2_WPU                  WPUCbits.WPUC2
#define SOC2_OD                   ODCONCbits.ODCC2
#define SOC2_ANS                  ANSELCbits.ANSELC2
#define SOC2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define SOC2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define SOC2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define SOC2_GetValue()           PORTCbits.RC2
#define SOC2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define SOC2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define SOC2_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define SOC2_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define SOC2_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define SOC2_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define SOC2_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define SOC2_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set SOC3 aliases
#define SOC3_TRIS                 TRISCbits.TRISC3
#define SOC3_LAT                  LATCbits.LATC3
#define SOC3_PORT                 PORTCbits.RC3
#define SOC3_WPU                  WPUCbits.WPUC3
#define SOC3_OD                   ODCONCbits.ODCC3
#define SOC3_ANS                  ANSELCbits.ANSELC3
#define SOC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define SOC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define SOC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define SOC3_GetValue()           PORTCbits.RC3
#define SOC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define SOC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define SOC3_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define SOC3_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define SOC3_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define SOC3_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define SOC3_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define SOC3_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set SOC4 aliases
#define SOC4_TRIS                 TRISCbits.TRISC4
#define SOC4_LAT                  LATCbits.LATC4
#define SOC4_PORT                 PORTCbits.RC4
#define SOC4_WPU                  WPUCbits.WPUC4
#define SOC4_OD                   ODCONCbits.ODCC4
#define SOC4_ANS                  ANSELCbits.ANSELC4
#define SOC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define SOC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define SOC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define SOC4_GetValue()           PORTCbits.RC4
#define SOC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define SOC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define SOC4_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define SOC4_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define SOC4_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define SOC4_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define SOC4_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define SOC4_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set SOC5 aliases
#define SOC5_TRIS                 TRISCbits.TRISC5
#define SOC5_LAT                  LATCbits.LATC5
#define SOC5_PORT                 PORTCbits.RC5
#define SOC5_WPU                  WPUCbits.WPUC5
#define SOC5_OD                   ODCONCbits.ODCC5
#define SOC5_ANS                  ANSELCbits.ANSELC5
#define SOC5_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define SOC5_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define SOC5_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define SOC5_GetValue()           PORTCbits.RC5
#define SOC5_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define SOC5_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define SOC5_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define SOC5_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define SOC5_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define SOC5_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define SOC5_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define SOC5_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set RD0 procedures
#define RD0_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define RD0_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define RD0_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define RD0_GetValue()              PORTDbits.RD0
#define RD0_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define RD0_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define RD0_SetPullup()             do { WPUDbits.WPUD0 = 1; } while(0)
#define RD0_ResetPullup()           do { WPUDbits.WPUD0 = 0; } while(0)
#define RD0_SetAnalogMode()         do { ANSELDbits.ANSELD0 = 1; } while(0)
#define RD0_SetDigitalMode()        do { ANSELDbits.ANSELD0 = 0; } while(0)

// get/set RD1 procedures
#define RD1_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define RD1_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define RD1_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define RD1_GetValue()              PORTDbits.RD1
#define RD1_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define RD1_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)
#define RD1_SetPullup()             do { WPUDbits.WPUD1 = 1; } while(0)
#define RD1_ResetPullup()           do { WPUDbits.WPUD1 = 0; } while(0)
#define RD1_SetAnalogMode()         do { ANSELDbits.ANSELD1 = 1; } while(0)
#define RD1_SetDigitalMode()        do { ANSELDbits.ANSELD1 = 0; } while(0)

// get/set Extra3 aliases
#define Extra3_TRIS                 TRISDbits.TRISD6
#define Extra3_LAT                  LATDbits.LATD6
#define Extra3_PORT                 PORTDbits.RD6
#define Extra3_WPU                  WPUDbits.WPUD6
#define Extra3_OD                   ODCONDbits.ODCD6
#define Extra3_ANS                  ANSELDbits.ANSELD6
#define Extra3_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define Extra3_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define Extra3_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define Extra3_GetValue()           PORTDbits.RD6
#define Extra3_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define Extra3_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define Extra3_SetPullup()          do { WPUDbits.WPUD6 = 1; } while(0)
#define Extra3_ResetPullup()        do { WPUDbits.WPUD6 = 0; } while(0)
#define Extra3_SetPushPull()        do { ODCONDbits.ODCD6 = 0; } while(0)
#define Extra3_SetOpenDrain()       do { ODCONDbits.ODCD6 = 1; } while(0)
#define Extra3_SetAnalogMode()      do { ANSELDbits.ANSELD6 = 1; } while(0)
#define Extra3_SetDigitalMode()     do { ANSELDbits.ANSELD6 = 0; } while(0)

// get/set Extra4 aliases
#define Extra4_TRIS                 TRISDbits.TRISD7
#define Extra4_LAT                  LATDbits.LATD7
#define Extra4_PORT                 PORTDbits.RD7
#define Extra4_WPU                  WPUDbits.WPUD7
#define Extra4_OD                   ODCONDbits.ODCD7
#define Extra4_ANS                  ANSELDbits.ANSELD7
#define Extra4_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define Extra4_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define Extra4_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define Extra4_GetValue()           PORTDbits.RD7
#define Extra4_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define Extra4_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define Extra4_SetPullup()          do { WPUDbits.WPUD7 = 1; } while(0)
#define Extra4_ResetPullup()        do { WPUDbits.WPUD7 = 0; } while(0)
#define Extra4_SetPushPull()        do { ODCONDbits.ODCD7 = 0; } while(0)
#define Extra4_SetOpenDrain()       do { ODCONDbits.ODCD7 = 1; } while(0)
#define Extra4_SetAnalogMode()      do { ANSELDbits.ANSELD7 = 1; } while(0)
#define Extra4_SetDigitalMode()     do { ANSELDbits.ANSELD7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/