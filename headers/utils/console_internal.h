#ifndef SMUG_UTILS_CONSOLE_INTERNAL_H
#define SMUG_UTILS_CONSOLE_INTERNAL_H

#include <utils/console_type.h>

typedef struct _Console {
    void (*write)(char* format, ...);
    void (*writeLine)(char* format, ...);
} _Console;

#endif /* SMUG_UTILS_CONSOLE_INTERNAL_H */
