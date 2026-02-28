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
#include "pack_interface.h"
#include "memory.h"

#define shutdown_reason_addr 0x00
#define shutdown_cell_addr 0x01
#define shutdown_value_addr 0x02 // 0x03

#define lockout_reason_addr 0x04
#define lockout_cell_addr 0x05
#define lockout_value_addr 0x06 // 0x07

#define shutdown_count_addr 0x08 // 0x09
#define lockout_count_addr 0x0A // 0x0B
#define charge_cycle_count_addr 0x0C // 0x0D

void save_value(uint16_t addr, uint16_t value) {
    uint8_t value_hi = (uint8_t)(value >> 8);
    uint8_t value_lo = value & 0xff;
    DATAEE_WriteByte(addr, value_hi);
    DATAEE_WriteByte(addr + 1, value_lo);
}

uint16_t load_value(uint16_t addr)  {
    uint8_t value_hi = DATAEE_ReadByte(addr);
    uint8_t value_lo = DATAEE_ReadByte(addr + 1);

    uint16_t value = (uint16_t)(value_hi << 8) | value_lo;
    // initialise to 0 if it is 65536 - the flash has been reset
    if(value == UINT16_MAX) {
        save_value(addr, 0);
        value = 0;
    }

    return value;
}

void save_charge_cycles(uint16_t cycles) {
    save_value(charge_cycle_count_addr, cycles);
}

uint16_t load_charge_cycles(void) {
    return load_value(charge_cycle_count_addr);
}


void save_shutdown_reason(shutdown_reason_t sd_reason, uint8_t cell, int16_t fault_value) {
    // increment shutdown count if it was error driven
    if((sd_reason != SHUTDOWN_REASON_NONE) && (sd_reason != SHUTDOWN_REASON_CHARGE_DONE)) {
        uint16_t sd_count = load_value(shutdown_count_addr);
        save_value(shutdown_count_addr, sd_count + 1);
    }

    // save shutdown reason
    DATAEE_WriteByte(shutdown_reason_addr, (uint8_t)sd_reason);
    DATAEE_WriteByte(shutdown_cell_addr, cell);
    save_value(shutdown_value_addr, (uint16_t)fault_value);
}

shutdown_reason_t load_shutdown_reason(void) {
    return DATAEE_ReadByte(shutdown_reason_addr);
}

uint8_t load_shutdown_cell(void) {
    return DATAEE_ReadByte(shutdown_cell_addr);
}

int16_t load_shutdown_value(void) {
    return (int16_t)load_value(shutdown_value_addr);
}

uint16_t load_shutdown_count(void) {
    return load_value(shutdown_count_addr);
}


void save_lockout_reason(lockout_reason_t lo_reason, uint8_t cell, int16_t fault_value) {
    // increment lockout count
    if(lo_reason != LOCKOUT_REASON_NONE) {
        uint16_t lo_count = load_value(lockout_count_addr);
        save_value(lockout_count_addr, lo_count + 1);
    }

    // save lockout reason
    DATAEE_WriteByte(lockout_reason_addr, (uint8_t)lo_reason);
    DATAEE_WriteByte(lockout_cell_addr, cell);
    save_value(lockout_value_addr, (uint16_t)fault_value);
}

lockout_reason_t load_lockout_reason(void) {
    return DATAEE_ReadByte(lockout_reason_addr);
}

uint8_t load_lockout_cell(void) {
    return DATAEE_ReadByte(lockout_cell_addr);
}

int16_t load_lockout_value(void) {
   return (int16_t)load_value(lockout_value_addr);
}

uint16_t load_lockout_count(void) {
    return load_value(lockout_count_addr);
}

void hard_fault_handler(lockout_reason_t lo_reason) {
    save_lockout_reason(lo_reason, 0, 0);
    pack_shutdown();
    RelayCtrl_SetLow();
    RESET();
}

void hard_fault_handler_2(lockout_reason_t lo_reason, uint8_t cell, int16_t value) {
    save_lockout_reason(lo_reason, cell, value);
    pack_shutdown();
    RelayCtrl_SetLow();
    RESET();
}
