#include <xc.h>
#include "mcc.h"
#include <stdarg.h>
#include "logging.h"


void log_dbg( const char* format, ... ) {
    if (log_level >= LOG_DBG) {
        va_list args;
        va_start( args, format );
        printf("DBG : ");
        vprintf(format, args );
        putchar('\n');
        va_end( args );
    }
}

void log_info( const char* format, ... ) {
    if (log_level >= LOG_INFO) {
        va_list args;
        va_start( args, format );
        printf("INFO: ");
        vprintf(format, args );
        putchar('\n');
        va_end( args );
    }
}

void log_warn( const char* format, ... ) {
    if (log_level >= LOG_WARN) {
        va_list args;
        va_start( args, format );
        printf("WARN: ");
        vprintf(format, args );
        putchar('\n');
        va_end( args );
    }
}

void log_err( const char* format, ... ) {
    if (log_level >= LOG_ERR) {
        va_list args;
        va_start( args, format );
        printf("ERR : ");
        vprintf(format, args );
        putchar('\n');
        va_end( args );
    }
}