#include <common.h>
#include <utils/log.h>
#include <graphics/drawable.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <graphics/spriteanimation.h>

#include <objects.h>
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
static SpriteSheet* beetleSheet = NULL;

static const float WALK_FRAME_DURATION = 0.2;

#define toMonster(object) ((MonsterData*)GameObject_getUserData(object))

static GameObject* newMonsterFromSheet(SpriteSheet* sheet, float width, float height, float posX, float posY, float offsetX, float offsetY, float hp)
{
    MonsterData* data = allocate(MonsterData);
    data->walkDown = SpriteAnimation_newEmpty();
    data->walkUp = SpriteAnimation_newEmpty();
    data->walkRight = SpriteAnimation_newEmpty();
    data->walkLeft = SpriteAnimation_newEmpty();

    SpriteAnimation_addFrame(data->walkDown, SpriteSheet_getSpriteXY(sheet, 0, 0), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkDown, SpriteSheet_getSpriteXY(sheet, 1, 0), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkDown, SpriteSheet_getSpriteXY(sheet, 0, 0), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkDown, SpriteSheet_getSpriteXY(sheet, 2, 0), WALK_FRAME_DURATION);

    SpriteAnimation_addFrame(data->walkUp, SpriteSheet_getSpriteXY(sheet, 0, 1), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkUp, SpriteSheet_getSpriteXY(sheet, 1, 1), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkUp, SpriteSheet_getSpriteXY(sheet, 0, 1), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkUp, SpriteSheet_getSpriteXY(sheet, 2, 1), WALK_FRAME_DURATION);

    SpriteAnimation_addFrame(data->walkRight, SpriteSheet_getSpriteXY(sheet, 0, 2), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkRight, SpriteSheet_getSpriteXY(sheet, 1, 2), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkRight, SpriteSheet_getSpriteXY(sheet, 0, 2), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkRight, SpriteSheet_getSpriteXY(sheet, 2, 2), WALK_FRAME_DURATION);

    SpriteAnimation_addFrame(data->walkLeft, SpriteSheet_getSpriteXY(sheet, 0, 3), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkLeft, SpriteSheet_getSpriteXY(sheet, 1, 3), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkLeft, SpriteSheet_getSpriteXY(sheet, 0, 3), WALK_FRAME_DURATION);
    SpriteAnimation_addFrame(data->walkLeft, SpriteSheet_getSpriteXY(sheet, 2, 3), WALK_FRAME_DURATION);

    SpriteAnimation_start(data->walkLeft);
    SpriteAnimation_start(data->walkRight);
    SpriteAnimation_start(data->walkUp);
    SpriteAnimation_start(data->walkDown);

    GameObject* monster = GameObject_new(posX, posY);

    Drawable* d = Drawable_newFromSpriteAnimationAndSize(data->walkDown, width, height);
    Drawable_setZ(d, posY);
    GameObject_addDrawableAt(monster, d, offsetX, offsetY);

    Body* b = Body_newRectangle(width, height);
    Body_addTag(b, OBJECT_MONSTER);
    GameObject_addBodyAt(monster, b, offsetX, offsetY);

    data->data.actionGauge = 100;
    data->data.hp = hp;
    GameObject_setUserData(monster, data);
    return monster;
}

GameObject* newMonster(int type, float posX, float posY)
{
    SpriteSheet** monsterSheet = NULL;
    static char* imageFile = NULL;
    static char* dataFile = NULL;
    float width, height;
    float hp;
    float offsetX, offsetY;
    switch (type)
    {
        case MONSTER_SHELLY:
            monsterSheet = &shellySheet;
            imageFile = "res/characters/monsters/shelly32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 150;
            break;
        case MONSTER_SHROOM:
            monsterSheet = &shroomSheet;
            imageFile = "res/characters/monsters/shroom32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 50;
            break;
        case MONSTER_MINKEY:
            monsterSheet = &minkeySheet;
            imageFile = "res/characters/monsters/minkey32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 100;
            break;
        case MONSTER_GOLEM:
            monsterSheet = &golemSheet;
            imageFile = "res/characters/monsters/golem32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 200;
            break;
        case MONSTER_SNELL:
            monsterSheet = &snellSheet;
            imageFile = "res/characters/monsters/snell16x16.png";
            dataFile = "res/characters/monsters/16x16.txt";
            width = 16;
            height = 16;
            offsetX = -8;
            offsetY = -16;
            hp = 20;
            break;
        case MONSTER_TROLLEY:
            monsterSheet = &trolleySheet;
            imageFile = "res/characters/monsters/trolley16x32.png";
            dataFile = "res/characters/monsters/16x32.txt";
            width = 16;
            height = 32;
            offsetX = -8;
            offsetY = -32;
            hp = 70;
            break;
        case MONSTER_SKELETON:
            monsterSheet = &skeletonSheet;
            imageFile = "res/characters/monsters/deathletor16x32.png";
            dataFile = "res/characters/monsters/16x32.txt";
            width = 16;
            height = 32;
            offsetX = -8;
            offsetY = -32;
            hp = 60;
            break;
        case MONSTER_FIRESKULL:
            monsterSheet = &fireskullSheet;
            imageFile = "res/characters/monsters/fireskull16x32.png";
            dataFile = "res/characters/monsters/16x32.txt";
            width = 16;
            height = 32;
            offsetX = -8;
            offsetY = -32;
            hp = 50;
            break;
        case MONSTER_BEE:
            monsterSheet = &beeSheet;
            imageFile = "res/characters/monsters/bee32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 50;
            break;
        case MONSTER_BEETLE:
            monsterSheet = &beetleSheet;
            imageFile = "res/characters/monsters/beetle32x32.png";
            dataFile = "res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 100;
            break;
        default:
            smug_assert(FALSE);
    }
    if (*monsterSheet == NULL)
    {
        *monsterSheet = SpriteSheet_new(imageFile, dataFile);
    }
    GameObject* monster = newMonsterFromSheet(*monsterSheet, width, height, posX, posY, offsetX, offsetY, hp);
    return monster;
}

BOOL damageMonster(GameObject* monster, float damage)
{
    MonsterData* data = toMonster(monster);
    BOOL dead = data->data.hp - damage <= 0;
    data->data.hp = max(0, data->data.hp - damage);
    return dead;
}

void deleteMonster(GameObject* monster)
{
    MonsterData* data = toMonster(monster);
    SpriteAnimation_delete(data->walkLeft);
    SpriteAnimation_delete(data->walkRight);
    SpriteAnimation_delete(data->walkUp);
    SpriteAnimation_delete(data->walkDown);
    GameObject_delete(monster);
}

void setMonsterLeft(GameObject* monster)
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(monster), toMonster(monster)->walkLeft);
}

void setMonsterRight(GameObject* monster)
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(monster), toMonster(monster)->walkRight);
}

void setMonsterUp(GameObject* monster)
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(monster), toMonster(monster)->walkUp);
}

void setMonsterDown(GameObject* monster)
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(monster), toMonster(monster)->walkDown);
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
    if (beetleSheet != NULL)
    {
        SpriteSheet_delete(beetleSheet);
    }
}
