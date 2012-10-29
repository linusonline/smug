#include <stdarg.h>

#include <common.h>
#include <utils/console_internal.h>

void Console_write(Console* self, char* format, ...)
{
    va_list vl;
    va_start(vl, format);

    self->write(format, vl);

    va_end(vl);
}

void Console_writeLine(Console* self, char* format, ...)
{
    va_list vl;
    va_start(vl, format);

    self->writeLine(format, vl);

    va_end(vl);
}
