#include <string.h>
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

void StringConsole_new_shouldReturnEmptyConsole(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    const char* string = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("", string) == 0);
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

void StringConsole_write_shouldWriteEmptyStringToBuffer(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    console->write("");
    const char* string = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("", string) == 0);
    StringConsole_delete(console);
}

void StringConsole_write_shouldWriteStringToBuffer(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    console->write("string");
    const char* string = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("string", string) == 0);
    StringConsole_delete(console);
}

void StringConsole_write_shouldWriteSeveralStringsToBuffer(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    console->write("string");
    console->write("STRING");
    const char* string = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("stringSTRING", string) == 0);
    StringConsole_delete(console);
}

void StringConsole_writeLine_shouldWriteEmptyStringToBuffer(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    console->writeLine("");
    const char* string = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("\n", string) == 0);
    StringConsole_delete(console);
}

void StringConsole_writeLine_shouldWriteStringToBuffer(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    console->writeLine("string");
    const char* string = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("string\n", string) == 0);
    StringConsole_delete(console);
}

void StringConsole_writeLine_shouldWriteSeveralStringsToBuffer(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    console->writeLine("string");
    console->writeLine("STRING");
    const char* string = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("string\nSTRING\n", string) == 0);
    StringConsole_delete(console);
}

void StringConsole_clearBuffer_shouldClearBuffer(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    console->write("string");
    StringConsole_clearBuffer();
    const char* string = StringConsole_getBuffer();
    CuAssertTrue(tc, strcmp("", string) == 0);
    StringConsole_delete(console);
}

CuSuite* StringConsoleTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, StringConsole_new_shouldReturnConsole);
	SUITE_ADD_TEST(suite, StringConsole_new_shouldReturnEmptyConsole);
	SUITE_ADD_TEST(suite, StringConsole_write_shouldBeCallable);
	SUITE_ADD_TEST(suite, StringConsole_writeLine_shouldBeCallable);
	SUITE_ADD_TEST(suite, StringConsole_write_shouldWriteEmptyStringToBuffer);
	SUITE_ADD_TEST(suite, StringConsole_write_shouldWriteStringToBuffer);
	SUITE_ADD_TEST(suite, StringConsole_write_shouldWriteSeveralStringsToBuffer);
	SUITE_ADD_TEST(suite, StringConsole_writeLine_shouldWriteEmptyStringToBuffer);
	SUITE_ADD_TEST(suite, StringConsole_writeLine_shouldWriteStringToBuffer);
	SUITE_ADD_TEST(suite, StringConsole_writeLine_shouldWriteSeveralStringsToBuffer);
	SUITE_ADD_TEST(suite, StringConsole_clearBuffer_shouldClearBuffer);

	return suite;
}
