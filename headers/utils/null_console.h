#ifndef SMUG_UTILS_NULL_CONSOLE_H
#define SMUG_UTILS_NULL_CONSOLE_H

#include <utils/console.h>

/**
 *  A dummy console which doesn't do anything with the output written to it.
 */

Console* NullConsole_new();
void NullConsole_delete(Console* nullConsole);

#endif /* SMUG_UTILS_NULL_CONSOLE_H */
