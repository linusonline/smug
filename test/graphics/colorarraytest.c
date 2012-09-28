#include <graphics/colorarray.h>
#include <stdlib.h>
#include <GL/glfw.h>
#include <CuTest.h>

void ColorArray_init_works(CuTest* tc)
{
    glfwInit();
    ColorArray_init();
    ColorArray_release();
    glfwTerminate();
}

void ColorArray_addUnicolorRect_shouldAddRightNumberOfElements(CuTest* tc)
{
    glfwInit();
    ColorArray_init();
    ColorArray_addUnicolorRect(0.1, 0.2, 0.3);
    GLfloat* ca = getColorArray();
    CuAssertTrue(tc, ColorArray_getNumberOfAddedColors() == 4);
    ColorArray_release();
    glfwTerminate();
}

CuSuite* colorArrayTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, ColorArray_init_works);
	SUITE_ADD_TEST(suite, ColorArray_addUnicolorRect_shouldAddRightNumberOfElements);

	return suite;
}
