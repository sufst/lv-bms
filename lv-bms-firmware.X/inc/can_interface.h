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
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>
#include "units.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#define CAN_NODE_BASE_ID 0x4321000
#define CAN_EMPTY_WARNING_THRESHOLD 5 //%
    
#define CAN_SENSOR_SENDING_INTERVAL 128 //ms#define CAN_EMPTY_WARNING_SENDING_INTERVAL 128 //ms
#define CAN_CRITICAL_SENDING_INTERVAL 64 //ms
#define CAN_LOCKOUT_SENDING_INTERVAL 1024 //ms
    
enum {
    CAN_VOLTAGE_MESSAGE_OFFSET = 0x00,
    CAN_TEMPS_MESSAGE_OFFSET = 0x01,
    CAN_CURR_MESSAGE_OFFSET = 0x02,
    CAN_STATUS_MESSAGE_OFFSET = 0x03,
    CAN_EMPTY_MESSAGE_OFFSET = 0x04,
    CAN_CRITICAL_WARNING_OFFSET = 0x05,
    CAN_LOCKOUT_OFFSET = 0x06,
} CAN_MESSAGE_ID_OFFSETS;
    
typedef enum  {
    CAN_POWERED_DOWN = 0x00,
    CAN_DISCHARING = 0x01,
    CAN_CHARING = 0x02,
    CAN_EMPTY = 0x03,
    CAN_FULL = 0x04,
    CAN_LOCKED_OUT = 0x05
} can_status_byte_t;

typedef enum {
    CAN_CRITICIAL_VOLGAGE = 0x01,
    CAN_CRITICAL_TEMP = 0x02,
    CAN_CRITICAL_CURRENT = 0x03,
} can_critical_byte_t;

typedef enum {
    LOCKOUT_OVERVOLT = 0x01,
    LOCKOUT_UNDERVOLT = 0x02,
    LOCKOUT_OVERTEMP = 0x03,
    LOCKOUT_UNDERTEMP = 0x04,
    LOCKOUT_OVERCURRENT = 0x05,
} lockout_reason_t;

// initialise the can interface
void can_init();

// updates can interface
void can_update();

void send_sensor_message();

// these give the can interface pointers to the variables 
// that it will read to build the can messages
void can_register_voltages(voltage_t* voltages);
void can_register_temps(temp_t* temps);
void can_register_current(current_t* current);
void can_register_SOC(uint8_t* soc);

// update some fields in the status
void can_set_status(can_status_byte_t status_byte);
void can_set_charge_cycles(uint16_t charge_cycles);
void can_set_shutdown_count(uint16_t shutdown_count);
void can_set_lockout_count(uint16_t lockout_count);

// turns on and off the data sending
void can_sending_enable(bool enabled);

// sends a critical warning
void send_critical_warning(can_critical_byte_t critical_byte, uint8_t cell_index, uint16_t critical_value);

// enabled the lockout message
void set_lockdout(lockout_reason_t lockout_reason, uint8_t cell_index, uint16_t dire_value);
void clear_lockout();

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

