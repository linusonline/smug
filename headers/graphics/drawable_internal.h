#ifndef SMUG_GRAPHICS_DRAWABLE_INTERNAL_H
#define SMUG_GRAPHICS_DRAWABLE_INTERNAL_H

#include <graphics/spriteanimation.h>
#include <graphics/drawable.h>

typedef struct _Drawable
{
    float width;
    float height;
    float z;
    SpriteAnimation* sprite;
    BOOL createdLocally;
} _Drawable;

#endif /* SMUG_GRAPHICS_DRAWABLE_INTERNAL_H */
