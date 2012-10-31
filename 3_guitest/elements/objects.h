#ifndef ELEMENTS_OBJECTS_H
#define ELEMENTS_OBJECTS_H

#include <common.h>
#include <engine/gameobject.h>

#define OBJECT_AVATAR       1
#define OBJECT_MONSTER      2
#define OBJECT_ATTACK       3
#define OBJECT_BACKGROUND   4
#define OBJECT_GENERIC      5

void Objects_initialize();
void Objects_terminate();

GameObject* newObject(void* specificData, void (*specificDataDeleter)(GameObject*), int type, float posX, float posY);
GameObject* newObjectNoData(int type, float posX, float posY);
BOOL objectIsOfType(GameObject* object, int type);
void* getSpecificObjectData(GameObject* object);

void killObject(GameObject* object);
BOOL objectIsDead(GameObject* object);
void removeAndDeleteDeadObjects(void);

/** Removes the object from the engine and deletes it. The object may or may not
 *  be dead. Should generally only be used just before shutting down engine.
 */
void removeAndDeleteObject(GameObject* object);

#endif /* ELEMENTS_OBJECTS_H */
