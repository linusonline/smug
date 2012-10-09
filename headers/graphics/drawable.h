#ifndef SMUG_GRAPHICS_DRAWABLE_H
#define SMUG_GRAPHICS_DRAWABLE_H

#include <common.h>
#include <graphics/sprite_type.h>
#include <graphics/drawable_type.h>
#include <graphics/spriteanimation.h>

Drawable* Drawable_newFromSprite(Sprite* sprite);
Drawable* Drawable_newFromSpriteAndDimensions(Sprite* sprite, int width, int height, int posX, int posY);
Drawable* Drawable_newFromSpriteAnimationAndDimensions(SpriteAnimation* sprite, int width, int height, int posX, int posY);
void Drawable_delete(Drawable* self);
void Drawable_setSize(Drawable* self, float width, float height);
void Drawable_setPos(Drawable* self, float x, float y);
Sprite* Drawable_getSprite(Drawable* self);
SpriteAnimation* Drawable_getSpriteAnimation(Drawable* self);
void Drawable_addRenderData(Drawable* self, RenderBatch* renderBatch);

#endif /* SMUG_GRAPHICS_DRAWABLE_H */
