/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UNITS_H
#define	UNITS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "inttypes.h"

#define VOLTAGE_MULTIPLIER (uint16_t)1024
#define CURRENT_MULTIPLIER (uint16_t)512
#define TEMP_MULTIPLIER 1

#define V(volt) (voltage_t)(volt * VOLTAGE_MULTIPLIER)
#define A(curr) (current_t)(curr * CURRENT_MULTIPLIER)
#define C(temp) (temp_t)(temp * TEMP_MULTIPLIER)

typedef uint16_t voltage_t; 
typedef int16_t current_t; 
typedef int8_t temp_t;

#define VOLTAGE_MAX UINT16_MAX
#define CURRENT_MAX INT16_MAX
#define CURRENT_MIN INT16_MIN
#define TEMP_MAX INT8_MAX

typedef union v {
        voltage_t v_value;
        current_t c_value;
        temp_t    t_value;
    } unit_value_u;

#ifndef min
#define min(a,b) ((a<b) ?a:b)
#endif

#endif	/* UNITS_H */

