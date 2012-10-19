#include <common.h>
#include <graphics/drawable.h>
#include <engine/gameobject.h>

BOOL _invariant(GameObject* self)
{
    return self != NULL;
}

GameObject* GameObject_new(float posX, float posY)
{
    GameObject* newObject = allocate(GameObject);
    newObject->positionX = posX;
    newObject->positionY = posY;
    newObject->drawable = NULL;
    newObject->drawableOffsetX = 0.0f;
    newObject->drawableOffsetY = 0.0f;
    return newObject;
}

GameObject* GameObject_newWithDrawable(float posX, float posY, Drawable* drawable, float offsetX, float offsetY)
{
    GameObject* newObject = GameObject_new(posX, posY);
    GameObject_addDrawableAt(newObject, drawable, offsetX, offsetY);
    return newObject;
}

void GameObject_delete(GameObject* self)
{
    if (self->drawable != NULL)
    {
        Drawable_delete(self->drawable);
    }
    free(self);
}

void GameObject_deleteVoid(void* self)
{
    GameObject_delete((GameObject*)self);
}

GameObject* GameObject_addDrawableAt(GameObject* self, Drawable* drawable, float offsetX, float offsetY)
{
    if (self->drawable != NULL)
    {
        Drawable_delete(self->drawable);
    }
    self->drawable = drawable;
    self->drawableOffsetX = offsetX;
    self->drawableOffsetY = offsetY;
}

Drawable* GameObject_getDrawable(GameObject* self)
{
    return self->drawable;
}

void GameObject_setDrawableOffset(GameObject* self, float offsetX, float offsetY)
{
    smug_assert(self->drawable != NULL);
    self->drawableOffsetX = offsetX;
    self->drawableOffsetY = offsetY;
}

void GameObject_draw(GameObject* self, RenderQueue* renderQueue)
{
    if (self->drawable != NULL)
    {
        RenderQueue_addDrawable(renderQueue,
            self->drawable,
            self->positionX + self->drawableOffsetX,
            self->positionY + self->drawableOffsetY);
    }
}

void GameObject_setPos(GameObject* self, float x, float y)
{
    smug_assert(_invariant(self));
    self->positionX = x;
    self->positionY = y;
}

float GameObject_getX(GameObject* self)
{
    smug_assert(_invariant(self));
    return self->positionX;
}

float GameObject_getY(GameObject* self)
{
    smug_assert(_invariant(self));
    return self->positionY;
}
