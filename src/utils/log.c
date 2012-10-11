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

static BOOL _isInitialized()
{
    return gPrefixStack != NULL && gConsole != NULL;
}

BOOL Log_init(Console* console)
{
    smug_assert(console != NULL);

    gConsole = console;

    // Allocate memory for prefix stack
    gPrefixStack = LinkedList_new();
    smug_assert(_isInitialized());

    // Set default format string
    #ifdef NDEBUG
    Log_setFormatString("%indent%%message%");
    gIndentString = "    ";
    #else
    // Log_setFormatString("%indent%[%file%:%line%][%scope%] %message%");
    Log_setFormatString("[%scope%]%indent%%message% [%file%:%line%]");
    gIndentString = "..";
    #endif

    return NULL != gPrefixStack;
}

BOOL Log_isInitialized()
{
	return _isInitialized();
}

void Log_terminate()
{
    smug_assert(_isInitialized());
    LinkedList_delete(gPrefixStack);
    gPrefixStack = NULL;
    gConsole = NULL;
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

static BOOL _isScopeActive(int scope)
{
    return gCurrentLogLevel & scope;
}

void Log_addEntryVa(int level, char* file, int line, char* fmt, va_list args)
{
    if (!_isInitialized())
    {
        return;
    }

    // Only print log if correct log level is set
    if (_isScopeActive(level))
    {
        char message[MAX_MESSAGE_SIZE];
        // Print formatted string to the message buffer
        int written = vsprintf(message, fmt, args);
        if (written >= 0)
        {
            message[written] = 0;

            // Static log version: "[%scope%]%indent%%message% [%file%:%line%]"
            LinkedList_doList(gPrefixStack, Console_writeVoid);
            gConsole->writeLine("[%s] %s     [%s:%i]", getScopeString(level), message, file, line);
        }
    }
}

void Log_setLevel(int level)
{
    smug_assert(_isInitialized());
    gCurrentLogLevel = 0;
    switch (level)
    {
        case LOG_ALL:
        case LOG_DEBUG:
            gCurrentLogLevel |= LOG_DEBUG;
        case LOG_NOTIFICATION:
            gCurrentLogLevel |= LOG_NOTIFICATION;
        case LOG_WARNING:
            gCurrentLogLevel |= LOG_WARNING;
        case LOG_ERROR:
            gCurrentLogLevel |= LOG_ERROR;
        case LOG_NONE:
        default:
            SMUG_NOOP();
    }
    gCurrentLogLevel = level;
}

int Log_getLevel()
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

char* Log_popPrefix()
{
    smug_assert(_isInitialized());
    return (char*)LinkedList_popLast(gPrefixStack);
}

void Log_indent()
{
    Log_pushPrefix(gIndentString);
}

void Log_dedent()
{
    Log_popPrefix();
}

void Log_setIndentationString(char* indentString)
{
    gIndentString = indentString;
}
