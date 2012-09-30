/**
 * @file console.h
 * @brief Defines a debug console
 * @if doc_files
 * @ingroup smug_platform
 * @endif
 */

/**
 * @addtogroup smug_platform
 * @{
 */

#ifndef SMUG_UTILS_CONSOLE_H
#define SMUG_UTILS_CONSOLE_H

typedef struct Console {
    void (*write)(char* format, ...);
    void (*writeLine)(char* format, ...);
} Console;

#endif /* SMUG_UTILS_CONSOLE_H */

/**@}*/
