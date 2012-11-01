#ifndef ELEMENTS_MONSTER_H
#define ELEMENTS_MONSTER_H

#include <common.h>
#include <engine/gameobject.h>

#include <monsters/bee.h>
#include <monsters/beetle.h>
#include <monsters/fireskull.h>
#include <monsters/goblin.h>
#include <monsters/golem.h>
#include <monsters/monkey.h>
#include <monsters/mushroom.h>
#include <monsters/skeleton.h>
#include <monsters/snail.h>
#include <monsters/tortoise.h>

#define MONSTER_STATE_NORMAL    1
#define MONSTER_STATE_ATTACKING 2
#define MONSTER_STATE_DYING     3

GameObject* newMonster(int type, float posX, float posY);

void setMonsterLeft(GameObject* monster);
void setMonsterRight(GameObject* monster);
void setMonsterUp(GameObject* monster);
void setMonsterDown(GameObject* monster);
void damageOrKillMonster(GameObject* monster, float damage);

void deinitMonsters();

#endif /* ELEMENTS_MONSTER_H */
