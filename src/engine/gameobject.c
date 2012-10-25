#include <common.h>
#include <graphics/drawable.h>
#include <utils/linkedlist.h>

#include <engine/gameobject.h>

static BOOL _invariant(GameObject* self)
{
    return self != NULL;
}

static float _getAbsolutePositionX(GameObject* self)
{
    if (self->parent == NULL)
    {
        return self->positionX;
    }
    else
    {
        return self->positionX + _getAbsolutePositionX(self->parent);
    }
}

static float _getAbsolutePositionY(GameObject* self)
{
    if (self->parent == NULL)
    {
        return self->positionY;
    }
    else
    {
        return self->positionY + _getAbsolutePositionY(self->parent);
    }
}

static void _updateBodyPosition(GameObject* self)
{
    if (self->body != NULL)
    {
        Body_setPosition(self->body, _getAbsolutePositionX(self) + self->bodyOffsetX, _getAbsolutePositionY(self) + self->bodyOffsetY);
    }
}

GameObject* GameObject_new(float posX, float posY)
{
    GameObject* newObject = allocate(GameObject);
    newObject->subObjects = LinkedList_new();
    newObject->parent = NULL;
    newObject->positionX = posX;
    newObject->positionY = posY;
    newObject->drawable = NULL;
    newObject->drawableOffsetX = 0.0f;
    newObject->drawableOffsetY = 0.0f;
    newObject->body = NULL;
    newObject->bodyOffsetX = 0.0f;
    newObject->bodyOffsetY = 0.0f;
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
    LinkedList_delete(self->subObjects);
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
            _getAbsolutePositionX(self) + self->drawableOffsetX,
            _getAbsolutePositionY(self) + self->drawableOffsetY);
    }
}

GameObject* GameObject_addBodyAt(GameObject* self, Body* body, float offsetX, float offsetY)
{
    if (self->body != NULL)
    {
        Body_delete(self->body);
    }
    self->body = body;
    self->bodyOffsetX = offsetX;
    self->bodyOffsetY = offsetY;
    _updateBodyPosition(self);
}

Body* GameObject_getBody(GameObject* self)
{
    return self->body;
}

void GameObject_setBodyOffset(GameObject* self, float offsetX, float offsetY)
{
    smug_assert(self->body != NULL);
    self->bodyOffsetX = offsetX;
    self->bodyOffsetY = offsetY;
    _updateBodyPosition(self);
}

BOOL GameObject_bodyHasTag(GameObject* self, int tag)
{
    if (self->body != NULL)
    {
        return Body_hasTag(self->body, tag);
    }
    else
    {
        return FALSE;
    }
}

void GameObject_setPos(GameObject* self, float x, float y)
{
    smug_assert(_invariant(self));
    self->positionX = x;
    self->positionY = y;
    _updateBodyPosition(self);
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

void GameObject_addObject(GameObject* self, GameObject* newChild)
{
    smug_assert(newChild->parent == NULL);
    LinkedList_addLast(self->subObjects, (void*)newChild);
    newChild->parent = self;
    _updateBodyPosition(newChild);
}

void GameObject_removeObject(GameObject* self, GameObject* child)
{
    LinkedList_removeItem(self->subObjects, (void*)child);
}

void GameObject_setUserData(GameObject* self, void* data)
{
    self->userData = data;
}

void* GameObject_getUserData(GameObject* self)
{
    return self->userData;
}
