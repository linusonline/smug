#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <graphics/drawable.h>
#include <graphics/spriteanimation.h>

#include <objects.h>
#include <attack.h>

static SpriteSheet* attackSheet = NULL;
static const char* imageFile = "5_res/effects/cold.png";
static const char* dataFile = "5_res/effects/32x32.txt";

static const float ATTACK_FRAME_DURATION = 0.1;

#define toAttack(object) ((AttackData*)getSpecificObjectData(object))
#define isAttack(object) objectIsOfType(object, OBJECT_ATTACK)

typedef struct AttackData
{
    LinkedList* hitObjects;
    SpriteAnimation* animation;
} AttackData;

static void deleteAttackData(GameObject* attack)
{
    AttackData* data = toAttack(attack);

    Drawable* d = GameObject_removeDrawable(attack);
    Drawable_delete(d);

    SpriteAnimation_delete(data->animation);
    LinkedList_delete(data->hitObjects);

    free(data);
}

GameObject* createAttack(float posX, float posY)
{
    AttackData* data = allocate(AttackData);
    data->hitObjects = LinkedList_new();

    if (attackSheet == NULL)
    {
        attackSheet = SpriteSheet_new(imageFile, dataFile);
    }
    SpriteAnimation* a = SpriteAnimation_newEmpty();

    SpriteAnimation_addFrame(a, SpriteSheet_getSprite(attackSheet, 0), ATTACK_FRAME_DURATION);
    SpriteAnimation_addFrame(a, SpriteSheet_getSprite(attackSheet, 1), ATTACK_FRAME_DURATION);
    SpriteAnimation_addFrame(a, SpriteSheet_getSprite(attackSheet, 2), ATTACK_FRAME_DURATION);
    SpriteAnimation_addFrame(a, SpriteSheet_getSprite(attackSheet, 1), ATTACK_FRAME_DURATION);
    SpriteAnimation_start(a);
    SpriteAnimation_doLoop(a, FALSE);
    data->animation = a;

    GameObject* attack = newObject(data, deleteAttackData, OBJECT_ATTACK, posX, posY);

    Drawable* d = Drawable_newFromSpriteAnimationAndSize(a, 32, 32);
    Drawable_setZ(d, 1000);
    GameObject_addDrawableAt(attack, d, -16, -16);

    Body* b = Body_newRectangle(32, 32);
    Body_addTag(b, OBJECT_ATTACK);
    GameObject_addBodyAt(attack, b, -16, -16);

    return attack;
}

void setAttackHasHitObject(GameObject* attack, GameObject* other)
{
    LinkedList_addFirst(toAttack(attack)->hitObjects, other);
}

BOOL attackHasHitObject(GameObject* attack, GameObject* other)
{
    return LinkedList_elementExists(toAttack(attack)->hitObjects, other);
}
