#include <common.h>
#include <utils/log.h>
#include <graphics/drawable.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <graphics/spriteanimation.h>
#include <audio/audio.h>

#include <objects.h>
#include <monster.h>
#include <characterlogic.h>

typedef struct MonsterData
{
    CharacterLogic data;
    SpriteAnimation* walkLeft;
    SpriteAnimation* walkRight;
    SpriteAnimation* walkUp;
    SpriteAnimation* walkDown;
    int state;
} MonsterData;

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

static Sound* hitSound = NULL;

static const float WALK_FRAME_DURATION = 0.2;

#define toMonster(object) ((MonsterData*)getSpecificObjectData(object))
#define isMonster(object) objectIsOfType(object, OBJECT_MONSTER)

static void deleteMonsterData(GameObject* monster)
{
    MonsterData* data = toMonster(monster);

    Drawable* d = GameObject_removeDrawable(monster);
    Drawable_delete(d);

    SpriteAnimation_delete(data->walkLeft);
    SpriteAnimation_delete(data->walkRight);
    SpriteAnimation_delete(data->walkUp);
    SpriteAnimation_delete(data->walkDown);

    free(data);
}

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

    data->state = MONSTER_STATE_NORMAL;
    data->data.actionGauge = 100;
    data->data.hp = hp;
    GameObject* monster = newObject(data, deleteMonsterData, OBJECT_MONSTER, posX, posY);

    Drawable* d = Drawable_newFromSpriteAnimationAndSize(data->walkDown, width, height);
    Drawable_setZ(d, posY);
    GameObject_addDrawableAt(monster, d, offsetX, offsetY);

    Body* b = Body_newRectangle(width, height);
    Body_addTag(b, OBJECT_MONSTER);
    GameObject_addBodyAt(monster, b, offsetX, offsetY);

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
            imageFile = "5_res/characters/monsters/shelly32x32.png";
            dataFile = "5_res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 150;
            break;
        case MONSTER_SHROOM:
            monsterSheet = &shroomSheet;
            imageFile = "5_res/characters/monsters/shroom32x32.png";
            dataFile = "5_res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 50;
            break;
        case MONSTER_MINKEY:
            monsterSheet = &minkeySheet;
            imageFile = "5_res/characters/monsters/minkey32x32.png";
            dataFile = "5_res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 100;
            break;
        case MONSTER_GOLEM:
            monsterSheet = &golemSheet;
            imageFile = "5_res/characters/monsters/golem32x32.png";
            dataFile = "5_res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 200;
            break;
        case MONSTER_SNELL:
            monsterSheet = &snellSheet;
            imageFile = "5_res/characters/monsters/snell16x16.png";
            dataFile = "5_res/characters/monsters/16x16.txt";
            width = 16;
            height = 16;
            offsetX = -8;
            offsetY = -16;
            hp = 20;
            break;
        case MONSTER_TROLLEY:
            monsterSheet = &trolleySheet;
            imageFile = "5_res/characters/monsters/trolley16x32.png";
            dataFile = "5_res/characters/monsters/16x32.txt";
            width = 16;
            height = 32;
            offsetX = -8;
            offsetY = -32;
            hp = 70;
            break;
        case MONSTER_SKELETON:
            monsterSheet = &skeletonSheet;
            imageFile = "5_res/characters/monsters/deathletor16x32.png";
            dataFile = "5_res/characters/monsters/16x32.txt";
            width = 16;
            height = 32;
            offsetX = -8;
            offsetY = -32;
            hp = 60;
            break;
        case MONSTER_FIRESKULL:
            monsterSheet = &fireskullSheet;
            imageFile = "5_res/characters/monsters/fireskull16x32.png";
            dataFile = "5_res/characters/monsters/16x32.txt";
            width = 16;
            height = 32;
            offsetX = -8;
            offsetY = -32;
            hp = 50;
            break;
        case MONSTER_BEE:
            monsterSheet = &beeSheet;
            imageFile = "5_res/characters/monsters/bee32x32.png";
            dataFile = "5_res/characters/monsters/32x32.txt";
            width = 32;
            height = 32;
            offsetX = -16;
            offsetY = -32;
            hp = 50;
            break;
        case MONSTER_BEETLE:
            monsterSheet = &beetleSheet;
            imageFile = "5_res/characters/monsters/beetle32x32.png";
            dataFile = "5_res/characters/monsters/32x32.txt";
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
    if (hitSound == NULL)
    {
        hitSound = Sound_new("5_res/audio/flyswatter.wav");
    }

    GameObject* monster = newMonsterFromSheet(*monsterSheet, width, height, posX, posY, offsetX, offsetY, hp);
    return monster;
}

void damageOrKillMonster(GameObject* monster, float damage)
{
    if (objectIsDead(monster))
    {
        return;
    }
    if (CharacterLogic_takeDamage(monster, damage))
    {
        // Sound_play(dieSound);
        killObject(monster);
        LOG(LOG_USER1, "Hit monster with attack! It died!");
        return;
    }
    else
    {
        Sound_play(hitSound);
    }
    LOG(LOG_USER1, "Hit monster with attack! Has %i HP left.", (int)toMonster(monster)->data.hp);
}

void setMonsterLeft(GameObject* monster)
{
    if (isMonster(monster))
    {
        Drawable_useSpriteAnimation(GameObject_getDrawable(monster), toMonster(monster)->walkLeft);
    }
}

void setMonsterRight(GameObject* monster)
{
    if (isMonster(monster))
    {
        Drawable_useSpriteAnimation(GameObject_getDrawable(monster), toMonster(monster)->walkRight);
    }
}

void setMonsterUp(GameObject* monster)
{
    if (isMonster(monster))
    {
        Drawable_useSpriteAnimation(GameObject_getDrawable(monster), toMonster(monster)->walkUp);
    }
}

void setMonsterDown(GameObject* monster)
{
    if (isMonster(monster))
    {
        Drawable_useSpriteAnimation(GameObject_getDrawable(monster), toMonster(monster)->walkDown);
    }
}

void deinitMonsters()
{
    Sound_delete(hitSound);
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
