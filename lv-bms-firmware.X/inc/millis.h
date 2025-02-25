/* 
 * File:   millis.h
 * Author: Alexander Mills (am9g22)
 *
 * Created on November 12, 2024, 7:52 PM
 */

#ifndef MILLIS_H
#define	MILLIS_H

void millis_setup(void);
uint64_t millis(void);
void delay(uint64_t delay_ms);

void start_timer(uint64_t length_ms);
bool time_up();

#endif	/* MILLIS_H */

