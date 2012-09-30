#include <CuTest.h>

#include <utils/log.h>
#include <utils/string_console.h>

static int STRING_CONSOLE_MAX_SIZE = 1024;

void Log_init_shoudlInitializeLog(CuTest* tc)
{
    Console* console = StringConsole_new(STRING_CONSOLE_MAX_SIZE);
    CuAssertTrue(tc, Log_init(console));
    CuAssertTrue(tc, Log_isInitialized());
    StringConsole_delete(console);
}

CuSuite* LogTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, Log_init_shoudlInitializeLog);

	return suite;
}
