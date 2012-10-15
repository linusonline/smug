#ifndef SMUG_UTILS_CONSOLE_INTERNAL_H
#define SMUG_UTILS_CONSOLE_INTERNAL_H

#include <utils/console.h>
#include <stdarg.h>

typedef struct _Console {
    void (*write)(char* format, va_list args);
    void (*writeLine)(char* format, va_list args);
} _Console;

#endif /* SMUG_UTILS_CONSOLE_INTERNAL_H */
