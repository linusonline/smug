#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <common.h>
#include <utils/console.h>
#include <utils/stdout_console.h>
#include <utils/linkedlist.h>
#include <utils/log.h>

static unsigned int gCurrentLogLevel = LOG_NONE;
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

    Log_setScopes(LOG_ERROR | LOG_WARNING | LOG_DEFAULT);

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

void Log_addEntry(int scope, char* file, int line, char* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    Log_addEntryVa(scope, file, line, fmt, vl);
    va_end(vl);
}

static char* getScopeString(int logScope)
{
    switch (logScope)
    {
        case LOG_DEBUG:
            return "DEBUG"; break;
        case LOG_WARNING:
            return "WARNING"; break;
        case LOG_ERROR:
            return "ERROR"; break;
        case LOG_FPS:
            return "FPS"; break;
        case LOG_WINDOW:
            return "WINDOW"; break;
        case LOG_IMAGE:
            return "IMAGE"; break;
        case LOG_ANIMATION:
            return "ANIMATION"; break;
        case LOG_SPRITESHEET:
            return "SPRITESHEET"; break;
        case LOG_TEXTURE:
            return "TEXTURE"; break;
        case LOG_INPUT:
            return "INPUT"; break;
        case LOG_MAP:
            return "MAP"; break;
        case LOG_ENGINE:
            return "ENGINE"; break;
        case LOG_DEFAULT:
            return "DEFAULT"; break;
        case LOG_USER1:
            return "LOG_USER1"; break;
        default:
            return "BAD_SCOPE";
    }
}

static void Console_writeVoid(void* chars)
{
    Console_write(gConsole, (char*)chars);
}

static BOOL _isScopeActive(int scope)
{
    return (gCurrentLogLevel & scope) != 0;
}

void Log_addEntryVa(int scope, char* file, int line, char* fmt, va_list args)
{
    if (!_isInitialized())
    {
        return;
    }

    // Only print log if correct log scope is set
    if (_isScopeActive(scope))
    {
        char message[MAX_MESSAGE_SIZE];
        // Print formatted string to the message buffer
        int written = vsprintf(message, fmt, args);
        if (written >= 0)
        {
            message[written] = 0;

            // Static log version: "[%scope%]%indent%%message% [%file%:%line%]"
            LinkedList_doList(gPrefixStack, Console_writeVoid);
            Console_writeLine(gConsole, "[%s] %s     [%s:%i]", getScopeString(scope), message, file, line);
        }
    }
}

void Log_silenceScopes(unsigned int scopes)
{
    gCurrentLogLevel &= (~scopes);
}

void Log_activateScopes(unsigned int scopes)
{
    gCurrentLogLevel |= scopes;
}

void Log_setScopes(unsigned int scopes)
{
    smug_assert(_isInitialized());
    gCurrentLogLevel = scopes;
}

unsigned int Log_getScopes()
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
    if (_isInitialized())
    {
        Log_pushPrefix(gIndentString);
    }
}

void Log_dedent()
{
    if (_isInitialized())
    {
        Log_popPrefix();
    }
}

void Log_setIndentationString(char* indentString)
{
    gIndentString = indentString;
}
