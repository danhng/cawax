/**
 * wrapper for log4c lib.
 * //todo doc
 */
#ifndef CAWAX_DEBUG_H
#define CAWAX_DEBUG_H
#include <stdarg.h>
typedef enum {
    /** fatal */	LOG4C_PRIORITY_FATAL	= 000,
    /** alert */	LOG4C_PRIORITY_ALERT	= 100,
    /** crit */	      	LOG4C_PRIORITY_CRIT	= 200,
    /** error */	LOG4C_PRIORITY_ERROR	= 300,
    /** warn */	      	LOG4C_PRIORITY_WARN	= 400,
    /** notice */	LOG4C_PRIORITY_NOTICE	= 500,
    /** info */	      	LOG4C_PRIORITY_INFO	= 600,
    /** debug */	LOG4C_PRIORITY_DEBUG	= 700,
    /** trace */	LOG4C_PRIORITY_TRACE	= 800,
    /** notset */	LOG4C_PRIORITY_NOTSET	= 900,
    /** unknown */	LOG4C_PRIORITY_UNKNOWN	= 1000
} log4c_priority_level_t;
#define debuggerName "cawaxLogger"

typedef log4c_priority_level_t LogLevel;
void initDebugger();
void customLog(LogLevel level, const char *format, ...);
void all(const char * format, ...);
void debug(const char * format, ...);
void info(const char * format, ...);
void warn(const char * format, ...);
void error(const char* format, ...);
void fatal(const char* format, ...);
#define setLoggingLevel(logLevel) log4c_category_set_priority(logger, loglevel)
#define disableLogging setLoggingLevel(LOG4C_PRIORITY_FATAL)
#endif


