#include <string.h>
#include <stdio.h>
#include <CuTest.h>

#include <utils/log.h>
#include <utils/string_console.h>
#include <utils/null_console.h>

static int STRING_CONSOLE_MAX_SIZE = 1024;

void Log_init_shouldInitializeLog(CuTest* tc)
{
    Console* console = NullConsole_new();
    CuAssertTrue(tc, Log_init(console));
    CuAssertTrue(tc, Log_isInitialized());
    Log_terminate();
    NullConsole_delete(console);
}

void Log_terminate_shouldTerminateLog(CuTest* tc)
{
    Console* console = NullConsole_new();
    Log_init(console);
    Log_terminate();
    CuAssertTrue(tc, !Log_isInitialized());
    NullConsole_delete(console);
}

void Log_setLevel_shouldSetSuppliedLogLevel(CuTest* tc)
{
    Console* console = NullConsole_new();
    Log_init(console);

    Log_setScopes(LOG_DEBUG);
    CuAssertTrue(tc, Log_getScopes() == LOG_DEBUG);
    Log_setScopes(LOG_WARNING);
    CuAssertTrue(tc, Log_getScopes() == LOG_WARNING);
    Log_setScopes(LOG_ERROR);
    CuAssertTrue(tc, Log_getScopes() == LOG_ERROR);
    Log_setScopes(LOG_ALL);
    CuAssertTrue(tc, Log_getScopes() == LOG_ALL);
    Log_setScopes(LOG_NONE);
    CuAssertTrue(tc, Log_getScopes() == LOG_NONE);

    Log_terminate();
    NullConsole_delete(console);
}

void Log_addEntry_shouldUseHardCodedFormatString(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_activateScopes(LOG_DEBUG);
    Log_addEntry(LOG_DEBUG, "filename", 6, "message");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("[DEBUG] message     [filename:6]\n", result) == 0);
    StringConsole_delete(console);
}

void Log_addEntry_shouldHandleEmptyMessage(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_activateScopes(LOG_DEBUG);
    Log_addEntry(LOG_DEBUG, "filename", 6, "");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("[DEBUG]      [filename:6]\n", result) == 0);
    StringConsole_delete(console);
}

void Log_addEntry_shouldNotPrintWhenScopeSetToNone(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_setScopes(LOG_NONE);
    Log_addEntry(LOG_DEBUG, "filename", 6, "message");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("", result) == 0);
    StringConsole_delete(console);
}

void Log_addEntry_shouldNotPrintSilencedScope(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_setScopes(LOG_DEBUG);
    Log_silenceScopes(LOG_DEBUG);
    Log_addEntry(LOG_DEBUG, "filename", 6, "message");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("", result) == 0);
    StringConsole_delete(console);
}

void Log_indent_shouldIncreaseIndentation(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_setIndentationString("test");
    Log_activateScopes(LOG_DEBUG);
    Log_indent();
    Log_addEntry(LOG_DEBUG, "filename", 6, "message");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("test[DEBUG] message     [filename:6]\n", result) == 0);
    StringConsole_delete(console);
}

void Log_indent_shouldIncreaseIndentationMoreThanOnce(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_setIndentationString("test");
    Log_activateScopes(LOG_DEBUG);
    Log_indent();
    Log_indent();
    Log_addEntry(LOG_DEBUG, "filename", 6, "message");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("testtest[DEBUG] message     [filename:6]\n", result) == 0);
    StringConsole_delete(console);
}

void Log_dedent_shouldDecreaseIndentation(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_setIndentationString("test");
    Log_activateScopes(LOG_DEBUG);
    Log_indent();
    Log_indent();
    Log_indent();
    Log_dedent();
    Log_dedent();
    Log_addEntry(LOG_DEBUG, "filename", 6, "message");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("test[DEBUG] message     [filename:6]\n", result) == 0);
    StringConsole_delete(console);
}

CuSuite* LogTest_GetSuite()
{
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, Log_init_shouldInitializeLog);
    SUITE_ADD_TEST(suite, Log_terminate_shouldTerminateLog);
    SUITE_ADD_TEST(suite, Log_setLevel_shouldSetSuppliedLogLevel);
    SUITE_ADD_TEST(suite, Log_addEntry_shouldUseHardCodedFormatString);
    SUITE_ADD_TEST(suite, Log_addEntry_shouldHandleEmptyMessage);
    SUITE_ADD_TEST(suite, Log_addEntry_shouldNotPrintWhenScopeSetToNone);
    SUITE_ADD_TEST(suite, Log_addEntry_shouldNotPrintSilencedScope);
    SUITE_ADD_TEST(suite, Log_indent_shouldIncreaseIndentation);
    SUITE_ADD_TEST(suite, Log_indent_shouldIncreaseIndentationMoreThanOnce);
    SUITE_ADD_TEST(suite, Log_dedent_shouldDecreaseIndentation);

    return suite;
}
