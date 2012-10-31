/**
 * @file spriteanimation.h
 * @brief Defines the SpriteAnimation type and related functions.
 * @if doc_files
 * @ingroup smug_graphics
 * @endif
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_SPRITEANIMATION_H
#define SMUG_GRAPHICS_SPRITEANIMATION_H

#include <common.h>
#include <graphics/sprite.h>

struct _SpriteAnimation;
typedef struct _SpriteAnimation SpriteAnimation;

typedef void (*SpriteAnimationStopCallback)(SpriteAnimation* animation, void* anything);

void SpriteAnimation_initialize();
void SpriteAnimation_terminate();

SpriteAnimation* SpriteAnimation_newEmpty();

void SpriteAnimation_addFrame(SpriteAnimation* self, Sprite* sprite, TIME duration);

void SpriteAnimation_start(SpriteAnimation* self);
void SpriteAnimation_startAt(SpriteAnimation* self, TIME time);
void SpriteAnimation_doLoop(SpriteAnimation* self, BOOL loop);

void SpriteAnimation_pause(SpriteAnimation* self);
void SpriteAnimation_reset(SpriteAnimation* self);

void SpriteAnimation_setStopCallback(SpriteAnimation* self, SpriteAnimationStopCallback callback, void* anything);
void SpriteAnimation_callAnimationStopCallbacks();

Sprite* SpriteAnimation_getCurrentSprite(SpriteAnimation* self);
Sprite* SpriteAnimation_getSpriteAtIndex(SpriteAnimation* self, int index);

void SpriteAnimation_delete(SpriteAnimation* self);

#endif /* SMUG_GRAPHICS_SPRITEANIMATION_H */

/**@}*/
