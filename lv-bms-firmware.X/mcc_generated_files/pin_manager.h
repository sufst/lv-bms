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

// get/set Bonus1 aliases
#define Bonus1_TRIS                 TRISAbits.TRISA0
#define Bonus1_LAT                  LATAbits.LATA0
#define Bonus1_PORT                 PORTAbits.RA0
#define Bonus1_WPU                  WPUAbits.WPUA0
#define Bonus1_OD                   ODCONAbits.ODCA0
#define Bonus1_ANS                  ANSELAbits.ANSELA0
#define Bonus1_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define Bonus1_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define Bonus1_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define Bonus1_GetValue()           PORTAbits.RA0
#define Bonus1_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define Bonus1_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define Bonus1_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define Bonus1_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define Bonus1_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define Bonus1_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define Bonus1_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define Bonus1_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set Bonus2 aliases
#define Bonus2_TRIS                 TRISAbits.TRISA1
#define Bonus2_LAT                  LATAbits.LATA1
#define Bonus2_PORT                 PORTAbits.RA1
#define Bonus2_WPU                  WPUAbits.WPUA1
#define Bonus2_OD                   ODCONAbits.ODCA1
#define Bonus2_ANS                  ANSELAbits.ANSELA1
#define Bonus2_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define Bonus2_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define Bonus2_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define Bonus2_GetValue()           PORTAbits.RA1
#define Bonus2_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define Bonus2_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define Bonus2_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define Bonus2_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define Bonus2_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define Bonus2_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define Bonus2_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define Bonus2_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set Bonus3 aliases
#define Bonus3_TRIS                 TRISAbits.TRISA2
#define Bonus3_LAT                  LATAbits.LATA2
#define Bonus3_PORT                 PORTAbits.RA2
#define Bonus3_WPU                  WPUAbits.WPUA2
#define Bonus3_OD                   ODCONAbits.ODCA2
#define Bonus3_ANS                  ANSELAbits.ANSELA2
#define Bonus3_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define Bonus3_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define Bonus3_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define Bonus3_GetValue()           PORTAbits.RA2
#define Bonus3_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define Bonus3_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define Bonus3_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define Bonus3_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define Bonus3_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define Bonus3_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define Bonus3_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define Bonus3_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set Bonus4 aliases
#define Bonus4_TRIS                 TRISAbits.TRISA3
#define Bonus4_LAT                  LATAbits.LATA3
#define Bonus4_PORT                 PORTAbits.RA3
#define Bonus4_WPU                  WPUAbits.WPUA3
#define Bonus4_OD                   ODCONAbits.ODCA3
#define Bonus4_ANS                  ANSELAbits.ANSELA3
#define Bonus4_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define Bonus4_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define Bonus4_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define Bonus4_GetValue()           PORTAbits.RA3
#define Bonus4_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define Bonus4_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define Bonus4_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define Bonus4_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define Bonus4_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define Bonus4_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define Bonus4_SetAnalogMode()      do { ANSELAbits.ANSELA3 = 1; } while(0)
#define Bonus4_SetDigitalMode()     do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set Bonus5 aliases
#define Bonus5_TRIS                 TRISAbits.TRISA4
#define Bonus5_LAT                  LATAbits.LATA4
#define Bonus5_PORT                 PORTAbits.RA4
#define Bonus5_WPU                  WPUAbits.WPUA4
#define Bonus5_OD                   ODCONAbits.ODCA4
#define Bonus5_ANS                  ANSELAbits.ANSELA4
#define Bonus5_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define Bonus5_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define Bonus5_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define Bonus5_GetValue()           PORTAbits.RA4
#define Bonus5_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define Bonus5_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define Bonus5_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define Bonus5_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define Bonus5_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define Bonus5_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define Bonus5_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define Bonus5_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set nFAULT aliases
#define nFAULT_TRIS                 TRISAbits.TRISA5
#define nFAULT_LAT                  LATAbits.LATA5
#define nFAULT_PORT                 PORTAbits.RA5
#define nFAULT_WPU                  WPUAbits.WPUA5
#define nFAULT_OD                   ODCONAbits.ODCA5
#define nFAULT_ANS                  ANSELAbits.ANSELA5
#define nFAULT_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define nFAULT_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define nFAULT_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define nFAULT_GetValue()           PORTAbits.RA5
#define nFAULT_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define nFAULT_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define nFAULT_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define nFAULT_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define nFAULT_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define nFAULT_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define nFAULT_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define nFAULT_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set RB1 procedures
#define RB1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define RB1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define RB1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RB1_GetValue()              PORTBbits.RB1
#define RB1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define RB1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define RB1_SetPullup()             do { WPUBbits.WPUB1 = 1; } while(0)
#define RB1_ResetPullup()           do { WPUBbits.WPUB1 = 0; } while(0)
#define RB1_SetAnalogMode()         do { ANSELBbits.ANSELB1 = 1; } while(0)
#define RB1_SetDigitalMode()        do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set MasterSwitch aliases
#define MasterSwitch_TRIS                 TRISBbits.TRISB3
#define MasterSwitch_LAT                  LATBbits.LATB3
#define MasterSwitch_PORT                 PORTBbits.RB3
#define MasterSwitch_WPU                  WPUBbits.WPUB3
#define MasterSwitch_OD                   ODCONBbits.ODCB3
#define MasterSwitch_ANS                  ANSELBbits.ANSELB3
#define MasterSwitch_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define MasterSwitch_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define MasterSwitch_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define MasterSwitch_GetValue()           PORTBbits.RB3
#define MasterSwitch_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define MasterSwitch_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define MasterSwitch_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define MasterSwitch_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define MasterSwitch_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define MasterSwitch_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define MasterSwitch_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define MasterSwitch_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

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

