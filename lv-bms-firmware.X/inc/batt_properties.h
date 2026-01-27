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
#ifndef BATT_PROPERTIES_H
#define	BATT_PROPERTIES_H

#include "units.h"
#include "millis.h"

#define CELL_COUNT 3

typedef enum  { UNIT_VOLTAGE, UNIT_CURRENT, UNIT_TEMP } unit_e;
typedef enum  { BOUND_UPPER, BOUND_LOWER } bound_e;
typedef struct {
  bound_e bound;
  voltage_t value;
  time_t duration;
} voltage_condition_t;

typedef struct {
  bound_e bound;
  current_t value;
  time_t duration;
} current_condition_t;

typedef struct {
  bound_e bound;
  temp_t value;
  time_t duration;
} temp_condition_t;


//                                                                          Possible values based on datasheet: https://cdn03.plentymarkets.com/i9a0e0hd8l6w/frontend/Datenblaeter/Molicell/INR21700P45B/p45b-datasheet.pdf
// Critical values - cause a warning
current_condition_t CRITICAL_OVER_CURRENT = { .bound = BOUND_UPPER, .value = A(30), .duration = 500 };
voltage_condition_t CRITICAL_OVER_VOLTAGE = { .bound = BOUND_UPPER, .value = V(4.2), .duration = 500 };
voltage_condition_t CRITICAL_UNDER_VOLTAGE = { .bound = BOUND_LOWER, .value = V(2.9), .duration = 500 };
temp_condition_t CRITICAL_CHARGING_OVER_TEMP = { .bound = BOUND_UPPER, .value = 30, .duration = 1000 };
temp_condition_t CRITICAL_CHARGING_UNDER_TEMP = { .bound = BOUND_LOWER, .value = 10, .duration = 1000 };
temp_condition_t CRITICAL_DISCHARGING_OVER_TEMP = { .bound = BOUND_UPPER, .value = 30, .duration = 1000 };
temp_condition_t CRITICAL_DISCHARGING_UNDER_TEMP = { .bound = BOUND_LOWER, .value = -30, .duration = 1000 };

// Shutdown values - cause battery to power off
current_condition_t SHUTDOWN_OVER_CURRENT = { .bound = BOUND_UPPER, .value = A(35), .duration = 500 };
voltage_condition_t SHUTDOWN_OVER_VOLTAGE = { .bound = BOUND_UPPER, .value = V(4.25), .duration = 500 };
voltage_condition_t SHUTDOWN_UNDER_VOLTAGE = { .bound = BOUND_LOWER, .value = V(2.7), .duration = 500 };
temp_condition_t SHUTDOWN_CHARGING_OVER_TEMP = { .bound = BOUND_UPPER, .value = 55, .duration = 1000 };
temp_condition_t SHUTDOWN_CHARGING_UNDER_TEMP = { .bound = BOUND_LOWER, .value = 5, .duration = 1000 };
temp_condition_t SHUTDOWN_DISCHARGING_OVER_TEMP = { .bound = BOUND_UPPER, .value = 60, .duration = 1000 };
temp_condition_t SHUTDOWN_DISCHARGING_UNDER_TEMP = { .bound = BOUND_LOWER, .value = -35, .duration = 1000 };
// TODO Replace with actual values
// exceeding these values damages the system so enter lockout
#define LOCKOUT_OVER_VOLTAGE                    V(4.3)
#define LOCKOUT_UNDER_VOLTAGE                   V(2.5)
#define LOCKOUT_CHARGING_OVER_TEMP              60
#define LOCKOUT_CHARGING_UNDER_TEMP             0
#define LOCKOUT_DISCHARGING_OVER_TEMP           80
#define LOCKOUT_DISCHARGING_UNDER_TEMP          -40

// const condition_t LOCKOUT_OVER_VOLTAGE = {.unit = UNIT_VOLTAGE, .bound = BOUND_UPPER, .v.v}


#define RELAY_COIL_R 160 // ohms
#define IDLE_CURRENT -A(0)

#define BALANCE_START_TH V(0.2)
#define BALANCE_END_TH V(0.05)

#define CHARGING_END_CURRENT A(0.03)
#define CHARGING_END_TIME_MS 300e3 // seconds

#endif	/* BATT_PROPERTIES_H */

