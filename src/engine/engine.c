#include <common.h>
#include <utils/log.h>
#include <graphics/graphics.h>
#include <graphics/drawable.h>
#include <utils/linkedlist.h>
#include <engine/collisiondetector.h>

#include <engine/engine.h>

static BOOL isInitialized = FALSE;
static LinkedList* gameObjects = NULL;

static RenderQueue* currentRenderQueue = NULL;

static void _addDrawableVoid(void* gameobject)
{
    GameObject_draw((GameObject*)gameobject, currentRenderQueue);
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
    LinkedList_deleteContents(gameObjects, GameObject_deleteVoid);
    LinkedList_delete(gameObjects);

    isInitialized = FALSE;
}

void Engine_addObject(GameObject* newObj)
{
    smug_assert(isInitialized);
    smug_assert(newObj != NULL);
    LinkedList_addLast(gameObjects, newObj);
}

void Engine_addObjects(GameObject** objects, int offset, int numObjects)
{
    for (int i = offset; i < offset + numObjects; i++)
    {
        Engine_addObject(objects[i]);
    }
}

void Engine_removeObject(GameObject* obj)
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

void Engine_collideObjects()
{
    CollisionDetector_detect(gameObjects);
}

void Engine_doForAllObjects(void (*function)(GameObject*))
{
    LinkedList_doList(gameObjects, (void (*)(void*))function);
}
