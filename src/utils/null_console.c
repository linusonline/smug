#include <stdarg.h>

#include <common.h>
#include <utils/console_internal.h>
#include <utils/null_console.h>

static void NullConsole_write(char* fmt, va_list args)
{
}

static void NullConsole_writeLine(char* fmt, va_list args)
{
}

Console* NullConsole_new()
{
    Console* console = allocate(Console);
    console->write = NullConsole_write;
    console->writeLine = NullConsole_writeLine;
    return console;
}

void NullConsole_delete(Console* nullConsole)
{
    free(nullConsole);
}
