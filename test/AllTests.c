#include <stdio.h>

#include <CuTest.h>

CuSuite* LinkedListTest_GetSuite();
CuSuite* vertexArrayTest_GetSuite();
CuSuite* colorArrayTest_GetSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, LinkedListTest_GetSuite());
	CuSuiteAddSuite(suite, vertexArrayTest_GetSuite());
	CuSuiteAddSuite(suite, colorArrayTest_GetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
    return 0;
}
