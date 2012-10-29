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
#define MONSTER_BEETLE      10

typedef struct CharacterData
{
    float actionGauge;
    float hp;
} CharacterData;

typedef struct MonsterData
{
    SpriteAnimation* walkLeft;
    SpriteAnimation* walkRight;
    SpriteAnimation* walkUp;
    SpriteAnimation* walkDown;
    CharacterData data;
} MonsterData;

GameObject* newMonster(int type, float posX, float posY);
void deleteMonster(GameObject* monster);
void setMonsterLeft(GameObject* monster);
void setMonsterRight(GameObject* monster);
void setMonsterUp(GameObject* monster);
void setMonsterDown(GameObject* monster);
BOOL damageMonster(GameObject* monster, float damage);

void deinitMonsters();

#endif /* ELEMENTS_MONSTER_H */
