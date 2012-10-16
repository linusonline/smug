#include <stdio.h>
#include <string.h>
#include <CuTest.h>
#include <graphics/image.h>
#include <graphics/texture.h>

#include <GL/glfw.h>
#include <utils/log.h>
#include <utils/stdout_console.h>

static unsigned char reference44[4*4*4] =
    {0, 0, 0, 255,         255, 255, 255, 0,       255, 255, 255, 0,       255, 0, 0, 255,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,
     0, 255, 0, 255,       255, 255, 255, 0,       255, 255, 255, 0,       0, 0, 255, 255};

static unsigned char reference55[5*5*4] =
    {0, 0, 0, 255,         255, 255, 255, 0,       255, 255, 255, 0,       255, 0, 0, 255,      0,0,0,0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,    0,0,0,0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,    0,0,0,0,
     0, 255, 0, 255,       255, 255, 255, 0,       255, 255, 255, 0,       0, 0, 255, 255,      0,0,0,0,
     0,0,0,0,              0,0,0,0,                0,0,0,0,                0,0,0,0,             0,0,0,0};

static Console* console;

static void init()
{
    console = StdoutConsole_new();
    Log_init(console);
    Log_setScopes(LOG_NONE);
    glfwInit();
    glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
}

static void deinit()
{
    glfwTerminate();
    Log_terminate();
    StdoutConsole_delete(console);
}

void Texture_newFromImage_shouldReturnNonNull(CuTest* tc)
{
    init();

    Image* image = Image_newFromData(reference44, sizeof(reference44), 4, 4, TRUE);
    Texture* texture = Texture_newFromImage(image);
    CuAssertTrue(tc, texture != NULL);
    Image_delete(image);
    Texture_delete(texture);

    deinit();
}

void Texture_getWidth_getHeight_shouldReturnRightValues(CuTest* tc)
{
    init();

    Image* image = Image_newFromData(reference44, sizeof(reference44), 4, 4, TRUE);
    Texture* texture = Texture_newFromImage(image);
    CuAssertTrue(tc, Texture_getWidth(texture) == 4);
    CuAssertTrue(tc, Texture_getHeight(texture) == 4);
    Image_delete(image);
    Texture_delete(texture);

    deinit();
}

void Texture_getWidth_getHeight_shouldReturnRightValuesForNonPowerOfTwoDimensions(CuTest* tc)
{
    init();

    Image* image = Image_newFromData(reference55, sizeof(reference55), 5, 5, TRUE);
    Texture* texture = Texture_newFromImage(image);
    CuAssertTrue(tc, Texture_getWidth(texture) == 5);
    CuAssertTrue(tc, Texture_getHeight(texture) == 5);
    Image_delete(image);
    Texture_delete(texture);

    deinit();
}

CuSuite* TextureTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, Texture_newFromImage_shouldReturnNonNull);
	SUITE_ADD_TEST(suite, Texture_getWidth_getHeight_shouldReturnRightValues);
	SUITE_ADD_TEST(suite, Texture_getWidth_getHeight_shouldReturnRightValuesForNonPowerOfTwoDimensions);

	return suite;
}
