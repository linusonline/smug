#include <common.h>
#include <utils/log.h>
#include <utils/hook.h>
#include <utils/linkedlist.h>
#include <engine/body.h>

#include <engine/collisiondetector.h>

// TODO: Implement list of tag combinations.

static CollisionCallback collisionCallback;

static LinkedList* collisionHooks = NULL;

static CollisionCallback _getCallbackForCollision(GameObject* obj1, GameObject* obj2)
{
    if (GameObject_getBody(obj1) == NULL || GameObject_getBody(obj2) == NULL)
    {
        return FALSE;
    }
    return collisionCallback;
}

void CollisionDetector_initialize()
{
    collisionHooks = LinkedList_new();
}

void CollisionDetector_terminate()
{
    smug_assert(collisionHooks != NULL);
    LinkedList_delete(collisionHooks);
}

void CollisionDetector_detect(LinkedList* objects)
{
    smug_assert(collisionHooks != NULL);
    LinkedListIterator first = LinkedList_getIterator(objects);
    LinkedListIterator second = LinkedList_getIterator(objects);

    GameObject* obj1 = (GameObject*)LinkedListIterator_getCurrent(&first);
    while (LinkedListIterator_hasMore(first))
    {
        GameObject* obj2 = (GameObject*)LinkedListIterator_getNext(&second);

        while (obj2 != NULL)
        {
            CollisionCallback callback = _getCallbackForCollision(obj1, obj2);
            if (callback != NULL && Body_colliding(GameObject_getBody(obj1), GameObject_getBody(obj2)))
            {
                LinkedList_addLast(collisionHooks, (void*)Hook_new2Params((void (*)(void*, void*))callback, obj1, obj2));
            }
            obj2 = (GameObject*)LinkedListIterator_getNext(&second);
        }
        obj1 = (GameObject*)LinkedListIterator_getNext(&first);
        LinkedListIterator_setTo(&second, &first);
        obj2 = (GameObject*)LinkedListIterator_getNext(&second);
    }
}

void CollisionDetector_callCollisionCallbacks()
{
    LinkedList_doList(collisionHooks, (void (*)(void*))Hook_call);
    LinkedList_doList(collisionHooks, (void (*)(void*))Hook_delete2Params);
    LinkedList_removeAll(collisionHooks);
}

void CollisionDetector_collideTags(int tag1, int tag2, CollisionCallback callback)
{
    collisionCallback = callback;
}
