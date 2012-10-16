/**
 * @file engine.h
 * @brief Header file for engine module
 * @if doc_files
 * @ingroup smug_engine
 * @endif
 */

/**
 * @defgroup smug_engine Engine
 *
 * @{
 */

#ifndef SMUG_ENGINE_ENGINE_H
#define SMUG_ENGINE_ENGINE_H

#include <common.h>
#include <graphics/drawable.h>

int Engine_init(void);
BOOL Engine_isInitialized(void);
void Engine_terminate(void);

void Engine_addObject(Drawable* newObj);
void Engine_addObjects(Drawable** objects, int offset, int numObjects);
void Engine_removeObject(Drawable* obj);
void Engine_removeAllObjects();

void Engine_drawObjects(RenderQueue* renderQueue);

#endif /* SMUG_ENGINE_ENGINE_H */

/**@}*/
