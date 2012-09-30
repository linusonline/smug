#ifndef SMUG_UTILS_STRING_CONSOLE_H
#define SMUG_UTILS_STRING_CONSOLE_H

#include <utils/console.h>

/* It is YOUR responsibility to not overflow the string buffer! Don't write more
 * than you know can fit. Only one of these allowed at a time.
 */

Console* StringConsole_new(int maxSize);
void StringConsole_delete(Console* stringConsole);
void StringConsole_setMaxSize(int newMaxSize);
char* StringConsole_getBuffer(void);
void StringConsole_clearBuffer(void);

#endif /* SMUG_UTILS_STRING_CONSOLE_H */

/**@}*/
