#include <common.h>
#include <utils/log.h>
#include <engine/body.h>

#include <engine/collisiondetector.h>

// TODO: Implement list of tag combinations.

static CollisionCallback collisionCallback;

static CollisionCallback _getCallbackForCollision(GameObject* obj1, GameObject* obj2)
{
    if (GameObject_getBody(obj1) == NULL || GameObject_getBody(obj2) == NULL)
    {
        return FALSE;
    }
    return collisionCallback;
}

void CollisionDetector_detect(LinkedList* objects)
{
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
                callback(obj1, obj2);
            }
            obj2 = (GameObject*)LinkedListIterator_getNext(&second);
        }
        obj1 = (GameObject*)LinkedListIterator_getNext(&first);
        LinkedListIterator_setTo(&second, &first);
        obj2 = (GameObject*)LinkedListIterator_getNext(&second);
    }
}

void CollisionDetector_collideTags(int tag1, int tag2, CollisionCallback callback)
{
    collisionCallback = callback;
}
