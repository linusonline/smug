#include <common.h>
#include <common.h>
#include <utils/log.h>
#include <graphics/drawable.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <graphics/spriteanimation.h>

#include <objects.h>
#include <avatar.h>

static GameObject* avatar = NULL;
static SpriteAnimation* walkLeft = NULL;
static SpriteAnimation* walkRight = NULL;
static SpriteAnimation* walkUp = NULL;
static SpriteAnimation* walkDown = NULL;
static SpriteSheet* avatarSheet = NULL;

static const float WALK_FRAME_DURATION = 0.1;
static BOOL walking = FALSE;

GameObject* getAvatar(float width, float height, float posX, float posY)
{
    avatarSheet = SpriteSheet_new("res/characters/rpg_sprite_walk.png", "res/characters/rpg_sprite_walk.txt");

    walkLeft = SpriteAnimation_newEmpty();
    walkRight = SpriteAnimation_newEmpty();
    walkUp = SpriteAnimation_newEmpty();
    walkDown = SpriteAnimation_newEmpty();

    for (int i = 0; i < 8; i++)
    {
        SpriteAnimation_addFrame(walkLeft, SpriteSheet_getSpriteXY(avatarSheet, i, 2), WALK_FRAME_DURATION);
    }
    for (int i = 0; i < 8; i++)
    {
        SpriteAnimation_addFrame(walkRight, SpriteSheet_getSpriteXY(avatarSheet, i, 3), WALK_FRAME_DURATION);
    }
    for (int i = 0; i < 8; i++)
    {
        SpriteAnimation_addFrame(walkUp, SpriteSheet_getSpriteXY(avatarSheet, i, 1), WALK_FRAME_DURATION);
    }
    for (int i = 0; i < 8; i++)
    {
        SpriteAnimation_addFrame(walkDown, SpriteSheet_getSpriteXY(avatarSheet, i, 0), WALK_FRAME_DURATION);
    }

    avatar = GameObject_new(posX, posY);
    GameObject_addDrawableAt(avatar, Drawable_newFromSpriteAnimationAndSize(walkDown, width, height), -12, -32);
    Body* b = Body_newRectangle(width, height);
    Body_addTag(b, OBJECT_AVATAR);
    GameObject_addBodyAt(avatar, b, -12, -32);
    return avatar;
}

void avatarWalk(BOOL walk)
{
    if (walk == walking)
    {
        return;
    }
    walking = walk;
    if (walking)
    {
        SpriteAnimation_start(walkLeft);
        SpriteAnimation_start(walkRight);
        SpriteAnimation_start(walkUp);
        SpriteAnimation_start(walkDown);
    }
    else
    {
        SpriteAnimation_pause(walkLeft);
        SpriteAnimation_reset(walkLeft);
        SpriteAnimation_pause(walkRight);
        SpriteAnimation_reset(walkRight);
        SpriteAnimation_pause(walkUp);
        SpriteAnimation_reset(walkUp);
        SpriteAnimation_pause(walkDown);
        SpriteAnimation_reset(walkDown);
    }
}

void deleteAvatar()
{
    GameObject_delete(avatar);
    SpriteAnimation_delete(walkLeft);
    SpriteAnimation_delete(walkRight);
    SpriteAnimation_delete(walkUp);
    SpriteAnimation_delete(walkDown);
    SpriteSheet_delete(avatarSheet);
}

void setAvatarLeft()
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(avatar), walkLeft);
}

void setAvatarRight()
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(avatar), walkRight);
}

void setAvatarUp()
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(avatar), walkUp);
}

void setAvatarDown()
{
    Drawable_useSpriteAnimation(GameObject_getDrawable(avatar), walkDown);
}
