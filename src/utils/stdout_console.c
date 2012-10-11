#include <stdarg.h>
#include <stdio.h>

#include <common.h>
#include <utils/console_internal.h>
#include <utils/stdout_console.h>

static void StdoutConsole_write(char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);

    vprintf(fmt, vl);

    va_end(vl);
}

static void StdoutConsole_writeLine(char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);

    vprintf(fmt, vl);

    va_end(vl);
    printf("\n");
}

Console* StdoutConsole_new()
{
    Console* console = allocate(Console);
    console->write = StdoutConsole_write;
    console->writeLine = StdoutConsole_writeLine;
    return console;
}

void StdoutConsole_delete(Console* stdoutConsole)
{
    free(stdoutConsole);
}
