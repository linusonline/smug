#ifndef SMUG_GRAPHICS_DRAWABLE_INTERNAL_H
#define SMUG_GRAPHICS_DRAWABLE_INTERNAL_H

#include <graphics/sprite_type.h>
#include <graphics/drawable_type.h>

typedef struct _Drawable
{
    float positionX;
    float positionY;
    float width;
    float height;
    Sprite* sprite;
} _Drawable;

#endif /* SMUG_GRAPHICS_DRAWABLE_INTERNAL_H */
