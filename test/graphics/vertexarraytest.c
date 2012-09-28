#include <graphics/vertexarray.h>
#include <stdlib.h>
#include <GL/glfw.h>
#include <CuTest.h>

void glfwInit_works(CuTest* tc)
{
    CuAssert(tc, "Could not initialize glfw.", glfwInit() == GL_TRUE);
    glfwTerminate();
}

void VertexArray_init_works(CuTest* tc)
{
    glfwInit();
    VertexArray_init();
    VertexArray_release();
    glfwTerminate();
}

void VertexArray_addRect_shouldAddRightNumberOfElements(CuTest* tc)
{
    glfwInit();
    VertexArray_init();
    VertexArray_addRect(1, 2, 3, 4);
    GLfloat* va = getVertexArray();
    CuAssertTrue(tc, VertexArray_getNumberOfAddedVertices() == 4);
    VertexArray_release();
    glfwTerminate();
}

CuSuite* vertexArrayTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, glfwInit_works);
	SUITE_ADD_TEST(suite, VertexArray_init_works);
	SUITE_ADD_TEST(suite, VertexArray_addRect_shouldAddRightNumberOfElements);

	return suite;
}
