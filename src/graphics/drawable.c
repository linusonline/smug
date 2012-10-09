#include <graphics/sprite.h>

#include <graphics/drawable.h>
#include <graphics/drawable_internal.h>

static BOOL _invariant(Drawable* self)
{
    return self != NULL && self->sprite != NULL;
}

Drawable* Drawable_newFromSprite(Sprite* sprite)
{
    // TODO: Default to sprite pixel size?
    Drawable_newFromSpriteAndDimensions(sprite, 0, 0, 0, 0);
}

Drawable* Drawable_newFromSpriteAnimationAndDimensions(SpriteAnimation* sprite, int width, int height, int posX, int posY)
{
    Drawable* newDrawable = allocate(Drawable);
    newDrawable->sprite = sprite;
    newDrawable->positionX = posX;
    newDrawable->positionY = posY;
    newDrawable->width = width;
    newDrawable->height = height;
    smug_assert(_invariant(newDrawable));
    return newDrawable;
}

Drawable* Drawable_newFromSpriteAndDimensions(Sprite* sprite, int width, int height, int posX, int posY)
{
    Drawable* newDrawable = allocate(Drawable);
    newDrawable->sprite = SpriteAnimation_newEmpty();
    SpriteAnimation_addFrame(newDrawable->sprite, sprite, 1.0);
    SpriteAnimation_start(newDrawable->sprite);
    newDrawable->positionX = posX;
    newDrawable->positionY = posY;
    newDrawable->width = width;
    newDrawable->height = height;
    smug_assert(_invariant(newDrawable));
    return newDrawable;
}

void Drawable_delete(Drawable* self)
{
    free(self);
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
