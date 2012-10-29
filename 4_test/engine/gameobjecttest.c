#include <CuTest.h>
#include <string.h>

#include <engine/gameobject.h>
#include <graphics/renderqueue.h>
#include <graphics/renderbatch.h>
#include <graphics/drawable.h>
#include <utils/map.h>

void GameObject_new_shouldReturnNonNull(CuTest* tc)
{
    GameObject* go = GameObject_new(0, 0);
    CuAssertTrue(tc, go != NULL);
    GameObject_delete(go);
}

void GameObject_new_shouldReturnObjectWithCorrectPosition(CuTest* tc)
{
    GameObject* go = GameObject_new(1.0f, 2.0f);
    CuAssertTrue(tc, GameObject_getX(go) == 1.0f);
    CuAssertTrue(tc, GameObject_getY(go) == 2.0f);
    GameObject_delete(go);
}

static float vertexTestArray[12] = {2.0, 2.0, 0.0,
                                    2.0, 3.0, 0.0,
                                    3.0, 3.0, 0.0,
                                    3.0, 2.0, 0.0};

void GameObject_draw_shouldDrawChildObjectToCorrectPosition(CuTest* tc)
{
    RenderQueue* rq = RenderQueue_new();
    GameObject* parent = GameObject_new(1, 1);
    GameObject* child = GameObject_new(1, 1);
    Drawable* d = Drawable_newFromColorAndSize(0.5, 0.5, 0.5, 0.5, 1, 1);
    GameObject_addDrawableAt(child, d, 0, 0);
    GameObject_addObject(parent, child);
    GameObject_draw(child, rq);
    Map* map = RenderQueue_getBatches(rq);
    int id = 0;
    RenderBatch* rb = (RenderBatch*)Map_get(map, &id);
    float* va = RenderBatch_getVertexArray(rb);

    CuAssertTrue(tc, memcmp(va, vertexTestArray, sizeof(float) * 12) == 0);

    GameObject_delete(parent);
    GameObject_delete(child);
    RenderQueue_delete(rq);
}

CuSuite* GameObjectTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, GameObject_new_shouldReturnNonNull);
	SUITE_ADD_TEST(suite, GameObject_new_shouldReturnObjectWithCorrectPosition);
	SUITE_ADD_TEST(suite, GameObject_draw_shouldDrawChildObjectToCorrectPosition);
	return suite;
}
