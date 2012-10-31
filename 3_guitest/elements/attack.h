#ifndef ELEMENTS_ATTACK_H
#define ELEMENTS_ATTACK_H

#include <common.h>
#include <engine/gameobject.h>

GameObject* createAttack(float posX, float posY);

void setAttackHasHitObject(GameObject* attack, GameObject* other);
BOOL attackHasHitObject(GameObject* attack, GameObject* other);

#endif /* ELEMENTS_ATTACK_H */
