#include <common.h>
#include <utils/log.h>
#include <graphics/graphics.h>
#include <graphics/drawable.h>
#include <utils/linkedlist.h>
#include <utils/timer.h>
#include <engine/collisiondetector.h>
#include <engine/mainloop.h>

#include <engine/engine.h>

static BOOL isInitialized = FALSE;
static LinkedList* gameObjects = NULL;
static LinkedList* timers = NULL;
static LinkedList* timersToCall = NULL;

static RenderQueue* currentRenderQueue = NULL;

static void _addDrawableVoid(void* gameobject)
{
    GameObject_draw((GameObject*)gameobject, currentRenderQueue);
}

int Engine_init()
{
    gameObjects = LinkedList_new();
    timers = LinkedList_new();
    timersToCall = LinkedList_new();
    CollisionDetector_initialize();
    SpriteAnimation_initialize();

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
    LinkedList_doList(gameObjects, GameObject_deleteVoid);
    LinkedList_delete(gameObjects);
    LinkedList_doList(timers, Timer_deleteVoid);
    LinkedList_delete(timers);
    LinkedList_delete(timersToCall);
    CollisionDetector_terminate();
    SpriteAnimation_terminate();

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

void Engine_removeAndDeleteAllObjects()
{
    LinkedList_doList(gameObjects, (void (*)(void*))GameObject_delete);
    Engine_removeAllObjects();
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
    CollisionDetector_callCollisionCallbacks();
}

void Engine_doForAllObjects(void (*function)(GameObject*))
{
    LinkedList_doList(gameObjects, (void (*)(void*))function);
}

void Engine_delayCallback(void(*callback)(void*), void* argument, TIME delay)
{
    Timer* newTimer = Timer_new(callback, argument, delay);
    LinkedList_addLast(timers, newTimer);
    Timer_start(newTimer, Mainloop_getTime());
}

static TIME timeForTimerCheck = 0.0;
void _callTimer(void* timer)
{
    Timer_call((Timer*)timer, timeForTimerCheck);
}

void _checkTimer(void* timer)
{
    if (Timer_check((Timer*)timer, timeForTimerCheck))
    {
        LinkedList_addLast(timersToCall, timer);
    }
}

static BOOL _isTimerFinished(void* timer)
{
    return Timer_isFinished((Timer*)timer);
}

void Engine_checkDelayedCallbacks()
{
    timeForTimerCheck = Mainloop_getTime();
    LinkedList_doList(timers, _checkTimer);
    LinkedList_doList(timersToCall, _callTimer);
    LinkedList_removeThose(timers, _isTimerFinished);
    LinkedList_removeAll(timersToCall);
}
