#ifndef SMUG_ENGINE_GAMEOBJECT_H
#define SMUG_ENGINE_GAMEOBJECT_H

#include <common.h>
#include <engine/body.h>
#include <graphics/drawable.h>
#include <graphics/renderqueue.h>
#include <utils/linkedlist.h>

struct _GameObject;
typedef struct _GameObject GameObject;

typedef struct _GameObject
{
    LinkedList* subObjects;
    GameObject* parent;
    void* userData;
    float positionX;    // If object has parent, this position is relative to the parent.
    float positionY;
    Drawable* drawable;
    float drawableOffsetX;
    float drawableOffsetY;
    Body* body;
    float bodyOffsetX;
    float bodyOffsetY;
} _GameObject;

GameObject* GameObject_new(float posX, float posY);
GameObject* GameObject_newWithDrawable(float posX, float posY, Drawable* drawable, float offsetX, float offsetY);
void GameObject_delete(GameObject* self);
void GameObject_deleteVoid(void* self);

void GameObject_addDrawableAt(GameObject* self, Drawable* drawable, float offsetX, float offsetY);
Drawable* GameObject_getDrawable(GameObject* self);
void GameObject_setDrawableOffset(GameObject* self, float offsetX, float offsetY);
Drawable* GameObject_removeDrawable(GameObject* self);
void GameObject_draw(GameObject* self, RenderQueue* renderQueue);

void GameObject_addBodyAt(GameObject* self, Body* body, float offsetX, float offsetY);
Body* GameObject_getBody(GameObject* self);
void GameObject_setBodyOffset(GameObject* self, float offsetX, float offsetY);
Body* GameObject_removeBody(GameObject* self);
BOOL GameObject_bodyHasTag(GameObject* self, int tag);

void GameObject_setPos(GameObject* self, float x, float y);
float GameObject_getX(GameObject* self);
float GameObject_getY(GameObject* self);

void GameObject_addObject(GameObject* self, GameObject* newChild);
void GameObject_removeObject(GameObject* self, GameObject* child);

void GameObject_setUserData(GameObject* self, void* data);
void* GameObject_getUserData(GameObject* self);

#endif /* SMUG_ENGINE_GAMEOBJECT_H */
