/* 
 * File:   millis.c
 * Author: Alexander Mills (am9g22)
 *
 * Created on November 12, 2024, 7:52 PM
 */

#include <xc.h>  
#include "mcc.h"
#include "millis.h"
#include "math.h"
#include "logging.h"

void millis_hook(time_t uptime);// define this in main to run a task once per ms

void _update_timers();
bool _add_timer(timer_t *timer); // bool -> success

volatile uint64_t uptime = 0;
timer_t *timer_list[MAX_TIMERS]; // list of initialised timers to keep updated
uint16_t timer_count = 0;

void TMR0_uptime_ISR(void) {
    uptime += 1;
    millis_hook(uptime);
    // _update_timers();
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

// timer functions for managing arbitrary timers
void timer_init_count_up(timer_t* timer) {
    timer->running = false;
    timer->auto_restart = false;
    timer->start = 0;
    timer->duration = UINT64_MAX / 2; // less than max to avoid overflows
    timer->last_checked = 0;
    timer->timer_done_cb = NULL;

    // add to timer list
    if (!_add_timer(timer)) return;

    timer->configured = true; 
}

void timer_init_count_down(timer_t* timer, time_t duration) {
    timer->running = false;
    timer->auto_restart = false;
    timer->start = 0;
    timer->duration = duration;
    timer->last_checked = 0;
    timer->timer_done_cb = NULL;

    // add to timer list
    if(!_add_timer(timer)) return;

    timer->configured = true;
}

void timer_set_done_cb(timer_t* timer, void(*timer_done_cb)()) {
    timer->timer_done_cb = timer_done_cb;
}

void timer_set_auto_restart(timer_t* timer, bool auto_restart) {
    timer->auto_restart = true;
}

void timer_start(timer_t* timer) {
    if(timer->configured) {
        time_t now = millis();
        timer->start = now;
        timer->last_checked = now;
    
        timer->running = true;
    } else {
        log_info("failed to start timer at %p - not yet configured", timer);
    }
}
void timer_cancel(timer_t* timer) {
    timer->running = false;
    timer->start = 0;
}

time_t timer_get_time_left(timer_t* timer) {
    time_t now = millis();
    timer->last_checked = now;

    if (timer->start + timer->duration > now) {
        return timer->start + timer->duration - now;
    } else {
        return 0;
    }
}

time_t timer_get_time_last_checked(timer_t* timer) {
    return timer->last_checked;
}

bool timer_get_done(timer_t* timer) {
    return timer_get_time_left(timer) == 0;
}

void _update_timers () {
    time_t now = millis();

    for (uint16_t i = 0; i < timer_count; i++) {
        timer_t *timer = timer_list[i];

        if (timer->start + timer->duration <= now) {
            if(timer->running) {
                timer->timer_done_cb();
                
                if(timer->auto_restart) {
                    timer->start = now;
                } else {
                    timer->running = false;
                }
            }
        }
    }
}

bool _add_timer(timer_t *timer) {
    // add to timer list
    if (timer_count == MAX_TIMERS) {
        log_err("In trying to init timer at %p, hit max number of timers (%d), please define and increase MAX_TIMERS", timer, MAX_TIMERS);
        return false;
    }
    timer_list[timer_count++] = timer;
    return true;
}