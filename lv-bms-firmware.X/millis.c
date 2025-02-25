/* 
 * File:   millis.c
 * Author: Alexander Mills (am9g22)
 *
 * Created on November 12, 2024, 7:52 PM
 */

#include <xc.h>  
#include "mcc_generated_files/mcc.h"
#include "millis.h"

 void millis_hook(uint64_t uptime);// define this in main to run a task once per ms

volatile uint64_t uptime = 0;
static uint64_t timer_end = 0;

void TMR0_uptime_ISR(void) {
    uptime += 1;
    millis_hook(uptime);
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

void delay(uint64_t delay_ms) {
    uint64_t start = millis();
    
    while(millis() < (start + delay_ms)){
        CLRWDT();
    };
}

void start_timer(uint64_t length_ms) {
    timer_end = millis() + length_ms;
}

bool time_up() {
    return millis() > timer_end;
}