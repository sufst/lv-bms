/* 
 * File:   logging.h
 * Author: alexl
 *
 * Created on 20 May 2025, 15:19
 */

#ifndef LOGGING_H
#define	LOGGING_H

typedef enum {LOG_ERR, LOG_WARN, LOG_INFO, LOG_DBG} log_level_t;
log_level_t log_level = LOG_INFO;

void log_dbg( const char* format, ... );
void log_info( const char* format, ... );
void log_warn( const char* format, ... );
void log_err( const char* format, ... );

#endif	/* LOGGING_H */

