#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <graphics/drawable.h>
#include <graphics/spriteanimation.h>

#include <objects.h>
#include <attack.h>

static SpriteSheet* attackSheet = NULL;
static const char* imageFile = "res/effects/cold.png";
static const char* dataFile = "res/effects/32x32.txt";

static const float ATTACK_FRAME_DURATION = 0.1;

typedef struct AttackData
{
    LinkedList* hitObjects;
} AttackData;

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

    Drawable* d = Drawable_newFromSpriteAnimationAndSize(a, 32, 32);
    Drawable_setZ(d, 1000);
    GameObject* attack = GameObject_new(posX, posY);
    GameObject_addDrawableAt(attack, d, -16, -16);
    Body* b = Body_newRectangle(32, 32);
    Body_addTag(b, OBJECT_ATTACK);
    GameObject_addBodyAt(attack, b, -16, -16);
    GameObject_setUserData(attack, (void*)data);
    return attack;
}

void deleteAttack(GameObject* self)
{
    SpriteAnimation* sa = Drawable_getSpriteAnimation(GameObject_getDrawable(self));
    GameObject_delete(self);
    SpriteAnimation_delete(sa);
}

void setAttackHasHitObject(GameObject* attack, GameObject* other)
{
    LinkedList_addFirst(((AttackData*)GameObject_getUserData(attack))->hitObjects, other);
}

BOOL attackHasHitObject(GameObject* attack, GameObject* other)
{
    return LinkedList_elementExists(((AttackData*)GameObject_getUserData(attack))->hitObjects, other);
}
