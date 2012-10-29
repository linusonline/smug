#include <graphics/sprite.h>

#include <utils/log.h>
#include <graphics/drawable.h>
#include <graphics/drawable_internal.h>

static BOOL _invariant(Drawable* self)
{
    return self != NULL && (self->sprite != NULL || self->createdLocally == FALSE);
}

static Drawable* _new(float width, float height)
{
    Drawable* newDrawable = allocate(Drawable);
    newDrawable->width = width;
    newDrawable->height = height;
    newDrawable->z = 0.0f;
    newDrawable->sprite = NULL;
    newDrawable->createdLocally = FALSE;
    newDrawable->r = 0.0f;
    newDrawable->g = 0.0f;
    newDrawable->b = 0.0f;
    newDrawable->a = 0.0f;
    return newDrawable;
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
    return Drawable_newFromSpriteAndSize(sprite, 0, 0);
}

Drawable* Drawable_newFromSpriteAnimationAndSize(SpriteAnimation* sprite, float width, float height)
{
    Drawable* newDrawable = _new(width, height);
    newDrawable->sprite = sprite;
    smug_assert(_invariant(newDrawable));
    return newDrawable;
}

Drawable* Drawable_newFromSpriteAndSize(Sprite* sprite, float width, float height)
{
    SpriteAnimation* sa = _animateSprite(sprite);
    Drawable* newDrawable = Drawable_newFromSpriteAnimationAndSize(sa, width, height);
    newDrawable->createdLocally = TRUE;
    return newDrawable;
}

Drawable* Drawable_newFromColorAndSize(float r, float g, float b, float a, float width, float height)
{
    Drawable* newDrawable = _new(width, height);
    newDrawable->r = r;
    newDrawable->g = g;
    newDrawable->b = b;
    newDrawable->a = a;
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

void Drawable_setZ(Drawable* self, float z)
{
    smug_assert(_invariant(self));
    self->z = z;
}

float Drawable_getZ(Drawable* self)
{
    smug_assert(_invariant(self));
    return self->z;
}

SpriteAnimation* Drawable_getSpriteAnimation(Drawable* self)
{
    smug_assert(_invariant(self));
    return self->sprite;
}

Sprite* Drawable_getSprite(Drawable* self)
{
    smug_assert(_invariant(self));
    return self->sprite != NULL ? SpriteAnimation_getCurrentSprite(self->sprite) : NULL;
}

void Drawable_addRenderData(Drawable* self, RenderBatch* renderBatch, float positionX, float positionY)
{
    smug_assert(_invariant(self));
    if (self->sprite != NULL)
    {
        Sprite_addRenderData(SpriteAnimation_getCurrentSprite(self->sprite), renderBatch, positionX, positionY, self->width, self->height, self->z);
    }
    else
    {
        RenderBatch_addColoredRect(renderBatch, positionX, positionY, positionX + self->width, positionY + self->height, self->z,
                                   self->r, self->g, self->b, self->a);
    }
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

void Drawable_useColor(Drawable* self, float r, float g, float b, float a)
{
    if (self->createdLocally)
    {
        SpriteAnimation_delete(self->sprite);
    }
    self->sprite = NULL;
    self->r = r;
    self->g = g;
    self->b = b;
    self->a = a;
}
