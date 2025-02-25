/* 
 * File:   indicator_lights.h
 * Author: alexl
 *
 * Created on 04 February 2025, 18:41
 */

#ifndef INDICATOR_LIGHTS_H
#define	INDICATOR_LIGHTS_H

#include <xc.h>
#include "stdbool.h"

#ifdef	__cplusplus
extern "C" {
#endif
    #define NUM_LEDS 5
#define CHARGE_ANIM_STEP_TIME_MS 300
#define EMPTY_ANIM_STEP_TIME_MS 300
#define CRITICAL_ANIM_BLINK_TIME_MS 100
#define CRITICAL_ANIM_BLINK_SPACING_MS 800
    
    typedef enum {DISP_SOC, DISP_NUM} disp_mode_e;
    
    // initialise the LEDs
    void disp_init();
    // updates the display's animations
    void disp_update();
    // sets the mode of the indicator lights
    void set_disp_mode(disp_mode_e mode);
    
    
    
    //// displaying state of charge
    // displays an SOC value from 0 - 100 
    void disp_set_soc(uint8_t soc);
    // enables charging animation
    void disp_set_charging(bool charging);
    // enable critical level animation
    void disp_set_critical(bool critical);
    // enable empty level animation 
    void disp_set_empty(bool empty);
    
    //// displays a binary number on the digits
    void disp_set_number(int num);


#ifdef	__cplusplus
}
#endif

#endif	/* INDICATOR_LIGHTS_H */

