#include <common.h>
#include <utils/log.h>
#include <graphics/drawable.h>
#include <graphics/spritesheet.h>
#include <graphics/spriteanimation.h>

#include <avatar.h>

static Drawable* avatar = NULL;
static SpriteAnimation* walkLeft = NULL;
static SpriteAnimation* walkRight = NULL;
static SpriteAnimation* walkUp = NULL;
static SpriteAnimation* walkDown = NULL;
static SpriteSheet* avatarSheet = NULL;

static const float WALK_FRAME_DURATION = 0.1;
static BOOL walking = FALSE;

Drawable* getAvatar(float width, float height, float posX, float posY)
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

    avatar = Drawable_newFromSpriteAnimationAndDimensions(walkDown, width, height, posX, posY);
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
    Drawable_delete(avatar);
    SpriteAnimation_delete(walkLeft);
    SpriteAnimation_delete(walkRight);
    SpriteAnimation_delete(walkUp);
    SpriteAnimation_delete(walkDown);
    SpriteSheet_delete(avatarSheet);
}

void setAvatarLeft()
{
    Drawable_useSpriteAnimation(avatar, walkLeft);
}

void setAvatarRight()
{
    Drawable_useSpriteAnimation(avatar, walkRight);
}

void setAvatarUp()
{
    Drawable_useSpriteAnimation(avatar, walkUp);
}

void setAvatarDown()
{
    Drawable_useSpriteAnimation(avatar, walkDown);
}
