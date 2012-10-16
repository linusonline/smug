#ifndef SMUG_GRAPHICS_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_H

#include <common.h>
#include <graphics/sprite.h>
#include <graphics/spriteanimation.h>

struct _Drawable;
typedef struct _Drawable Drawable;

Drawable* Drawable_newFromSprite(Sprite* sprite);
Drawable* Drawable_newFromSpriteAndDimensions(Sprite* sprite, float width, float height, float posX, float posY);
Drawable* Drawable_newFromSpriteAnimationAndDimensions(SpriteAnimation* sprite, float width, float height, float posX, float posY);
void Drawable_delete(Drawable* self);
void Drawable_deleteVoid(void* self);
void Drawable_setSize(Drawable* self, float width, float height);
void Drawable_setPos(Drawable* self, float x, float y);
float Drawable_getX(Drawable* self);
float Drawable_getY(Drawable* self);
Sprite* Drawable_getSprite(Drawable* self);
SpriteAnimation* Drawable_getSpriteAnimation(Drawable* self);
void Drawable_useSprite(Drawable* self, Sprite* sprite);
void Drawable_useSpriteAnimation(Drawable* self, SpriteAnimation* spriteAnimation);
void Drawable_addRenderData(Drawable* self, RenderBatch* renderBatch);

#endif /* SMUG_GRAPHICS_DRAWABLE_H */
