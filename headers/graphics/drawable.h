#ifndef SMUG_GRAPHICS_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_H

#include <common.h>
#include <graphics/sprite.h>
#include <graphics/spriteanimation.h>

struct _Drawable;
typedef struct _Drawable Drawable;

Drawable* Drawable_newFromSprite(Sprite* sprite);
Drawable* Drawable_newFromSpriteAndSize(Sprite* sprite, float width, float height);
Drawable* Drawable_newFromSpriteAnimationAndSize(SpriteAnimation* sprite, float width, float height);
void Drawable_delete(Drawable* self);
void Drawable_deleteVoid(void* self);
void Drawable_setSize(Drawable* self, float width, float height);
void Drawable_setZ(Drawable* self, float z);
float Drawable_getZ(Drawable* self);
Sprite* Drawable_getSprite(Drawable* self);
SpriteAnimation* Drawable_getSpriteAnimation(Drawable* self);
void Drawable_useSprite(Drawable* self, Sprite* sprite);
void Drawable_useSpriteAnimation(Drawable* self, SpriteAnimation* spriteAnimation);
void Drawable_addRenderData(Drawable* self, RenderBatch* renderBatch, float positionX, float positionY);

#endif /* SMUG_GRAPHICS_DRAWABLE_H */
