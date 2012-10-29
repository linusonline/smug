#ifndef SMUG_UTILS_CONSOLE_H
#define SMUG_UTILS_CONSOLE_H

struct _Console;
typedef struct _Console Console;

void Console_write(Console* self, char* format, ...);
void Console_writeLine(Console* self, char* format, ...);

#endif /* SMUG_UTILS_CONSOLE_H */
