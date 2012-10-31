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
#include <engine/gameobject.h>

int Engine_init(void);
BOOL Engine_isInitialized(void);
void Engine_terminate(void);

void Engine_addObject(GameObject* newObj);
void Engine_addObjects(GameObject** objects, int offset, int numObjects);
void Engine_removeObject(GameObject* obj);
void Engine_removeAllObjects();
void Engine_removeAndDeleteAllObjects();

void Engine_drawObjects(RenderQueue* renderQueue);
void Engine_collideObjects();

void Engine_doForAllObjects(void (*function)(GameObject*));

#endif /* SMUG_ENGINE_ENGINE_H */

/**@}*/
