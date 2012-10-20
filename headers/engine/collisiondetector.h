#ifndef SMUG_ENGINE_COLLISION_DETECTOR_H
#define SMUG_ENGINE_COLLISION_DETECTOR_H

#include <common.h>
#include <engine/body.h>
#include <engine/gameobject.h>
#include <utils/linkedlist.h>

typedef void (*CollisionCallback)(GameObject* object1, GameObject* object2);

void CollisionDetector_detect(LinkedList* objects);
void CollisionDetector_collideTags(int tag1, int tag2, CollisionCallback callback);

#endif /* SMUG_ENGINE_COLLISION_DETECTOR_H */

