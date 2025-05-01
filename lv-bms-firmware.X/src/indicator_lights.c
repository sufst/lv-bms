/* 
 * File:   indicator_lights.h
 * Author: alexl
 *
 * Created on 04 February 2025, 18:41
 */

#include <xc.h>
#include "mcc.h"
#include "indicator_lights.h"
#include "pin_manager.h"
#include "millis.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    disp_mode_e _mode = DISP_NUM;
    int anim_state = 0;
    uint64_t _anim_timestamp = 0;
    uint64_t critical_timestamp = 0;
    uint8_t _soc = 0;
    uint8_t _num = 0;
    bool _charging = false;
    bool _critical = false;
    bool _empty = false;
    
    uint64_t _init_start_time = 0;
    bool init = false;
    
    void set_led(int led_no, bool val) {
        switch (led_no) {
            case 0:
                if (val) SOC1_SetHigh();
                else SOC1_SetLow();
                break;
            case 1:
                if (val) SOC2_SetHigh();
                else SOC2_SetLow();
                break;
            case 2:
                if (val) SOC3_SetHigh();
                else SOC3_SetLow();
                break;
            case 3:
                if (val) SOC4_SetHigh();
                else SOC4_SetLow();
                break;
            case 4:
                if (val) SOC5_SetHigh();
                else SOC5_SetLow();
                break;
            default:
                break;
        }
    }
    // used for advancing onto the next step of an animation if the delay has passed. time is 
    // kept track of using the timestamp
    void _anim_advance(int next_state, uint64_t delay_until_next_state, uint64_t* timestamp ) {
        uint64_t now = millis();
        if ((now - *timestamp) > delay_until_next_state) {
            anim_state = next_state;
            *timestamp = now;
        }
    }
    
    uint8_t disp_get_led_bars() {
        uint8_t led_bars = 0x00;
        
        uint64_t now = millis();
        
        // store what the pattern in the led bar graph should be
        for (int led_no = 0; led_no < NUM_LEDS; led_no++) {
            if(_soc > ((led_no) * (100/NUM_LEDS))) {
                led_bars |= 1 << led_no;
            }
        }

        if(_charging) {
            // Charging animation
            // apply charging mask where the bar scans upwards

            // update animation state
            if ((now - _anim_timestamp) > CHARGE_ANIM_STEP_TIME_MS) {
                anim_state++;
                if(anim_state == (NUM_LEDS + 1))
                    anim_state = 0;
                _anim_timestamp = now;
            }

            // mask off some of the leds depending on the state of the animation
            uint8_t charging_mask = 0x00;
            for(int i = 0; i < anim_state; i++) {
                charging_mask |= 1 << i;
            }
            led_bars |= 0x01;
            led_bars &= charging_mask;

        } else if (_empty) {
            // empty animation
            // update anim state
            if ((now - _anim_timestamp) > EMPTY_ANIM_STEP_TIME_MS) {
                anim_state = !(anim_state);
                _anim_timestamp = now;
            }

            // update led bars
            led_bars |= 0x01; // set the lowest LED so it blinks when close to empty regardless of soc
            led_bars = (anim_state) ? led_bars : 0;

        } 
        
        if (_critical) {
            // critical animation
            switch (anim_state){
                case 0:
                    // don't do anything to the leds
                    _anim_advance(1, CRITICAL_ANIM_BLINK_SPACING_MS, &critical_timestamp);
                    break;
                case 1:
                    led_bars = 0x0A; // flashing pattern
                    _anim_advance(2, CRITICAL_ANIM_BLINK_TIME_MS, &critical_timestamp);
                    break;
                case 2:
                    led_bars = 0x15;
                    _anim_advance(3, CRITICAL_ANIM_BLINK_TIME_MS, &critical_timestamp);
                    break;
                case 3:
                    led_bars = 0x0A;
                    _anim_advance(4, CRITICAL_ANIM_BLINK_TIME_MS, &critical_timestamp);
                    break;
                case 4:
                    led_bars = 0x15;
                    _anim_advance(0, CRITICAL_ANIM_BLINK_TIME_MS, &critical_timestamp);
                    break;
            }
        }
        return led_bars;
    }
    
    
    // initialise the LEDs
    void disp_init() {
        SOC1_SetDigitalMode();
        SOC1_SetDigitalOutput();
        SOC1_SetPushPull();
        SOC1_SetHigh();
        
        SOC2_SetDigitalMode();
        SOC2_SetDigitalOutput();
        SOC2_SetPushPull();
        SOC2_SetHigh();
        
        SOC3_SetDigitalMode();
        SOC3_SetDigitalOutput();
        SOC3_SetPushPull();
        SOC3_SetHigh();
        
        SOC4_SetDigitalMode();
        SOC4_SetDigitalOutput();
        SOC4_SetPushPull();
        SOC4_SetHigh();
        
        SOC5_SetDigitalMode();
        SOC5_SetDigitalOutput();
        SOC5_SetPushPull();
        SOC5_SetHigh();
        
        init = true;
        _init_start_time = millis();
    }
    
    // updates the display's animations
    void disp_update() {
        uint64_t now = millis();
        
        // on init illuminate all for a brief period, the turn off, then 
        //start displaying
        if(init) {
            if((now - _init_start_time) > 1000) {
                init = false;
            } else if((now - _init_start_time) > 500) {
                for(int i=0; i < NUM_LEDS; i++){
                    set_led(i, false);
                }
            } else {
                for(int i=0; i < NUM_LEDS; i++){
                    set_led(i, true);
                }
            }

        } else {
            uint8_t led_bars = 0x00; // is being used like an array, but bit stuffing allows anding masks
            switch (_mode) {
                case DISP_SOC:
                    led_bars = disp_get_led_bars();
                    break;
                case DISP_NUM:
                    led_bars = _num;
                    break;
                default:
                    break;
            }
            
            // render the LED bars onto the leds:
            for (char bit = 0; bit < 5; bit++) {
                set_led(bit, led_bars & (1<<(bit)));
            }
        }
    }
      
    // sets the mode of the indicator lights
    void set_disp_mode(disp_mode_e mode) {
        _mode = mode;
        anim_state = 0;
        _anim_timestamp = millis(); 
   }
    
    //// displaying state of charge
    // displays an SOC value from 0 - 100 
    void disp_set_soc(uint8_t soc) {
        if (_mode != DISP_SOC){
            set_disp_mode(DISP_SOC);
        }
        _soc = soc;
        disp_update();
    }
    
    // enables charging animation
    void disp_set_charging(bool charging) {
        _charging = charging;
        set_disp_mode(_mode);
        disp_update();
    }
    
    // enable critical level animation
    void disp_set_critical(bool critical) {
        _critical = critical;
        set_disp_mode(_mode);
        disp_update();
    }
    
    // enable empty level animation 
    void disp_set_empty(bool empty) {
        _empty = empty;
        set_disp_mode(_mode);
        disp_update();
    }
    
    //// displays a binary number on the digits
    void disp_set_number(uint8_t num) {
        _num = num;
        set_disp_mode(DISP_NUM);
        disp_update();
    }


#ifdef	__cplusplus
}
#endif
