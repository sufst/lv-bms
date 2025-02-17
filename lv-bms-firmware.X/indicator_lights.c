/* 
 * File:   indicator_lights.h
 * Author: alexl
 *
 * Created on 04 February 2025, 18:41
 */

#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "indicator_lights.h"
#include "mcc_generated_files/pin_manager.h"
#include "millis.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    disp_mode_e _mode = DISP_NUM;
    int anim_state = 0;
    uint64_t _anim_timestamp = 0;
    uint8_t _soc = 0;
    uint8_t _num = 0;
    bool charging = false;
    bool critical = false;
    bool empty = false;
    
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
    
    uint8_t disp_get_led_bars() {
        uint8_t led_bars = 0x00;
        
        uint64_t now = millis();
        
        // store what the pattern in the led bar graph should be
        for (int led_no = 0; led_no < NUM_LEDS; led_no++) {
            if(_soc > (led_no * (100/NUM_LEDS))) {
                led_bars |= 1 << led_no;
            }
        }

        if(charging) {
            // Charging animation
            // apply charging mask where the bar scans upwards

            // update animation state
            if ((now - _anim_timestamp) > CHARGE_ANIM_STEP_TIME_MS) {
                anim_state++;
                if(anim_state == NUM_LEDS)
                    anim_state = 0;
                _anim_timestamp = now;
            }

            // mask off some of the leds depending on the state of the animation
            uint8_t charging_mask = 0x00;
            for(int i = 0; i < anim_state; i++) {
                charging_mask |= 1 << i;
            }
            led_bars &= charging_mask;

        } else if (empty) {
            // empty animation
            // update anim state
            if ((now - _anim_timestamp) > EMPTY_ANIM_STEP_TIME_MS) {
                anim_state = !(anim_state);
                _anim_timestamp = now;
            }

            // update led bars
            led_bars = (anim_state) ? led_bars : 0;

        } else if (critical) {
            // critical animation
            if(anim_state == 0) { // high section
                // don't affect led_bars
                // start blink
                if ((now -_anim_timestamp) > CRITICAL_ANIM_BLINK_SPACING_MS) {
                    anim_state = 1;
                    _anim_timestamp = now;
                }
            } else if (anim_state == 1) { // low blink
                // blink led bars
                led_bars = 0;
                if((now - _anim_timestamp) > CRITICAL_ANIM_BLINK_TIME_MS) {
                    anim_state = 0;
                    _anim_timestamp = now;
                }
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
                set_led(bit, _num & (1<<(bit)));
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
        _soc = soc;
        set_disp_mode(DISP_SOC);
        disp_update();
    }
    
    // enables charging animation
    void disp_set_charging(bool charging) {
        charging = charging;
        set_disp_mode(_mode);
        disp_update();
    }
    
    // enable critical level animation
    void disp_set_critical(bool critical) {
        critical = critical;
        set_disp_mode(_mode);
        disp_update();
    }
    
    // enable empty level animation 
    void disp_set_empty(bool empty) {
        empty = empty;
        set_disp_mode(_mode);
        disp_update();
    }
    
    //// displays a binary number on the digits
    void disp_set_number(int num) {
        _num = num;
        set_disp_mode(DISP_NUM);
        disp_update();
    }


#ifdef	__cplusplus
}
#endif
