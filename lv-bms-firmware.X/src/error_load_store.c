/* 
 * File:   shutdown_reason.c
 * Author: Alex Mills am9g22
 *
 * Created on November 12, 2024, 8:06 PM
 */

#include <xc.h>
#include "mcc.h"
#include "error_load_store.h"
#include "logging.h"
#include "bq796xx.h"
#include "memory.h"

#define shutdown_reason_addr 0x00
#define lockout_reason_addr 0x01
#define lockout_cell_addr 0x02
#define lockout_value_addr 0x03


void save_shutdown_reason(shutdown_reason_t sd_reason) {
    DATAEE_WriteByte(shutdown_reason_addr, (uint8_t)sd_reason);
}

shutdown_reason_t load_shutdown_reason(void) {
    return DATAEE_ReadByte(shutdown_reason_addr);
}

void save_lockout_reason(lockout_reason_t lo_reason, uint8_t cell, int16_t fault_value) {
    uint8_t fault_value_hi = (uint8_t)(fault_value >> 8);
    uint8_t fault_value_lo = fault_value & 0xff;
    
    DATAEE_WriteByte(lockout_reason_addr, (uint8_t)lo_reason);
    DATAEE_WriteByte(lockout_cell_addr, cell);
    DATAEE_WriteByte(lockout_value_addr, fault_value_hi);
    DATAEE_WriteByte(lockout_value_addr + 1, fault_value_lo);
}

lockout_reason_t load_lockout_reason(void) {
    return DATAEE_ReadByte(lockout_reason_addr);
}

uint8_t load_lockout_cell(void) {
    return DATAEE_ReadByte(lockout_cell_addr);
}

int16_t load_lockout_value(void) {
    uint8_t fault_value_hi = DATAEE_ReadByte(lockout_value_addr);
    uint8_t fault_value_lo = DATAEE_ReadByte(lockout_value_addr + 1);
    
    return fault_value_hi << 8 | fault_value_lo;
}

void hard_fault_handler(lockout_reason_t lo_reason) {
    save_lockout_reason(lo_reason, 0, 0);
    SD796XX();
    RelayCtrl_SetLow();
    RESET();
}

void hard_fault_handler_2(lockout_reason_t lo_reason, uint8_t cell, int16_t value) {
    save_lockout_reason(lo_reason, cell, value);
    SD796XX();
    RelayCtrl_SetLow();
    RESET();
}
