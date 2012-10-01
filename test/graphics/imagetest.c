#include <stdio.h>
#include <string.h>
#include <CuTest.h>
#include <graphics/image.h>

static unsigned char reference[4*4*4] =
    {0, 0, 0, 255,         255, 255, 255, 0,       255, 255, 255, 0,       255, 0, 0, 255,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,
     255, 255, 255, 0,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 0,
     0, 255, 0, 255,       255, 255, 255, 0,       255, 255, 255, 0,       0, 0, 255, 255};

void Image_newFromData_works(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, 4);
    CuAssert(tc, "Could not create image on the fly.", image != NULL);
    Image_delete(image);
}

void Image_saveToFile_works(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, 4);
    BOOL result = Image_saveToFile(image, "test-image.png");
    CuAssert(tc, "Could not save image.", result == TRUE);
    Image_delete(image);
}

void Image_loadFromFile_works(CuTest* tc)
{
    Image* image1 = Image_newFromData(reference, sizeof(reference), 4, 4, 4);
    Image_saveToFile(image1, "test-image.png");
    Image_delete(image1);

    Image* image2 = Image_new();
    BOOL result = Image_loadFromFile(image2, "test-image.png");
    CuAssert(tc, "Could not load image.", result == TRUE);
    Image_delete(image2);
}

void Image_loadFromFile_shouldYieldRightDimensions(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, 4);
    Image_saveToFile(image, "test-image.png");
    Image_delete(image);

    image = Image_new();
    Image_loadFromFile(image, "test-image.png");
    CuAssert(tc, "Image domensions incorrect.", image->width == 4 && image->height == 4 && image->channels == 4);
    CuAssert(tc, "Image size incorrect.", 64 == image->size);

    Image_delete(image);
}

void Image_loadFromFile_shouldYieldCorrectImage(CuTest* tc)
{
    Image* image = Image_newFromData(reference, sizeof(reference), 4, 4, 4);
    Image_saveToFile(image, "test-image.png");
    Image_delete(image);

    image = Image_new();
    Image_loadFromFile(image, "test-image.png");

    CuAssert(tc, "Image contents incorrect.", memcmp(reference, image->data, 64) == 0);

    Image_delete(image);
}

CuSuite* ImageTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, Image_newFromData_works);
	SUITE_ADD_TEST(suite, Image_saveToFile_works);
	SUITE_ADD_TEST(suite, Image_loadFromFile_works);
	SUITE_ADD_TEST(suite, Image_loadFromFile_shouldYieldRightDimensions);
	SUITE_ADD_TEST(suite, Image_loadFromFile_shouldYieldCorrectImage);

	return suite;
}
