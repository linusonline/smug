#include <common.h>
#include <utils/log.h>
#include <graphics/drawable.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <graphics/spriteanimation.h>
#include <audio/sound.h>
#include <audio/soundfactory.h>

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
    Sound* hitSound;
    Sound* dieSound;
} MonsterData;

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

// Protected
GameObject* newMonsterFromSheet(SpriteSheet* sheet, float width, float height, float posX, float posY, float offsetX, float offsetY, float hp, Sound* hitSound, Sound* dieSound)
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

    data->hitSound = hitSound;
    data->dieSound = dieSound;
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

void damageOrKillMonster(GameObject* monster, float damage)
{
    if (objectIsDead(monster))
    {
        return;
    }
    if (CharacterLogic_takeDamage(monster, damage))
    {
        Sound_play(toMonster(monster)->dieSound);
        killObject(monster);
        LOG(LOG_USER1, "Hit monster with attack! It died!");
        return;
    }
    else
    {
        Sound_play(toMonster(monster)->hitSound);
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
