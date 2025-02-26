/* 
 * File:   error_load_store.h
 * Author: Alex Mills am9g22
 *
 * Created on November 12, 2024, 8:06 PM
 */

#ifndef ERROR_LOAD_STORE_H
#define	ERROR_LOAD_STORE_H

#define SHUTDOWN_REASON_ADDR TODO

#include "can_interface.h"

typedef enum { 
    SHUTDOWN_REASON_NONE, 
    SHUTDOWN_REASON_UNDER_V,
    SHUTDOWN_REASON_OVER_V,
    SHUTDOWN_REASON_UNDER_TEMP,
    SHUTDOWN_REASON_OVER_TEMP,
    SHUTDOWN_REASON_OVER_CURR,
    SHUTDOWN_REASON_WDT,
} shutdown_reason_t;
             
/**
 * save_shutdown_reason
 * @param err
 * 
 *  Saves the a shutdown reason to flash
 */
void save_shutdown_reason(shutdown_reason_t sd_reason);

/**
 * load_shutdown_reason
 * @return 
 * gets the shutdown reason that is stored in flash
 */
shutdown_reason_t load_shutdown_reason(void);

/**
 * save_lockout_reason
 * @param lo_reason
 */
void save_lockout_reason(lockout_reason_t lo_reason);

/**
 * load_lockout_reason
 * @returns last saved lockout reason
 */
lockout_reason_t load_lockout_reason(void);

#endif	/* ERROR_LOAD_STORE_H */

