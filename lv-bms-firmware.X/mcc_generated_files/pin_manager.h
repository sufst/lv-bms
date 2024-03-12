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

// get/set Bat1V aliases
#define Bat1V_TRIS                 TRISAbits.TRISA0
#define Bat1V_LAT                  LATAbits.LATA0
#define Bat1V_PORT                 PORTAbits.RA0
#define Bat1V_WPU                  WPUAbits.WPUA0
#define Bat1V_OD                   ODCONAbits.ODCA0
#define Bat1V_ANS                  ANSELAbits.ANSELA0
#define Bat1V_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define Bat1V_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define Bat1V_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define Bat1V_GetValue()           PORTAbits.RA0
#define Bat1V_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define Bat1V_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define Bat1V_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define Bat1V_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define Bat1V_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define Bat1V_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define Bat1V_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define Bat1V_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set Bat2V aliases
#define Bat2V_TRIS                 TRISAbits.TRISA1
#define Bat2V_LAT                  LATAbits.LATA1
#define Bat2V_PORT                 PORTAbits.RA1
#define Bat2V_WPU                  WPUAbits.WPUA1
#define Bat2V_OD                   ODCONAbits.ODCA1
#define Bat2V_ANS                  ANSELAbits.ANSELA1
#define Bat2V_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define Bat2V_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define Bat2V_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define Bat2V_GetValue()           PORTAbits.RA1
#define Bat2V_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define Bat2V_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define Bat2V_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define Bat2V_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define Bat2V_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define Bat2V_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define Bat2V_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define Bat2V_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set Bat3V aliases
#define Bat3V_TRIS                 TRISAbits.TRISA2
#define Bat3V_LAT                  LATAbits.LATA2
#define Bat3V_PORT                 PORTAbits.RA2
#define Bat3V_WPU                  WPUAbits.WPUA2
#define Bat3V_OD                   ODCONAbits.ODCA2
#define Bat3V_ANS                  ANSELAbits.ANSELA2
#define Bat3V_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define Bat3V_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define Bat3V_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define Bat3V_GetValue()           PORTAbits.RA2
#define Bat3V_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define Bat3V_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define Bat3V_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define Bat3V_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define Bat3V_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define Bat3V_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define Bat3V_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define Bat3V_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

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

// get/set BatCur aliases
#define BatCur_TRIS                 TRISBbits.TRISB0
#define BatCur_LAT                  LATBbits.LATB0
#define BatCur_PORT                 PORTBbits.RB0
#define BatCur_WPU                  WPUBbits.WPUB0
#define BatCur_OD                   ODCONBbits.ODCB0
#define BatCur_ANS                  ANSELBbits.ANSELB0
#define BatCur_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define BatCur_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define BatCur_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define BatCur_GetValue()           PORTBbits.RB0
#define BatCur_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define BatCur_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define BatCur_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define BatCur_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define BatCur_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define BatCur_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define BatCur_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define BatCur_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set Therm1V aliases
#define Therm1V_TRIS                 TRISBbits.TRISB1
#define Therm1V_LAT                  LATBbits.LATB1
#define Therm1V_PORT                 PORTBbits.RB1
#define Therm1V_WPU                  WPUBbits.WPUB1
#define Therm1V_OD                   ODCONBbits.ODCB1
#define Therm1V_ANS                  ANSELBbits.ANSELB1
#define Therm1V_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define Therm1V_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define Therm1V_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define Therm1V_GetValue()           PORTBbits.RB1
#define Therm1V_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define Therm1V_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define Therm1V_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define Therm1V_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define Therm1V_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define Therm1V_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define Therm1V_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define Therm1V_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set Therm2V aliases
#define Therm2V_TRIS                 TRISBbits.TRISB2
#define Therm2V_LAT                  LATBbits.LATB2
#define Therm2V_PORT                 PORTBbits.RB2
#define Therm2V_WPU                  WPUBbits.WPUB2
#define Therm2V_OD                   ODCONBbits.ODCB2
#define Therm2V_ANS                  ANSELBbits.ANSELB2
#define Therm2V_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define Therm2V_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define Therm2V_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define Therm2V_GetValue()           PORTBbits.RB2
#define Therm2V_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define Therm2V_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define Therm2V_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define Therm2V_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define Therm2V_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define Therm2V_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define Therm2V_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define Therm2V_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set Therm3V aliases
#define Therm3V_TRIS                 TRISBbits.TRISB3
#define Therm3V_LAT                  LATBbits.LATB3
#define Therm3V_PORT                 PORTBbits.RB3
#define Therm3V_WPU                  WPUBbits.WPUB3
#define Therm3V_OD                   ODCONBbits.ODCB3
#define Therm3V_ANS                  ANSELBbits.ANSELB3
#define Therm3V_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define Therm3V_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define Therm3V_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define Therm3V_GetValue()           PORTBbits.RB3
#define Therm3V_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define Therm3V_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define Therm3V_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define Therm3V_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define Therm3V_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define Therm3V_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define Therm3V_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define Therm3V_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

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

// get/set Extra1 aliases
#define Extra1_TRIS                 TRISDbits.TRISD4
#define Extra1_LAT                  LATDbits.LATD4
#define Extra1_PORT                 PORTDbits.RD4
#define Extra1_WPU                  WPUDbits.WPUD4
#define Extra1_OD                   ODCONDbits.ODCD4
#define Extra1_ANS                  ANSELDbits.ANSELD4
#define Extra1_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define Extra1_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define Extra1_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define Extra1_GetValue()           PORTDbits.RD4
#define Extra1_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define Extra1_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define Extra1_SetPullup()          do { WPUDbits.WPUD4 = 1; } while(0)
#define Extra1_ResetPullup()        do { WPUDbits.WPUD4 = 0; } while(0)
#define Extra1_SetPushPull()        do { ODCONDbits.ODCD4 = 0; } while(0)
#define Extra1_SetOpenDrain()       do { ODCONDbits.ODCD4 = 1; } while(0)
#define Extra1_SetAnalogMode()      do { ANSELDbits.ANSELD4 = 1; } while(0)
#define Extra1_SetDigitalMode()     do { ANSELDbits.ANSELD4 = 0; } while(0)

// get/set Extra2 aliases
#define Extra2_TRIS                 TRISDbits.TRISD5
#define Extra2_LAT                  LATDbits.LATD5
#define Extra2_PORT                 PORTDbits.RD5
#define Extra2_WPU                  WPUDbits.WPUD5
#define Extra2_OD                   ODCONDbits.ODCD5
#define Extra2_ANS                  ANSELDbits.ANSELD5
#define Extra2_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define Extra2_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define Extra2_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define Extra2_GetValue()           PORTDbits.RD5
#define Extra2_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define Extra2_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define Extra2_SetPullup()          do { WPUDbits.WPUD5 = 1; } while(0)
#define Extra2_ResetPullup()        do { WPUDbits.WPUD5 = 0; } while(0)
#define Extra2_SetPushPull()        do { ODCONDbits.ODCD5 = 0; } while(0)
#define Extra2_SetOpenDrain()       do { ODCONDbits.ODCD5 = 1; } while(0)
#define Extra2_SetAnalogMode()      do { ANSELDbits.ANSELD5 = 1; } while(0)
#define Extra2_SetDigitalMode()     do { ANSELDbits.ANSELD5 = 0; } while(0)

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