#include <graphics/renderbatch.h>
#include <string.h>
#include <GL/glfw.h>
#include <CuTest.h>

static const int INITIAL_BATCH_SIZE = 8;
static float vertexTestArray[24] = {1.0, 1.0, -1.0,
                                    1.0, 1.0, -1.0,
                                    1.0, 1.0, -1.0,
                                    1.0, 1.0, -1.0,
                                    1.0, 1.0, -1.0,
                                    1.0, 1.0, -1.0,
                                    1.0, 1.0, -1.0,
                                    1.0, 1.0, -1.0};

static float colorAndTexCoordTestArray[16] = {1.0, 1.0,
                                              1.0, 1.0,
                                              1.0, 1.0,
                                              1.0, 1.0,
                                              1.0, 1.0,
                                              1.0, 1.0,
                                              1.0, 1.0,
                                              1.0, 1.0,};

void RenderBatch_new_shouldWorkWithSmallInitialSize(CuTest* tc)
{
    RenderBatch* rb = RenderBatch_new(1, TRUE);
    CuAssertTrue(tc, rb != NULL);
    RenderBatch_delete(rb);
}

void RenderBatch_addTexturedRect_shouldWorkForTexturedBatch(CuTest* tc)
{
    RenderBatch* rb = RenderBatch_new(INITIAL_BATCH_SIZE, TRUE);
    RenderBatch_addTexturedRect(rb, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    RenderBatch_delete(rb);
}

void RenderBatch_addColoredRect_shouldWorkForColoredBatch(CuTest* tc)
{
    RenderBatch* rb = RenderBatch_new(INITIAL_BATCH_SIZE, FALSE);
    RenderBatch_addColoredRect(rb, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    RenderBatch_delete(rb);
}

void RenderBatch_addTexturedRect_shouldAddRightNumberOfElements(CuTest* tc)
{
    RenderBatch* rb = RenderBatch_new(INITIAL_BATCH_SIZE, TRUE);
    RenderBatch_addTexturedRect(rb, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    CuAssertTrue(tc, RenderBatch_getNumberOfAddedElements(rb) == 4);
    RenderBatch_delete(rb);
}

void RenderBatch_addColoredRect_shouldAddRightNumberOfElements(CuTest* tc)
{
    RenderBatch* rb = RenderBatch_new(INITIAL_BATCH_SIZE, FALSE);
    RenderBatch_addColoredRect(rb, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    CuAssertTrue(tc, RenderBatch_getNumberOfAddedElements(rb) == 4);
    RenderBatch_delete(rb);
}

void RenderBatch_addTexturedRect_shouldAddRightElements(CuTest* tc)
{
    RenderBatch* rb = RenderBatch_new(INITIAL_BATCH_SIZE, TRUE);
    RenderBatch_addTexturedRect(rb, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0);
    float* va = RenderBatch_getVertexArray(rb);
    CuAssertTrue(tc, memcmp(va, vertexTestArray, sizeof(float) * 12) == 0);
    float* ta = RenderBatch_getTexCoordArray(rb);
    CuAssertTrue(tc, memcmp(ta, colorAndTexCoordTestArray, sizeof(float) * 8) == 0);
    RenderBatch_delete(rb);
}

void RenderBatch_addColoredRect_shouldAddRightElements(CuTest* tc)
{
    RenderBatch* rb = RenderBatch_new(INITIAL_BATCH_SIZE, FALSE);
    RenderBatch_addColoredRect(rb, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0);
    float* va = RenderBatch_getVertexArray(rb);
    CuAssertTrue(tc, memcmp(va, vertexTestArray, sizeof(float) * 8) == 0);
    float* ca = RenderBatch_getColorArray(rb);
    CuAssertTrue(tc, memcmp(ca, colorAndTexCoordTestArray, sizeof(float) * 16) == 0);
    RenderBatch_delete(rb);
}

CuSuite* RenderBatchTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, RenderBatch_new_shouldWorkWithSmallInitialSize);
	SUITE_ADD_TEST(suite, RenderBatch_addTexturedRect_shouldWorkForTexturedBatch);
	SUITE_ADD_TEST(suite, RenderBatch_addColoredRect_shouldWorkForColoredBatch);
	SUITE_ADD_TEST(suite, RenderBatch_addTexturedRect_shouldAddRightNumberOfElements);
	SUITE_ADD_TEST(suite, RenderBatch_addColoredRect_shouldAddRightNumberOfElements);
	SUITE_ADD_TEST(suite, RenderBatch_addTexturedRect_shouldAddRightElements);
	SUITE_ADD_TEST(suite, RenderBatch_addColoredRect_shouldAddRightElements);

	return suite;
}
