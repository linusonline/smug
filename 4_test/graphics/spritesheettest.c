#include <stdio.h>
#include <string.h>
#include <CuTest.h>
#include <graphics/spritesheet.h>

#include <GL/glfw.h>
#include <utils/log.h>
#include <utils/stdout_console.h>

static void init()
{
    initTest();
    glfwInit();
    glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
}

static void deinit()
{
    glfwTerminate();
    deinitTest();
}

void SpriteSheet_new_shouldReturnValidSheet(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../5_res/unittests/landscape.png", "../5_res/unittests/landscape.txt");
    CuAssertTrue(tc, ss != NULL);
    CuAssertTrue(tc, SpriteSheet_isValid(ss));
    SpriteSheet_delete(ss);

    deinit();
}

void SpriteSheet_new_shouldReturnValidSheetOnNullDataFile(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../5_res/unittests/landscape.png", NULL);
    CuAssertTrue(tc, ss != NULL);
    CuAssertTrue(tc, SpriteSheet_isValid(ss));
    SpriteSheet_delete(ss);

    deinit();
}

void SpriteSheet_new_shouldReturnInvalidSheetOnImageFileReadError(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("idontexist.png", "../5_res/unittests/landscape.txt");
    CuAssertTrue(tc, ss != NULL);
    CuAssertTrue(tc, !SpriteSheet_isValid(ss));

    deinit();
}

void SpriteSheet_new_shouldReturnInvalidSheetOnDataFileReadError(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../5_res/unittests/landscape.png", "idontexist.txt");
    CuAssertTrue(tc, ss != NULL);
    CuAssertTrue(tc, !SpriteSheet_isValid(ss));

    deinit();
}

void SpriteSheet_newUnloaded_shouldReturnInvalidSheet(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_newUnloaded("../5_res/unittests/landscape.png", "../5_res/unittests/landscape.txt");
    CuAssertTrue(tc, ss != NULL);
    CuAssertTrue(tc, !SpriteSheet_isValid(ss));

    deinit();
}

void SpriteSheet_reload_shouldValidateSheet(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_newUnloaded("../5_res/unittests/landscape.png", "../5_res/unittests/landscape.txt");
    SpriteSheet_reload(ss);
    CuAssertTrue(tc, SpriteSheet_isValid(ss));

    deinit();
}

void SpriteSheet_new_shouldYieldRightNumberOfSprites(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../5_res/unittests/landscape.png", "../5_res/unittests/landscape.txt");
    CuAssertTrue(tc, SpriteSheet_getNrOfSprites(ss) == 24);
    SpriteSheet_delete(ss);

    deinit();
}

void SpriteSheet_getSprite_shouldReturnSameObjectForSameIndex(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../5_res/unittests/landscape.png", "../5_res/unittests/landscape.txt");
    Sprite* s = SpriteSheet_getSprite(ss, 0);
    CuAssertTrue(tc, SpriteSheet_getSprite(ss, 0) == s);
    SpriteSheet_delete(ss);

    deinit();
}

void SpriteSheet_getSprite_shouldReturnSpriteForValidIndex(CuTest* tc)
{
    init();

    SpriteSheet* ss = SpriteSheet_new("../5_res/unittests/landscape.png", "../5_res/unittests/landscape.txt");

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

	SUITE_ADD_TEST(suite, SpriteSheet_new_shouldReturnValidSheet);
	SUITE_ADD_TEST(suite, SpriteSheet_new_shouldReturnValidSheetOnNullDataFile);
	SUITE_ADD_TEST(suite, SpriteSheet_new_shouldReturnInvalidSheetOnImageFileReadError);
	SUITE_ADD_TEST(suite, SpriteSheet_new_shouldReturnInvalidSheetOnDataFileReadError);
	SUITE_ADD_TEST(suite, SpriteSheet_newUnloaded_shouldReturnInvalidSheet);
	SUITE_ADD_TEST(suite, SpriteSheet_reload_shouldValidateSheet);
	SUITE_ADD_TEST(suite, SpriteSheet_new_shouldYieldRightNumberOfSprites);
	SUITE_ADD_TEST(suite, SpriteSheet_getSprite_shouldReturnSameObjectForSameIndex);
	SUITE_ADD_TEST(suite, SpriteSheet_getSprite_shouldReturnSpriteForValidIndex);

	return suite;
}
