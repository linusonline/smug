#include <common.h>
#include <engine/gameobject.h>
#include <engine/engine.h>

#include <objects.h>

typedef struct Generic
{
    void (*dataDeleter)(GameObject*);
    int type;
    void* specific;
} Generic;

static LinkedList* objectsToDelete = NULL;

static void _removeAndDeleteObject(void* object)
{
    if (object == NULL)
    {
        return;
    }
    Engine_removeObject((GameObject*)object);
    GameObject_delete((GameObject*)object);
}

static Generic* _getGeneric(GameObject* object)
{
    return ((Generic*)GameObject_getUserData(object));
}

static void _nullDeleter(GameObject* ignored)
{
}

void Objects_initialize()
{
    objectsToDelete = LinkedList_new();
}

void Objects_terminate()
{
    LinkedList_delete(objectsToDelete);
}

GameObject* newObject(void* specificData, void (*specificDataDeleter)(GameObject*), int type, float posX, float posY)
{
    Generic* newGeneric = allocate(Generic);
    newGeneric->dataDeleter = specificDataDeleter;
    newGeneric->type = type;
    newGeneric->specific = specificData;

    GameObject* newObject = GameObject_new(posX, posY);
    GameObject_setUserData(newObject, newGeneric);
    return newObject;
}

GameObject* newObjectNoData(int type, float posX, float posY)
{
    return newObject(NULL, _nullDeleter, type, posX, posY);
}

BOOL objectIsOfType(GameObject* object, int type)
{
    smug_assert(object != NULL);
    if (objectIsDead(object))
    {
        return FALSE;
    }
    return _getGeneric(object)->type == type;
}

void* getSpecificObjectData(GameObject* object)
{
    smug_assert(object != NULL);
    return _getGeneric(object)->specific;
}

void killObject(GameObject* object)
{
    smug_assert(object != NULL);
    if (objectIsDead(object))
    {
        return;
    }
    smug_assert(objectsToDelete != NULL);
    _getGeneric(object)->dataDeleter(object);
    free(_getGeneric(object));
    GameObject_setUserData(object, NULL);
    LinkedList_addLast(objectsToDelete, (void*)object);
}

BOOL objectIsDead(GameObject* object)
{
    smug_assert(object != NULL);
    return GameObject_getUserData(object) == NULL;
}

void removeAndDeleteDeadObjects()
{
    LinkedList_doList(objectsToDelete, _removeAndDeleteObject);
    LinkedList_removeAll(objectsToDelete);
}

void removeAndDeleteObject(GameObject* object)
{
    smug_assert(object != NULL);
    if (!objectIsDead(object))
    {
        killObject(object);
    }
    _removeAndDeleteObject(object);
}
