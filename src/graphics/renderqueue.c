#include <GL/glfw.h> // We can get rid of this dependency

#include <utils/log.h>
#include <utils/map.h>
#include <graphics/sprite.h>
#include <graphics/renderqueue.h>

typedef struct _RenderQueue
{
    Map* renderBatches;
} _RenderQueue;

static const int RENDERBATCH_INITIAL_SIZE = 16;

static BOOL _invariant(RenderQueue* self)
{
    return
        self != NULL &&
        self->renderBatches != NULL;
}

RenderQueue* RenderQueue_new()
{
    RenderQueue* newRenderQueue = allocate(RenderQueue);
    newRenderQueue->renderBatches = Map_new(Map_compareInts);
    smug_assert(_invariant(newRenderQueue));
    return newRenderQueue;
}

static void _deleteRenderBatchInMap(void* key, void* renderBatch)
{
    RenderBatch_delete((RenderBatch*)renderBatch);
    free((int*)key);
}

static void _clearRenderBatchInMap(void* keyIgnored, void* renderBatch)
{
    RenderBatch_clear((RenderBatch*)renderBatch);
}

void RenderQueue_delete(RenderQueue* self)
{
    smug_assert(_invariant(self));
    Map_doForEach(self->renderBatches, _deleteRenderBatchInMap);
    Map_delete(self->renderBatches);
    free(self);
}

void RenderQueue_clear(RenderQueue* self)
{
    smug_assert(_invariant(self));
    Map_doForEach(self->renderBatches, _clearRenderBatchInMap);
}

int* _allocInt(int value)
{
    int* i = allocate(int);
    *i = value;
    return i;
}

void RenderQueue_addDrawable(RenderQueue* self, Drawable* drawable, float positionX, float positionY)
{
    smug_assert(_invariant(self));
    Sprite* sprite = Drawable_getSprite(drawable);
    smug_assert(sprite != NULL);
    int id = Sprite_getTextureId(sprite);
    RenderBatch* renderBatch = (RenderBatch*)Map_get(self->renderBatches, &id);
    if (renderBatch == NULL)
    {
        renderBatch = RenderBatch_new(RENDERBATCH_INITIAL_SIZE, TRUE);
        Map_set(self->renderBatches, _allocInt(id), renderBatch);
    }
    Drawable_addRenderData(drawable, renderBatch, positionX, positionY);
}

/** Requires OpenGL to be initialized, and GL_VERTEX_ARRAY and
 *  GL_TEXTURE_COORD_ARRAY to be enabled.
 */
void RenderQueue_render(RenderQueue* self)
{
    smug_assert(_invariant(self));
    MapIterator* iter = Map_getIterator(self->renderBatches);
    MapPair* pair = MapIterator_getNextPair(iter);
    while (pair != NULL)
    {
        glBindTexture(GL_TEXTURE_2D, *(int*)pair->key);
        RenderBatch_render((RenderBatch*)pair->value);
        pair = MapIterator_getNextPair(iter);
    }
    MapIterator_delete(iter);
}
