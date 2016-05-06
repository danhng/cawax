//
// Created by dtn on 30/04/16.
//
#include "cawax_debug.h"
#include <stdio.h>


#define DEBUG_HEADER_INFO "[    DEBUG    ]    "

#define DEBUG_HEADER_FATAL "[    FATAL    ]    "
#define DEBUG_HEADER_ERR "[    ERR    ]    "
#define DEBUG_HEADER_WARN "[    WARN    ]    "




void customLog(LogLevel level, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);
}

void all(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);
}

void debug(const char * format, ...) {
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);
}
void info(const char * format, ...) {
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);

}
void warn(const char * format, ...) {
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);
}
void error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);
}

void fatal(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf(format, args);
    va_end(args);
}


