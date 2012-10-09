#include <stdio.h>

#include <CuTest.h>

CuSuite* LinkedListTest_GetSuite();
CuSuite* BinaryTreeTest_GetSuite();
CuSuite* MapTest_GetSuite();
CuSuite* LogTest_GetSuite();
CuSuite* StringConsoleTest_GetSuite();

CuSuite* ImageTest_GetSuite();
CuSuite* RenderBatchTest_GetSuite();
CuSuite* TextureTest_GetSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, LinkedListTest_GetSuite());
	CuSuiteAddSuite(suite, BinaryTreeTest_GetSuite());
	CuSuiteAddSuite(suite, MapTest_GetSuite());
	CuSuiteAddSuite(suite, LogTest_GetSuite());
	CuSuiteAddSuite(suite, StringConsoleTest_GetSuite());

	CuSuiteAddSuite(suite, ImageTest_GetSuite());
	CuSuiteAddSuite(suite, RenderBatchTest_GetSuite());
	CuSuiteAddSuite(suite, TextureTest_GetSuite());

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
