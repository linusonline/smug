#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <common.h>
#include <utils/string_console.h>

// TODO: Put these in a new type, inheriting from Console, to enable several
// string console objects to be used simultaneously.
static char* gBuffer = NULL;
static int gBufferMaxSize = 0;
static int gBufferPointer = 0;

static void StringConsole_write(char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);

    int writtenChars = vsprintf(gBuffer + gBufferPointer, fmt, vl);
    gBufferPointer += writtenChars;

    va_end(vl);
}

static void StringConsole_writeLine(char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);

    int writtenChars = vsprintf(gBuffer + gBufferPointer, fmt, vl);
    gBufferPointer += writtenChars;
    gBuffer[gBufferPointer++] = '\n';

    va_end(vl);
    printf("\n");
}

Console* StringConsole_new(int maxSize)
{
    Console* console = allocate(Console);
    console->write = StringConsole_write;
    console->writeLine = StringConsole_writeLine;
    gBufferMaxSize = maxSize;
    gBuffer = allocatev(char, maxSize + 1);
    StringConsole_clearBuffer();
    return console;
}

void StringConsole_setMaxSize(int newMaxSize)
{
    char* newBuffer = allocatev(char, newMaxSize + 1);
    memcpy(newBuffer, gBuffer, min(newMaxSize, gBufferMaxSize));
    gBufferMaxSize = newMaxSize;
}

void StringConsole_delete(Console* stringConsole)
{
    free(stringConsole);
    free(gBuffer);
}

char* StringConsole_getBuffer()
{
    return gBuffer;
}

void StringConsole_clearBuffer()
{
    memset(gBuffer, 0, gBufferMaxSize + 1);
    gBufferPointer = 0;
}
