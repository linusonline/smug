#ifndef ELEMENTS_MONSTER_H
#define ELEMENTS_MONSTER_H

#include <common.h>
#include <engine/gameobject.h>

#define MONSTER_SHROOM      2
#define MONSTER_MINKEY      3
#define MONSTER_GOLEM       4
#define MONSTER_SNELL       5
#define MONSTER_TROLLEY     6
#define MONSTER_SKELETON    7
#define MONSTER_FIRESKULL   8
#define MONSTER_BEE         9
#define MONSTER_BEETLE      10

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
