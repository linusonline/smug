#include <graphics/sprite.h>

#include <graphics/drawable.h>
#include <graphics/drawable_internal.h>

static BOOL _invariant(Drawable* self)
{
    return self != NULL && self->sprite != NULL;
}

Drawable* Drawable_newFromSprite(Sprite* sprite)
{
    Drawable* newDrawable = allocate(Drawable);
    newDrawable->sprite = sprite;
    newDrawable->positionX = 0;
    newDrawable->positionY = 0;
    // TODO: Default to sprite pixel size?
    newDrawable->width = 0;
    newDrawable->height = 0;
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

Sprite* Drawable_getSprite(Drawable* self)
{
    smug_assert(_invariant(self));
    return self->sprite;
}

void Drawable_addRenderData(Drawable* self, RenderBatch* renderBatch)
{
    smug_assert(_invariant(self));
    Sprite_addRenderData(self->sprite, renderBatch, self->positionX, self->positionY, self->width, self->height);
}
