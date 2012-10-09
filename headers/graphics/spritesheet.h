/**
 * @file spritesheet.h
 * @brief Defines the SpriteSheet type and related functions.
 * @if doc_files
 * @ingroup smug_graphics
 * @endif
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_SPRITESHEET_H
#define SMUG_GRAPHICS_SPRITESHEET_H

#include <common.h>
#include <graphics/sprite.h>

typedef struct SpriteSheet
{
    Sprite** sprites;
    int indexWidth;
    int nrSprites;
    Texture* texture;
} SpriteSheet;

SpriteSheet* SpriteSheet_new(char* imageFile, char* dataFile);

Sprite* SpriteSheet_getSprite(SpriteSheet* self, int index);
Sprite* SpriteSheet_getSpriteXY(SpriteSheet* self, int xIndex, int yIndex);

int SpriteSheet_getNrOfSprites(SpriteSheet* self);

void SpriteSheet_delete(SpriteSheet* self);

#endif /* SMUG_GRAPHICS_SPRITESHEET_H */

/**@}*/
