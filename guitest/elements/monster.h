#ifndef ELEMENTS_MONSTER_H
#define ELEMENTS_MONSTER_H

#include <common.h>
#include <graphics/drawable.h>
#include <graphics/spriteanimation.h>

#define MONSTER_SHELLY      1
#define MONSTER_SHROOM      2
#define MONSTER_MINKEY      3
#define MONSTER_GOLEM       4
#define MONSTER_SNELL       5
#define MONSTER_TROLLEY     6
#define MONSTER_SKELETON    7
#define MONSTER_FIRESKULL   8
#define MONSTER_BEE         9

typedef struct Monster
{
    GameObject* monsterObject;
    SpriteAnimation* walkLeft;
    SpriteAnimation* walkRight;
    SpriteAnimation* walkUp;
    SpriteAnimation* walkDown;
} Monster;

Monster newMonster(int type, float posX, float posY);
void deleteMonster(Monster monster);
void setMonsterLeft(Monster monster);
void setMonsterRight(Monster monster);
void setMonsterUp(Monster monster);
void setMonsterDown(Monster monster);

void deinitMonsters();

#endif /* ELEMENTS_MONSTER_H */
