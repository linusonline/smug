#ifndef ELEMENTS_CHARACTER_LOGIC_H
#define ELEMENTS_CHARACTER_LOGIC_H

#include <common.h>
#include <engine/gameobject.h>

typedef struct CharacterLogic
{
    float actionGauge;
    float hp;
} CharacterLogic;

BOOL CharacterLogic_takeDamage(GameObject* character, float damage);

#endif /* ELEMENTS_CHARACTER_LOGIC_H */
