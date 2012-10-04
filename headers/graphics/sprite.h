/**
 * @file sprite.h
 * @brief Defines the Sprite type and related functions.
 * @if doc_files
 * @ingroup smug_graphics
 * @endif
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_SPRITE_H
#define SMUG_GRAPHICS_SPRITE_H

#include <common.h>
#include <graphics/texture.h>
#include <graphics/renderbatch.h>

#include <graphics/sprite_type.h>

/**
 * Create new sprite from pixel coordinates.
 * @param texture a pointer to a valid texture
 * @param x X coordinate of the sprite in pixels
 * @param y Y coordinate of the sprite in pixels
 * @param w W of the sprite in pixels
 * @param h H of the sprite in pixels
 * @relates Sprite
 */
Sprite* Sprite_newFromPixelCoords(Texture* texture, int x, int y, int w, int h);

void Sprite_addRenderData(Sprite* self, RenderBatch* renderBatch, float posX, float posY, float width, float height);

/**
 * Return the texture the sprite draws its graphic from.
 * @relates Sprite
 */
Texture* Sprite_getTexture(Sprite* self);

/**
 * Return the ID of the texture the sprite draws its graphic from.
 * @relates Sprite
 */
unsigned int Sprite_getTextureId(Sprite* self);

/**
 * Delete sprite
 * @param sprite a pointer to a sprite
 * @relates Sprite
 */
void Sprite_delete(void* sprite);

#endif /* SMUG_GRAPHICS_SPRITE_H */

/**@}*/
