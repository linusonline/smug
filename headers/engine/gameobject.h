#ifndef SMUG_ENGINE_GAMEOBJECT_H
#define SMUG_ENGINE_GAMEOBJECT_H

#include <common.h>
#include <graphics/drawable.h>
#include <graphics/renderqueue.h>

struct _GameObject;
typedef struct _GameObject GameObject;

typedef struct _GameObject
{
    Drawable* drawable;
    float positionX;
    float positionY;
    float drawableOffsetX;
    float drawableOffsetY;
} _GameObject;

GameObject* GameObject_new(float posX, float posY);
GameObject* GameObject_newWithDrawable(float posX, float posY, Drawable* drawable, float offsetX, float offsetY);
void GameObject_delete(GameObject* self);
void GameObject_deleteVoid(void* self);

GameObject* GameObject_addDrawableAt(GameObject* self, Drawable* drawable, float offsetX, float offsetY);
Drawable* GameObject_getDrawable(GameObject* self);
void GameObject_setDrawableOffset(GameObject* self, float offsetX, float offsetY);
void GameObject_draw(GameObject* self, RenderQueue* renderQueue);

void GameObject_setPos(GameObject* self, float x, float y);
float GameObject_getX(GameObject* self);
float GameObject_getY(GameObject* self);

#endif /* SMUG_ENGINE_GAMEOBJECT_H */
