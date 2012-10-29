#include <stdio.h>
#include <string.h>
#include <CuTest.h>
#include <graphics/image.h>
#include <graphics/image_internal.h>

static unsigned char reference[4*4*4] =
    {0, 0, 0, 255,         255, 255, 255, 0,       255, 255, 255, 0,       255, 0, 0, 255,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,
     0, 255, 0, 255,       255, 255, 255, 0,       255, 255, 255, 0,       0, 0, 255, 255};

static unsigned char referenceFilledOutH[4*5*4] =
    {0, 0, 0, 255,         255, 255, 255, 0,       255, 255, 255, 0,       255, 0, 0, 255,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,
     0, 255, 0, 255,       255, 255, 255, 0,       255, 255, 255, 0,       0, 0, 255, 255,
     0,0,0,0,              0,0,0,0,                0,0,0,0,                0,0,0,0};

static unsigned char referenceFilledOutW[5*4*4] =
    {0, 0, 0, 255,         255, 255, 255, 0,       255, 255, 255, 0,       255, 0, 0, 255,      0,0,0,0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,    0,0,0,0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,    0,0,0,0,
     0, 255, 0, 255,       255, 255, 255, 0,       255, 255, 255, 0,       0, 0, 255, 255,      0,0,0,0};

static unsigned char referenceFilledOutHW[5*5*4] =
    {0, 0, 0, 255,         255, 255, 255, 0,       255, 255, 255, 0,       255, 0, 0, 255,      0,0,0,0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,    0,0,0,0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,    0,0,0,0,
     0, 255, 0, 255,       255, 255, 255, 0,       255, 255, 255, 0,       0, 0, 255, 255,      0,0,0,0,
     0,0,0,0,              0,0,0,0,                0,0,0,0,                0,0,0,0,             0,0,0,0};

static unsigned char testImage[6*8*4] =
    {255, 0, 0,255,  255, 0, 0,255,  255, 0, 0,255,  255, 0, 0,255,  255, 0, 0,255,  255, 0, 0,255,
     255, 0, 0,255,  0, 0, 255,255,  0, 0, 255,255,  0, 0, 255,255,  0, 0, 255,255,  0, 0, 255,255,
     255, 0, 0,255,  0, 0, 255,255,  0, 255, 0,255,  0, 255, 0,255,  0, 255, 0,255,  0, 255, 0,255,
     255, 0, 0,255,  0, 0, 255,255,  0, 255, 0,255,  255, 0, 255,255,255, 0, 255,255,0, 255, 0,255,
     255, 0, 0,255,  0, 0, 255,255,  0, 255, 0,255,  255, 0, 255,255,255, 0, 255,255,0, 255, 0,255,
     255, 0, 0,255,  0, 0, 255,255,  0, 255, 0,255,  0, 255, 0,255,  0, 255, 0,255,  0, 255, 0,255,
     255, 0, 0,255,  0, 0, 255,255,  0, 0, 255,255,  0, 0, 255,255,  0, 0, 255,255,  0, 0, 255,255,
     255, 0, 0,255,  255, 0, 0,255,  255, 0, 0,255,  255, 0, 0,255,  255, 0, 0,255,  255, 0, 0,255};

void Image_newFromData_works(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, TRUE);
    CuAssert(tc, "Could not create image on the fly.", image != NULL);
    CuAssertTrue(tc, image->width == 4);
    CuAssertTrue(tc, image->height == 4);
    CuAssertTrue(tc, image->channels == 4);
    CuAssertTrue(tc, image->bpp == 4);
    CuAssertTrue(tc, image->size == 4*4*4);
    CuAssertTrue(tc, image->file == NULL);
    CuAssertTrue(tc, image->data != NULL);
    CuAssert(tc, "Image contents incorrect.", memcmp(reference, image->data, 4*4*4) == 0);
    Image_delete(image);
}

void Image_newFromData_shouldNotDeleteAlphaValues(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, FALSE);
    CuAssertTrue(tc, image->channels = 3);
    CuAssert(tc, "Image contents incorrect.", memcmp(reference, image->data, 4*4*4) == 0);
    Image_delete(image);
}

