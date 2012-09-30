#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <common.h>
#include <utils/console.h>
#include <utils/stdout_console.h>
#include <utils/linkedlist.h>
#include <utils/log.h>

// As default, include all log output
static int   gCurrentLogLevel = LOG_ALL;
static char* gFormatString = NULL;
static char* gIndentString;
static LinkedList* gPrefixStack = NULL;
static const int MAX_MESSAGE_SIZE = 1024;
static Console* gConsole = NULL;

static BOOL _isInitialized(void)
{
    return NULL != gPrefixStack;
}

BOOL Log_init(Console* console)
{
    gConsole = console;

    // Allocate memory for prefix stack
    gPrefixStack = LinkedList_new();

    // Set default format string
    #ifdef NDEBUG
    Log_setFormatString("%indent%%message%");
    gIndentString = "    ";
    #else
    Log_setFormatString("%indent%[%file%:%line%][%scope%] %message%");
    gIndentString = "..";
    #endif

    return NULL != gPrefixStack;
}

BOOL Log_isInitialized(void)
{
	return _isInitialized();
}

void Log_terminate(void)
{
    smug_assert(_isInitialized());
    LinkedList_delete(gPrefixStack);
    StdoutConsole_delete(gConsole);
}

void Log_addEntry(int level, char* file, int line, char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    Log_addEntryVa(level, file, line, fmt, vl);
    va_end(vl);
}

static char* getScopeString(int logScope)
{
    switch (logScope)
    {
        case LOG_DEBUG:
            return "DEBUG";
        case LOG_NOTIFICATION:
            return "NOTIFICATION";
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERROR:
            return "ERROR";
        default:
            return "BAD_SCOPE";
    }
}

static void Console_writeVoid(void* chars)
{
    gConsole->write((char*)chars);
}

void Log_addEntryVa(int level, char* file, int line, char* fmt, va_list args)
{
    smug_assert(_isInitialized());

    // Only print log if correct log level is set
    if (gCurrentLogLevel & level)
    {
        char message[MAX_MESSAGE_SIZE];
        // Print formatted string to the message buffer
        vsprintf(message, fmt, args);

        // Static log version: "%indent%[%file%:%line%][%scope%] %message%"
        LinkedList_doList(gPrefixStack, Console_writeVoid);
        gConsole->writeLine("[%s:%n][] %s", file, line, getScopeString(level), message);
    }
}

void Log_setLevel(int level)
{
    smug_assert(_isInitialized());
    gCurrentLogLevel = level;
}

int Log_getLevel(void)
{
    return gCurrentLogLevel;
}

void Log_setFormatString(char* format_string)
{
    smug_assert(_isInitialized());
    gFormatString = format_string;
}

void Log_pushPrefix(char* prefix)
{
    smug_assert(_isInitialized());
    LinkedList_addLast(gPrefixStack, prefix);
}

char* Log_popPrefix(void)
{
    smug_assert(_isInitialized());
    return (char*)LinkedList_popLast(gPrefixStack);
}

void Log_indent(void)
{
    Log_pushPrefix(gIndentString);
}

void Log_dedent(void)
{
    Log_popPrefix();
}

void Log_setIndentationString(char* indentString)
{
    gIndentString = indentString;
}
