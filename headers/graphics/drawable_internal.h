#ifndef SMUG_GRAPHICS_DRAWABLE_INTERNAL_H
#define SMUG_GRAPHICS_DRAWABLE_INTERNAL_H

#include <graphics/spriteanimation.h>
#include <graphics/drawable.h>

typedef struct _Drawable
{
    float positionX;
    float positionY;
    float width;
    float height;
    SpriteAnimation* sprite;
} _Drawable;

#endif /* SMUG_GRAPHICS_DRAWABLE_INTERNAL_H */
