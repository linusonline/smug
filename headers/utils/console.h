#ifndef SMUG_UTILS_CONSOLE_H
#define SMUG_UTILS_CONSOLE_H

#include <utils/console_type.h>

void Console_write(Console* self, char* format, ...);
void Console_writeLine(Console* self, char* format, ...);

#endif /* SMUG_UTILS_CONSOLE_H */
