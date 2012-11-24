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

struct _SheetProxy;
typedef struct _SheetProxy SpriteSheet;

SpriteSheet* SpriteSheet_new(const char* imageFile, const char* dataFile);
SpriteSheet* SpriteSheet_newUnloaded(const char* imageFile, const char* dataFile);
BOOL SpriteSheet_isValid(SpriteSheet* self);
void SpriteSheet_reload(SpriteSheet* self);

Sprite* SpriteSheet_getSprite(SpriteSheet* self, int index);
Sprite* SpriteSheet_getSpriteXY(SpriteSheet* self, int xIndex, int yIndex);

int SpriteSheet_getNrOfSprites(SpriteSheet* self);

void SpriteSheet_delete(SpriteSheet* self);

#endif /* SMUG_GRAPHICS_SPRITESHEET_H */

/**@}*/
