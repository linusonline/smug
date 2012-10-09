#include <stdio.h>
#include <CuTest.h>
#include <graphics/spriteanimation.h>
#include <utils/log.h>

void SpriteAnimation_newEmpty_shouldNotReturnNull(CuTest* tc)
{
    SpriteAnimation* sa = SpriteAnimation_newEmpty();
    CuAssertTrue(tc, sa != NULL);
    SpriteAnimation_delete(sa);
}

void SpriteAnimation_getCurrentSprite_shouldReturnNullOnEmptyAnimation(CuTest* tc)
{
    SpriteAnimation* sa = SpriteAnimation_newEmpty();
    CuAssertTrue(tc, SpriteAnimation_getCurrentSprite(sa) == NULL);
    SpriteAnimation_delete(sa);
}

void SpriteAnimation_getCurrentSprite_shouldReturnNullWhenAnimationNotStarted(CuTest* tc)
{
    Sprite* a = Sprite_newEmpty();
    SpriteAnimation* sa = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(sa, a, 1);
    CuAssertTrue(tc, SpriteAnimation_getCurrentSprite(sa) == NULL);
    SpriteAnimation_delete(sa);
    Sprite_delete(a);
}

void SpriteAnimation_getCurrentSprite_shouldReturnOnlySpriteAfterStart(CuTest* tc)
{
    initTest();

    Sprite* a = Sprite_newEmpty();
    SpriteAnimation* sa = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(sa, a, 1);
    SpriteAnimation_start(sa);
    CuAssertTrue(tc, SpriteAnimation_getCurrentSprite(sa) == a);
    SpriteAnimation_delete(sa);
    Sprite_delete(a);

    deinitTest();
}

void SpriteAnimation_getCurrentSprite_shouldReturnNullOnEmptyStartedAnimation(CuTest* tc)
{
    initTest();

    SpriteAnimation* sa = SpriteAnimation_newEmpty();
    SpriteAnimation_start(sa);
    CuAssertTrue(tc, SpriteAnimation_getCurrentSprite(sa) == NULL);
    SpriteAnimation_delete(sa);

    deinitTest();
}

void SpriteAnimation_getCurrentSprite_shouldReturnFirstSpriteAfterStart(CuTest* tc)
{
    initTest();

    Sprite* a = Sprite_newEmpty();
    Sprite* b = Sprite_newEmpty();
    Sprite* c = Sprite_newEmpty();
    SpriteAnimation* sa = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(sa, a, 1);
    SpriteAnimation_addFrame(sa, b, 1);
    SpriteAnimation_addFrame(sa, c, 1);
    SpriteAnimation_start(sa);
    CuAssertTrue(tc, SpriteAnimation_getCurrentSprite(sa) == a);
    SpriteAnimation_delete(sa);
    Sprite_delete(a);

    deinitTest();
}

void SpriteAnimation_getCurrentSprite_shouldReturnRightSprite(CuTest* tc)
{
    initTest();

    Sprite* a = Sprite_newEmpty();
    Sprite* b = Sprite_newEmpty();
    Sprite* c = Sprite_newEmpty();
    SpriteAnimation* sa = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(sa, a, 1);
    SpriteAnimation_addFrame(sa, b, 1);
    SpriteAnimation_addFrame(sa, c, 1);
    SpriteAnimation_startAt(sa, 1.5);
    CuAssertTrue(tc, SpriteAnimation_getCurrentSprite(sa) == b);
    SpriteAnimation_delete(sa);
    Sprite_delete(a);

    deinitTest();
}

void SpriteAnimation_getCurrentSprite_shouldReturnRightSpriteAfterLooping(CuTest* tc)
{
    initTest();

    Sprite* a = Sprite_newEmpty();
    Sprite* b = Sprite_newEmpty();
    Sprite* c = Sprite_newEmpty();
    SpriteAnimation* sa = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(sa, a, 1);
    SpriteAnimation_addFrame(sa, b, 1);
    SpriteAnimation_addFrame(sa, c, 1);
    SpriteAnimation_startAt(sa, 4.5);
    CuAssertTrue(tc, SpriteAnimation_getCurrentSprite(sa) == b);
    SpriteAnimation_delete(sa);
    Sprite_delete(a);

    deinitTest();
}

CuSuite* SpriteAnimationTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, SpriteAnimation_newEmpty_shouldNotReturnNull);
	SUITE_ADD_TEST(suite, SpriteAnimation_getCurrentSprite_shouldReturnNullOnEmptyAnimation);
	SUITE_ADD_TEST(suite, SpriteAnimation_getCurrentSprite_shouldReturnNullWhenAnimationNotStarted);
	SUITE_ADD_TEST(suite, SpriteAnimation_getCurrentSprite_shouldReturnOnlySpriteAfterStart);
	SUITE_ADD_TEST(suite, SpriteAnimation_getCurrentSprite_shouldReturnNullOnEmptyStartedAnimation);
	SUITE_ADD_TEST(suite, SpriteAnimation_getCurrentSprite_shouldReturnFirstSpriteAfterStart);
	SUITE_ADD_TEST(suite, SpriteAnimation_getCurrentSprite_shouldReturnRightSprite);
	SUITE_ADD_TEST(suite, SpriteAnimation_getCurrentSprite_shouldReturnRightSpriteAfterLooping);

	return suite;
}