void Image_saveToFile_works(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, TRUE);
    BOOL result = Image_saveToFile(image, "test-image.png");
    CuAssert(tc, "Could not save image.", result == TRUE);
    Image_delete(image);
}

void Image_loadFromFile_works(CuTest* tc)
{
    Image* image = Image_new();
    BOOL result = Image_loadFromFile(image, "5_res/test.png");
    CuAssert(tc, "Could not load image.", result == TRUE);
    Image_delete(image);
}

void Image_loadFromFile_shouldYieldRightDimensions(CuTest* tc)
{
    Image* image = Image_new();
    Image_loadFromFile(image, "5_res/test.png");
    CuAssert(tc, "Image dimensions incorrect.", image->width == 6 && image->height == 8);
    CuAssertTrue(tc, image->channels == 3);
    CuAssertTrue(tc, image->bpp == 4);
    CuAssert(tc, "Image size incorrect.", 6*8*4 == image->size);

    Image_delete(image);
}

void Image_loadFromFile_shouldYieldCorrectImage(CuTest* tc)
{
    Image* image = Image_new();
    Image_loadFromFile(image, "5_res/test.png");

    CuAssert(tc, "Image contents incorrect.", memcmp(testImage, image->data, 6*8*4) == 0);

    Image_delete(image);
}

void Image_fillOut_shouldYieldCorrectImageInTrivialCase(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, TRUE);
    Image_fillOut(image, 4, 4);

    CuAssertTrue(tc, image->height == 4);
    CuAssertTrue(tc, image->width == 4);
    CuAssertTrue(tc, image->channels == 4);
    CuAssertTrue(tc, image->size == 64);
    CuAssert(tc, "Image contents incorrect.", memcmp(reference, image->data, 64) == 0);

    Image_delete(image);
}

void Image_fillOut_shouldYieldCorrectImageFillingWidth(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, TRUE);
    Image_fillOut(image, 5, 4);

    CuAssertTrue(tc, image->width == 5);
    CuAssertTrue(tc, image->height == 4);
    CuAssertTrue(tc, image->channels == 4);
    CuAssertTrue(tc, image->size == 80);
    CuAssert(tc, "Image contents incorrect.", memcmp(referenceFilledOutW, image->data, 80) == 0);

    Image_delete(image);
}

void Image_fillOut_shouldYieldCorrectImageFillingHeight(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, TRUE);
    Image_fillOut(image, 4, 5);

    CuAssertTrue(tc, image->width == 4);
    CuAssertTrue(tc, image->height == 5);
    CuAssertTrue(tc, image->channels == 4);
    CuAssertTrue(tc, image->size == 80);
    CuAssert(tc, "Image contents incorrect.", memcmp(referenceFilledOutH, image->data, 80) == 0);

    Image_delete(image);
}

void Image_fillOut_shouldYieldCorrectImageFillingWidthAndHeight(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, TRUE);
    Image_fillOut(image, 5, 5);

    CuAssertTrue(tc, image->width == 5);
    CuAssertTrue(tc, image->height == 5);
    CuAssertTrue(tc, image->channels == 4);
    CuAssertTrue(tc, image->size == 100);
    CuAssert(tc, "Image contents incorrect.", memcmp(referenceFilledOutHW, image->data, 100) == 0);

    Image_delete(image);
}

CuSuite* ImageTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, Image_newFromData_works);
	SUITE_ADD_TEST(suite, Image_newFromData_shouldNotDeleteAlphaValues);
	SUITE_ADD_TEST(suite, Image_saveToFile_works);
	SUITE_ADD_TEST(suite, Image_loadFromFile_works);
	SUITE_ADD_TEST(suite, Image_loadFromFile_shouldYieldRightDimensions);
	SUITE_ADD_TEST(suite, Image_loadFromFile_shouldYieldCorrectImage);
	SUITE_ADD_TEST(suite, Image_fillOut_shouldYieldCorrectImageInTrivialCase);
	SUITE_ADD_TEST(suite, Image_fillOut_shouldYieldCorrectImageFillingWidth);
	SUITE_ADD_TEST(suite, Image_fillOut_shouldYieldCorrectImageFillingHeight);
	SUITE_ADD_TEST(suite, Image_fillOut_shouldYieldCorrectImageFillingWidthAndHeight);

	return suite;
}
