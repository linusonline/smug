#include <CuTest.h>

#include <common.h>
#include <utils/string_console.h>

static int STRING_CONSOLE_MAX_SIZE = 1024;

void StringConsole_new_shouldReturnConsole(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    CuAssertTrue(tc, console != NULL);
    StringConsole_delete(console);
}

void StringConsole_write_shouldBeCallable(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    console->write("");
    StringConsole_delete(console);
}

void StringConsole_writeLine_shouldBeCallable(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    console->writeLine("");
    StringConsole_delete(console);
}

CuSuite* StringConsoleTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, StringConsole_new_shouldReturnConsole);
	SUITE_ADD_TEST(suite, StringConsole_write_shouldBeCallable);
	SUITE_ADD_TEST(suite, StringConsole_writeLine_shouldBeCallable);

	return suite;
}
