/* 
 * File:   error_load_store.h
 * Author: Alex Mills am9g22
 *
 * Created on November 12, 2024, 8:06 PM
 */

#ifndef ERROR_LOAD_STORE_H
#define	ERROR_LOAD_STORE_H

#define SHUTDOWN_REASON_ADDR TODO

#include "error_types.h"
             
/**
 * save_shutdown_reason
 * @param err
 * 
 *  Saves the a shutdown reason to flash
 */
void save_shutdown_reason(shutdown_reason_t sd_reason, uint8_t cell, int16_t fault_value);

/**
 * load_shutdown_reason
 * @return 
 * gets the shutdown reason that is stored in flash
 */
shutdown_reason_t load_shutdown_reason(void);
uint8_t load_shutdown_cell(void);
int16_t load_shutdown_value(void);

/**
 * save_lockout_reason
 * @param lo_reason
 */
void save_lockout_reason(lockout_reason_t lo_reason, uint8_t cell, int16_t fault_value);

/**
 * load_lockout_reason
 * @returns last saved lockout reason
 */
lockout_reason_t load_lockout_reason(void);
uint8_t load_lockout_cell(void);
int16_t load_lockout_value(void);


// deals with if there is an unrecoverable error
// saves the error cause then puts the system into lockout
void hard_fault_handler(lockout_reason_t lo_reason);
void hard_fault_handler_2(lockout_reason_t lo_reason, uint8_t cell, int16_t value);

#endif	/* ERROR_LOAD_STORE_H */

