/*******************************************************************************
 *         File: can_interface.c
 *       Author: Alexander Mills (am9g22)
 *         Date: 2025-02-26 13:10:46
 *  Description: the can interface for the lvbms
 ******************************************************************************/


#include <xc.h>
#include "can_interface.h"
#include "mcc.h"
#include "millis.h"
#include "stdio.h"
#include "logging.h"

// timers
uint64_t sending_timer, critical_warning_timer, lockout_message_timer;

// pointers
voltage_t* voltages_p;
temp_t* temps_p;
current_t* curr_p;
uint8_t* SOC_p;

can_status_byte_t status_byte;
bool sensor_sending_en;

lockout_reason_t lockout_reason;
bool lockout_set;
uint8_t lockout_cell_index;
uint16_t lockout_cell_value;

uint16_t charge_cycles, shutdown_count, lockout_count;

void can_recieve_handler() {
    CAN_MSG_OBJ rx_msg;
    CAN1_ReceiveFrom(FIFO1, &rx_msg);
    
    log_info("can message recieved");
    // TODO some kind of command interface - emulate the ORION BMS for the LV battery
}

void tx_message(uint8_t offset, uint8_t* message_body, uint8_t len) { 
    CAN_MSG_OBJ tx_msg;
    tx_msg.msgId = CAN_EID + offset;
    tx_msg.field.formatType = CAN_2_0_FORMAT;
    tx_msg.field.brs = CAN_NON_BRS_MODE;
    tx_msg.field.dlc = len;
    tx_msg.field.frameType = CAN_FRAME_DATA;
    tx_msg.field.idType = CAN_FRAME_EXT;
    tx_msg.data = message_body;
    
    int trys;
    for(trys = 10; trys != 0; trys--){
        if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_TXQ) & CAN_TX_FIFO_AVAILABLE)) {
            CAN1_Transmit(CAN1_TX_TXQ, &tx_msg);
            break;
        }
    }
    
    if(trys == 0) {
        log_warn("ran out of tries to send a CAN message");
    }
    
}

void send_sensor_message() {
    // voltage 
    uint8_t voltage_message[6];
    for (int i = 0; i < 3; i++) {
        voltage_t v = voltages_p[i];
        voltage_message[i * 2] = v & 0xff ;
        voltage_message[i * 2 + 1] = v >> 8;
    }
    tx_message(CAN_VOLTAGE_MESSAGE_OFFSET, voltage_message, 6);
    
    delay(1);
    // temperatures
    uint8_t temp_message[3];
    for (int i = 0; i < 3; i++) {
        temp_t t = temps_p[i];
        temp_message[i] = (uint8_t)t;
    }
    tx_message(CAN_TEMPS_MESSAGE_OFFSET, temp_message, 3);
    
    delay(1);
    // current
    uint8_t current_message[2]; 
    current_t c = *curr_p;
    current_message[0] = (uint8_t)(c & 0xff);
    current_message[1] = (uint8_t)(c >> 8);
    tx_message(CAN_CURR_MESSAGE_OFFSET, current_message, 2);
}

void send_status_message() {
    uint8_t status_message[8];
    status_message[0] = (uint8_t)status_byte;
    status_message[1] = *SOC_p;
    status_message[2] = charge_cycles & 0xff;
    status_message[3] = charge_cycles >> 8;
    status_message[4] = shutdown_count & 0xff;
    status_message[5] = shutdown_count >> 8;
    status_message[6] = lockout_count & 0xff;
    status_message[7] = lockout_count >> 8;
    tx_message(CAN_STATUS_MESSAGE_OFFSET, status_message, 8);
}

void send_empty_warning() {
    uint8_t empty_message[0];
    tx_message(CAN_EMPTY_MESSAGE_OFFSET, empty_message, 0);
}

void send_critical_warning(can_critical_byte_t critical_byte, uint8_t cell_index, uint16_t critical_value){
    uint8_t critical_message[4];
    critical_message[0] = (uint8_t)critical_byte;
    critical_message[1] = cell_index;
    critical_message[2] = critical_value & 0xff;
    critical_message[3] = critical_value >> 8;
    tx_message(CAN_CRITICAL_WARNING_OFFSET, critical_message, 4);
}

void send_lockout_message() {
    uint8_t lockout_message[4];
    lockout_message[0] = (uint8_t)lockout_reason;
    lockout_message[1] = lockout_cell_index;
    lockout_message[2] = lockout_cell_value >> 8;
    lockout_message[3] = lockout_cell_value & 0xff;
    tx_message(CAN_LOCKOUT_OFFSET, lockout_message, 4);
}

void can_init() {   
    CAN1_Initialize();
    CAN1_SetFIFO1NotEmptyHandler(can_recieve_handler);
    
    while (CAN1_OperationModeGet() != CAN_NORMAL_2_0_MODE) {
        CAN_OP_MODE_STATUS status = CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);
        if (CAN_OP_MODE_REQUEST_SUCCESS != status) {
            log_err("error initialising the can interface\n");
            delay(1000);
        }
    }
}

// updates can interface
void can_update() {
    uint64_t now = millis();
    
    // sensor message sending
    if((now - sending_timer) > CAN_SENSOR_SENDING_INTERVAL && sensor_sending_en) {
        sending_timer = now;
        send_sensor_message();
        delay(1);
        send_status_message();

        // empty warning sending
        if(*SOC_p < CAN_EMPTY_WARNING_THRESHOLD) {
            send_empty_warning();
        }
    }
    
    // lockout
    if((now - lockout_message_timer) > CAN_LOCKOUT_SENDING_INTERVAL && lockout_set) {
        lockout_message_timer = now;
        send_lockout_message();
        delay(1);
        send_status_message();
    }
}

// these give the can interface pointers to the variables 
// that it will read to build the can messages
void can_register_voltages(voltage_t* voltages) {
    voltages_p = voltages;
}

void can_register_temps(temp_t* temps) {
    temps_p = temps;
}

void can_register_current(current_t* current) {
    curr_p = current;
}

void can_register_SOC(uint8_t* soc) {
    SOC_p = soc;
}

// update some fields in the status
void can_set_status(can_status_byte_t new_status_byte) {
    status_byte = new_status_byte;
}

void can_set_charge_cycles(uint16_t new_charge_cycles) {
    charge_cycles = new_charge_cycles;
}

void can_set_shutdown_count(uint16_t new_shutdown_count) {
    shutdown_count = new_shutdown_count;
}

void can_set_lockout_count(uint16_t new_lockout_count) {
    lockout_count = new_lockout_count;
}

// turns on and off the data sending
void can_sensor_sending_enable(bool enabled) {
    sensor_sending_en = enabled;
}

// enabled the lockout message
void can_set_lockdout(lockout_reason_t new_lockout_reason, uint8_t new_cell_index, uint16_t new_dire_value) {
    lockout_reason = new_lockout_reason;
    lockout_cell_index = new_cell_index;
    lockout_cell_value = new_dire_value;
    lockout_set = true;
}

void can_clear_lockout() {
    lockout_set = false;
}