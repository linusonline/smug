#include <common.h>
#include <utils/log.h>
#include <graphics/graphics.h>
#include <graphics/drawable.h>
#include <utils/linkedlist.h>

#include <engine/engine.h>

static BOOL isInitialized = FALSE;
static LinkedList* gameObjects = NULL;

static RenderQueue* currentRenderQueue = NULL;

static void _addDrawableVoid(void* drawable)
{
    RenderQueue_addDrawable(currentRenderQueue, (Drawable*)drawable);
}

int Engine_init()
{
    gameObjects = LinkedList_new();

    isInitialized = TRUE;
    return 1;
}

BOOL Engine_isInitialized()
{
    return isInitialized;
}

void Engine_terminate()
{
    smug_assert(isInitialized);
    LinkedList_deleteContents(gameObjects, Drawable_deleteVoid);
    LinkedList_delete(gameObjects);

    isInitialized = FALSE;
}

void Engine_addObject(Drawable* newObj)
{
    smug_assert(isInitialized);
    LinkedList_addLast(gameObjects, newObj);
}

void Engine_addObjects(Drawable** objects, int offset, int numObjects)
{
    for (int i = offset; i < offset + numObjects; i++)
    {
        Engine_addObject(objects[i]);
    }
}

void Engine_removeObject(Drawable* obj)
{
    smug_assert(isInitialized);
    LinkedList_removeItem(gameObjects, obj);
}

void Engine_removeAllObjects()
{
    LinkedList_removeAll(gameObjects);
}

void Engine_drawObjects(RenderQueue* renderQueue)
{
    currentRenderQueue = renderQueue;
    LinkedList_doList(gameObjects, _addDrawableVoid);
    currentRenderQueue = NULL;
}
