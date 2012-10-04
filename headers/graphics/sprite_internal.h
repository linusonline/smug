#ifndef SMUG_GRAPHICS_SPRITE_INTERNAL_H
#define SMUG_GRAPHICS_SPRITE_INTERNAL_H

#include <utils/rectangle.h>
#include <graphics/texture_type.h>

/**
 * Holds a texture and a rectangular area on it.
 * Also supports an offset to that area.
 */
typedef struct _Sprite
{
    Rectangle rect;
    Texture* texture;
} _Sprite;

#endif /* SMUG_GRAPHICS_SPRITE_INTERNAL_H */
