/* 
 * File:   millis.h
 * Author: Alexander Mills (am9g22)
 *
 * Created on November 12, 2024, 7:52 PM
 */

#ifndef MILLIS_H
#define	MILLIS_H

#ifndef MAX_TIMERS
#define MAX_TIMERS 20
#endif

typedef uint64_t time_t;
typedef struct 
{
    bool configured, running, auto_restart;
    time_t start;
    time_t duration;
    time_t last_checked;
    void (*timer_done_cb)(); // callback 
} timer_t;



void millis_setup(void);
uint64_t millis(void);
void delay(uint64_t delay_ms);

// timer functions for managing arbitrary timers
void timer_init_count_up(timer_t *timer);
void timer_init_count_down(timer_t *timer, time_t duration);
void timer_set_done_cb(timer_t* timer, void(*timer_done_cb)()); // function that is called on timer expiration
void timer_set_auto_restart(timer_t* timer, bool auto_restart); // does the timer restart itself when done?
void timer_start(timer_t* timer);
void timer_cancel(timer_t *timer);

time_t timer_get_time_left(timer_t *timer);
time_t timer_get_time_last_checked(timer_t* timer); // timestamp where the timer was last checked
bool timer_get_done(timer_t *timer);


#endif	/* MILLIS_H */

