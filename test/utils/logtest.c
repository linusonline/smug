#include <string.h>
#include <stdio.h>
#include <CuTest.h>

#include <utils/log.h>
#include <utils/string_console.h>
#include <utils/null_console.h>

static int STRING_CONSOLE_MAX_SIZE = 1024;

void Log_init_shoudlInitializeLog(CuTest* tc)
{
    Console* console = NullConsole_new();
    CuAssertTrue(tc, Log_init(console));
    CuAssertTrue(tc, Log_isInitialized());
    Log_terminate();
    NullConsole_delete(console);
}

void Log_terminate_shoudlTerminateLog(CuTest* tc)
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

    Log_setLevel(LOG_DEBUG);
    CuAssertTrue(tc, Log_getLevel() == LOG_DEBUG);
    Log_setLevel(LOG_NOTIFICATION);
    CuAssertTrue(tc, Log_getLevel() == LOG_NOTIFICATION);
    Log_setLevel(LOG_WARNING);
    CuAssertTrue(tc, Log_getLevel() == LOG_WARNING);
    Log_setLevel(LOG_ERROR);
    CuAssertTrue(tc, Log_getLevel() == LOG_ERROR);
    Log_setLevel(LOG_ALL);
    CuAssertTrue(tc, Log_getLevel() == LOG_ALL);
    Log_setLevel(LOG_NONE);
    CuAssertTrue(tc, Log_getLevel() == LOG_NONE);

    Log_terminate();
    NullConsole_delete(console);
}

void Log_addEntry_shouldUseHardCodedFormatString(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_setIndentationString("test");
    Log_setLevel(LOG_DEBUG);
    Log_addEntry(LOG_DEBUG, "filename", 6, "message");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("[filename:6][DEBUG] message\n", result) == 0);
    StringConsole_delete(console);
}

void Log_addEntry_shouldHandleEmptyMessage(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_setIndentationString("test");
    Log_setLevel(LOG_DEBUG);
    Log_addEntry(LOG_DEBUG, "filename", 6, "");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("[filename:6][DEBUG] \n", result) == 0);
    StringConsole_delete(console);
}

void Log_addEntry_shouldNotPrintWrongScope(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);

    Log_init(console);
    Log_setIndentationString("test");
    Log_setLevel(LOG_ERROR);
    Log_addEntry(LOG_DEBUG, "filename", 6, "message");
    Log_terminate();

    char* result = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("", result) == 0);
    StringConsole_delete(console);
}

CuSuite* LogTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, Log_init_shoudlInitializeLog);
	SUITE_ADD_TEST(suite, Log_terminate_shoudlTerminateLog);
	SUITE_ADD_TEST(suite, Log_setLevel_shouldSetSuppliedLogLevel);
	SUITE_ADD_TEST(suite, Log_addEntry_shouldUseHardCodedFormatString);
	SUITE_ADD_TEST(suite, Log_addEntry_shouldHandleEmptyMessage);
	SUITE_ADD_TEST(suite, Log_addEntry_shouldNotPrintWrongScope);

	return suite;
}
