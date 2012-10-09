#include <stdio.h>
#include <string.h>
#include <CuTest.h>
#include <graphics/spritesheet.h>

#include <GL/glfw.h>
#include <utils/log.h>
#include <utils/stdout_console.h>

static Console* console;

static void init()
{
    console = StdoutConsole_new();
    Log_init(console);
    Log_setLevel(LOG_NONE);
    glfwInit();
    glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
}

static void deinit()
{
    glfwTerminate();
    Log_terminate();
    StdoutConsole_delete(console);
}

void SpriteSheet_new_shouldReturnNonNull(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../res/unittests/landscape.png", "../res/unittests/landscape.txt");
    CuAssertTrue(tc, ss != NULL);
    SpriteSheet_delete(ss);

    deinit();
}

void SpriteSheet_new_shouldYieldRightNumberOfSprites(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../res/unittests/landscape.png", "../res/unittests/landscape.txt");
    CuAssertTrue(tc, SpriteSheet_getNrOfSprites(ss) == 24);
    SpriteSheet_delete(ss);

    deinit();
}

void SpriteSheet_getSprite_shouldReturnSameObjectForSameIndex(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../res/unittests/landscape.png", "../res/unittests/landscape.txt");
    Sprite* s = SpriteSheet_getSprite(ss, 0);
    CuAssertTrue(tc, SpriteSheet_getSprite(ss, 0) == s);
    SpriteSheet_delete(ss);

    deinit();
}

void SpriteSheet_getSprite_shouldReturnSpriteForValidIndex(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../res/unittests/landscape.png", "../res/unittests/landscape.txt");

    for (int i = 0; i < 24; i++)
    {
        Sprite* s = SpriteSheet_getSprite(ss, i);
        CuAssertTrue(tc, s != NULL);
    }
    SpriteSheet_delete(ss);

    deinit();
}

CuSuite* SpriteSheetTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, SpriteSheet_new_shouldReturnNonNull);
	SUITE_ADD_TEST(suite, SpriteSheet_new_shouldYieldRightNumberOfSprites);
	SUITE_ADD_TEST(suite, SpriteSheet_getSprite_shouldReturnSameObjectForSameIndex);
	SUITE_ADD_TEST(suite, SpriteSheet_getSprite_shouldReturnSpriteForValidIndex);

	return suite;
}