// get/set OffButton aliases
#define OffButton_TRIS                 TRISBbits.TRISB5
#define OffButton_LAT                  LATBbits.LATB5
#define OffButton_PORT                 PORTBbits.RB5
#define OffButton_WPU                  WPUBbits.WPUB5
#define OffButton_OD                   ODCONBbits.ODCB5
#define OffButton_ANS                  ANSELBbits.ANSELB5
#define OffButton_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define OffButton_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define OffButton_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define OffButton_GetValue()           PORTBbits.RB5
#define OffButton_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define OffButton_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define OffButton_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define OffButton_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define OffButton_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define OffButton_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define OffButton_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define OffButton_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

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

// get/set RD4 procedures
#define RD4_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define RD4_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define RD4_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define RD4_GetValue()              PORTDbits.RD4
#define RD4_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define RD4_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define RD4_SetPullup()             do { WPUDbits.WPUD4 = 1; } while(0)
#define RD4_ResetPullup()           do { WPUDbits.WPUD4 = 0; } while(0)
#define RD4_SetAnalogMode()         do { ANSELDbits.ANSELD4 = 1; } while(0)
#define RD4_SetDigitalMode()        do { ANSELDbits.ANSELD4 = 0; } while(0)

// get/set RD5 procedures
#define RD5_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define RD5_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define RD5_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define RD5_GetValue()              PORTDbits.RD5
#define RD5_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define RD5_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define RD5_SetPullup()             do { WPUDbits.WPUD5 = 1; } while(0)
#define RD5_ResetPullup()           do { WPUDbits.WPUD5 = 0; } while(0)
#define RD5_SetAnalogMode()         do { ANSELDbits.ANSELD5 = 1; } while(0)
#define RD5_SetDigitalMode()        do { ANSELDbits.ANSELD5 = 0; } while(0)

// get/set old_nFAULT aliases
#define old_nFAULT_TRIS                 TRISDbits.TRISD6
#define old_nFAULT_LAT                  LATDbits.LATD6
#define old_nFAULT_PORT                 PORTDbits.RD6
#define old_nFAULT_WPU                  WPUDbits.WPUD6
#define old_nFAULT_OD                   ODCONDbits.ODCD6
#define old_nFAULT_ANS                  ANSELDbits.ANSELD6
#define old_nFAULT_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define old_nFAULT_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define old_nFAULT_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define old_nFAULT_GetValue()           PORTDbits.RD6
#define old_nFAULT_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define old_nFAULT_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define old_nFAULT_SetPullup()          do { WPUDbits.WPUD6 = 1; } while(0)
#define old_nFAULT_ResetPullup()        do { WPUDbits.WPUD6 = 0; } while(0)
#define old_nFAULT_SetPushPull()        do { ODCONDbits.ODCD6 = 0; } while(0)
#define old_nFAULT_SetOpenDrain()       do { ODCONDbits.ODCD6 = 1; } while(0)
#define old_nFAULT_SetAnalogMode()      do { ANSELDbits.ANSELD6 = 1; } while(0)
#define old_nFAULT_SetDigitalMode()     do { ANSELDbits.ANSELD6 = 0; } while(0)

// get/set DbgUart aliases
#define DbgUart_TRIS                 TRISDbits.TRISD7
#define DbgUart_LAT                  LATDbits.LATD7
#define DbgUart_PORT                 PORTDbits.RD7
#define DbgUart_WPU                  WPUDbits.WPUD7
#define DbgUart_OD                   ODCONDbits.ODCD7
#define DbgUart_ANS                  ANSELDbits.ANSELD7
#define DbgUart_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define DbgUart_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define DbgUart_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define DbgUart_GetValue()           PORTDbits.RD7
#define DbgUart_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define DbgUart_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define DbgUart_SetPullup()          do { WPUDbits.WPUD7 = 1; } while(0)
#define DbgUart_ResetPullup()        do { WPUDbits.WPUD7 = 0; } while(0)
#define DbgUart_SetPushPull()        do { ODCONDbits.ODCD7 = 0; } while(0)
#define DbgUart_SetOpenDrain()       do { ODCONDbits.ODCD7 = 1; } while(0)
#define DbgUart_SetAnalogMode()      do { ANSELDbits.ANSELD7 = 1; } while(0)
#define DbgUart_SetDigitalMode()     do { ANSELDbits.ANSELD7 = 0; } while(0)

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


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCAF5 pin functionality
 * @Example
    IOCAF5_ISR();
 */
void IOCAF5_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCAF5 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCAF5 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF5_SetInterruptHandler(MyInterruptHandler);

*/
void IOCAF5_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCAF5 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCAF5_SetInterruptHandler() method.
    This handler is called every time the IOCAF5 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF5_SetInterruptHandler(IOCAF5_InterruptHandler);

*/
extern void (*IOCAF5_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCAF5 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCAF5_SetInterruptHandler() method.
    This handler is called every time the IOCAF5 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF5_SetInterruptHandler(IOCAF5_DefaultInterruptHandler);

*/
void IOCAF5_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF3 pin functionality
 * @Example
    IOCBF3_ISR();
 */
void IOCBF3_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF3 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF3 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF3_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF3_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF3 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF3_SetInterruptHandler() method.
    This handler is called every time the IOCBF3 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF3_SetInterruptHandler(IOCBF3_InterruptHandler);

*/
extern void (*IOCBF3_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF3 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF3_SetInterruptHandler() method.
    This handler is called every time the IOCBF3 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF3_SetInterruptHandler(IOCBF3_DefaultInterruptHandler);

*/
void IOCBF3_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/