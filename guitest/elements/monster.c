#include <common.h>
#include <utils/log.h>
#include <graphics/drawable.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <graphics/spriteanimation.h>

#include <monster.h>

static SpriteSheet* shellySheet = NULL;
static SpriteSheet* shroomSheet = NULL;
static SpriteSheet* minkeySheet = NULL;
static SpriteSheet* golemSheet = NULL;
static SpriteSheet* snellSheet = NULL;
static SpriteSheet* trolleySheet = NULL;
static SpriteSheet* skeletonSheet = NULL;
static SpriteSheet* fireskullSheet = NULL;
static SpriteSheet* beeSheet = NULL;

static const float WALK_FRAME_DURATION = 0.2;

Monster newMonsterFromSheet(SpriteSheet* sheet, float width, float height, float posX, float posY)
{
    Monster monster;
    monster.walkDown = SpriteAnimation_newEmpty();
    monster.walkUp = SpriteAnimation_newEmpty();
    monster.walkRight = SpriteAnimation_newEmpty();
    monster.walkLeft = SpriteAnimation_newEmpty();

    SpriteAnimation_addFrame(monster.walkDown, SpriteSheet_getSpriteXY(sheet, 0, 0), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkDown, SpriteSheet_getSpriteXY(sheet, 1, 0), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkDown, SpriteSheet_getSpriteXY(sheet, 0, 0), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkDown, SpriteSheet_getSpriteXY(sheet, 2, 0), WALK_FRAME_DURATION);

    SpriteAnimation_addFrame(monster.walkUp, SpriteSheet_getSpriteXY(sheet, 0, 1), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkUp, SpriteSheet_getSpriteXY(sheet, 1, 1), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkUp, SpriteSheet_getSpriteXY(sheet, 0, 1), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkUp, SpriteSheet_getSpriteXY(sheet, 2, 1), WALK_FRAME_DURATION);

    SpriteAnimation_addFrame(monster.walkRight, SpriteSheet_getSpriteXY(sheet, 0, 2), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkRight, SpriteSheet_getSpriteXY(sheet, 1, 2), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkRight, SpriteSheet_getSpriteXY(sheet, 0, 2), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkRight, SpriteSheet_getSpriteXY(sheet, 2, 2), WALK_FRAME_DURATION);

    SpriteAnimation_addFrame(monster.walkLeft, SpriteSheet_getSpriteXY(sheet, 0, 3), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkLeft, SpriteSheet_getSpriteXY(sheet, 1, 3), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkLeft, SpriteSheet_getSpriteXY(sheet, 0, 3), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(monster.walkLeft, SpriteSheet_getSpriteXY(sheet, 2, 3), WALK_FRAME_DURATION);

    SpriteAnimation_start(monster.walkLeft);
    SpriteAnimation_start(monster.walkRight);
    SpriteAnimation_start(monster.walkUp);
    SpriteAnimation_start(monster.walkDown);

    Drawable* d = Drawable_newFromSpriteAnimationAndDimensions(monster.walkDown, width, height);
    Drawable_setZ(d, posY);
    monster.monsterObject = GameObject_new(posX, posY);
    GameObject_addDrawableAt(monster.monsterObject, d, 0.0f, 0.0f);
    return monster;
}

Monster newMonster(int type, float posX, float posY)
{
    SpriteSheet** monsterSheet = NULL;
    static char* imageFile = NULL;
    static char* dataFile = NULL;
    float width, height;
    switch (type)
    {
        case MONSTER_SHELLY:
            monsterSheet = &shellySheet;
            imageFile = "res/characters/monsters/shelly32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            break;
        case MONSTER_SHROOM:
            monsterSheet = &shroomSheet;
            imageFile = "res/characters/monsters/shroom32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            break;
        case MONSTER_MINKEY:
            monsterSheet = &minkeySheet;
            imageFile = "res/characters/monsters/minkey32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            break;
        case MONSTER_GOLEM:
            monsterSheet = &golemSheet;
            imageFile = "res/characters/monsters/golem32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            break;
        case MONSTER_SNELL:
            monsterSheet = &snellSheet;
            imageFile = "res/characters/monsters/snell16x16.png";
            dataFile = "res/characters/monsters/16x16.txt";
            width = 16;
            height = 16;
            break;
        case MONSTER_TROLLEY:
            monsterSheet = &trolleySheet;
            imageFile = "res/characters/monsters/trolley16x32.png";
            dataFile = "res/characters/monsters/16x32.txt";
            width = 16;
            height = 32;
            break;
        case MONSTER_SKELETON:
            monsterSheet = &skeletonSheet;
            imageFile = "res/characters/monsters/deathletor16x32.png";
            dataFile = "res/characters/monsters/16x32.txt";
            width = 16;
            height = 32;
            break;
        case MONSTER_FIRESKULL:
            monsterSheet = &fireskullSheet;
            imageFile = "res/characters/monsters/fireskull16x32.png";
            dataFile = "res/characters/monsters/16x32.txt";
            width = 16;
            height = 32;
            break;
        case MONSTER_BEE:
            monsterSheet = &beeSheet;
            imageFile = "res/characters/monsters/bee32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            break;
        default:
            smug_assert(FALSE);
    }
    if (*monsterSheet == NULL)
    {
        *monsterSheet = SpriteSheet_new(imageFile, dataFile);
    }
    return newMonsterFromSheet(*monsterSheet, width, height, posX, posY);
}

void deleteMonster(Monster monster)
{
    GameObject_delete(monster.monsterObject);
    SpriteAnimation_delete(monster.walkLeft);
    SpriteAnimation_delete(monster.walkRight);
    SpriteAnimation_delete(monster.walkUp);
    SpriteAnimation_delete(monster.walkDown);
}

void setMonsterLeft(Monster monster)
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(monster.monsterObject), monster.walkLeft);
}

void setMonsterRight(Monster monster)
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(monster.monsterObject), monster.walkRight);
}

void setMonsterUp(Monster monster)
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(monster.monsterObject), monster.walkUp);
}

void setMonsterDown(Monster monster)
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(monster.monsterObject), monster.walkDown);
}

void deinitMonsters()
{
    if (shellySheet != NULL)
    {
        SpriteSheet_delete(shellySheet);
    }
    if (shroomSheet != NULL)
    {
        SpriteSheet_delete(shroomSheet);
    }
    if (minkeySheet != NULL)
    {
        SpriteSheet_delete(minkeySheet);
    }
    if (golemSheet != NULL)
    {
        SpriteSheet_delete(golemSheet);
    }
    if (snellSheet != NULL)
    {
        SpriteSheet_delete(snellSheet);
    }
    if (trolleySheet != NULL)
    {
        SpriteSheet_delete(trolleySheet);
    }
    if (skeletonSheet != NULL)
    {
        SpriteSheet_delete(skeletonSheet);
    }
    if (fireskullSheet != NULL)
    {
        SpriteSheet_delete(fireskullSheet);
    }
    if (beeSheet != NULL)
    {
        SpriteSheet_delete(beeSheet);
    }
}
