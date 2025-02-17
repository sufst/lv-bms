/* 
 * File:   millis.c
 * Author: Alexander Mills (am9g22)
 *
 * Created on November 12, 2024, 7:52 PM
 */

#include <xc.h>  
#include "mcc_generated_files/mcc.h"
#include "millis.h"

volatile uint64_t uptime = 0;

void TMR0_uptime_ISR(void) {
    uptime += 1;
}

void millis_setup(void) {
    TMR0_SetInterruptHandler(TMR0_uptime_ISR);
    TMR0_StartTimer();
}

uint64_t millis(void) {
    INTERRUPT_GlobalInterruptDisable();
    uint64_t ret = uptime;
    INTERRUPT_GlobalInterruptEnable();
    return ret;
}

