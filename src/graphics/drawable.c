#include <graphics/sprite.h>

#include <utils/log.h>
#include <graphics/drawable.h>
#include <graphics/drawable_internal.h>

static BOOL _invariant(Drawable* self)
{
    return self != NULL && self->sprite != NULL;
}

static SpriteAnimation* _animateSprite(Sprite* sprite)
{
    SpriteAnimation* sa = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(sa, sprite, 1.0);
    return sa;
}

Drawable* Drawable_newFromSprite(Sprite* sprite)
{
    // TODO: Default to sprite pixel size?
    return Drawable_newFromSpriteAndDimensions(sprite, 0, 0, 0, 0);
}

Drawable* Drawable_newFromSpriteAnimationAndDimensions(SpriteAnimation* sprite, float width, float height, float posX, float posY)
{
    Drawable* newDrawable = allocate(Drawable);
    newDrawable->sprite = sprite;
    newDrawable->positionX = posX;
    newDrawable->positionY = posY;
    newDrawable->width = width;
    newDrawable->height = height;
    newDrawable->createdLocally = FALSE;
    smug_assert(_invariant(newDrawable));
    return newDrawable;
}

Drawable* Drawable_newFromSpriteAndDimensions(Sprite* sprite, float width, float height, float posX, float posY)
{
    SpriteAnimation* sa = _animateSprite(sprite);
    Drawable* newDrawable = Drawable_newFromSpriteAnimationAndDimensions(sa, width, height, posX, posY);
    newDrawable->createdLocally = TRUE;
    return newDrawable;
}

void Drawable_delete(Drawable* self)
{
    if (self->createdLocally)
    {
        SpriteAnimation_delete(self->sprite);
    }
    free(self);
}

void Drawable_deleteVoid(void* self)
{
    Drawable_delete((Drawable*)self);
}

void Drawable_setSize(Drawable* self, float width, float height)
{
    smug_assert(_invariant(self));
    self->width = width;
    self->height = height;
}

void Drawable_setPos(Drawable* self, float x, float y)
{
    smug_assert(_invariant(self));
    self->positionX = x;
    self->positionY = y;
}

float Drawable_getX(Drawable* self)
{
    return self->positionX;
}

float Drawable_getY(Drawable* self)
{
    return self->positionY;
}

SpriteAnimation* Drawable_getSpriteAnimation(Drawable* self)
{
    smug_assert(_invariant(self));
    return self->sprite;
}

Sprite* Drawable_getSprite(Drawable* self)
{
    smug_assert(_invariant(self));
    return SpriteAnimation_getCurrentSprite(self->sprite);
}

void Drawable_addRenderData(Drawable* self, RenderBatch* renderBatch)
{
    smug_assert(_invariant(self));
    Sprite_addRenderData(SpriteAnimation_getCurrentSprite(self->sprite), renderBatch, self->positionX, self->positionY, self->width, self->height);
}

void Drawable_useSprite(Drawable* self, Sprite* sprite)
{
    if (self->createdLocally)
    {
        SpriteAnimation_delete(self->sprite);
    }
    self->sprite = _animateSprite(sprite);
    self->createdLocally = TRUE;
}

void Drawable_useSpriteAnimation(Drawable* self, SpriteAnimation* spriteAnimation)
{
    if (self->createdLocally)
    {
        SpriteAnimation_delete(self->sprite);
    }
    self->sprite = spriteAnimation;
    self->createdLocally = FALSE;
}
