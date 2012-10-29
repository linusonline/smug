/**
 * @file camera.h
 * @brief Defines the Camera type and related functions.
 * @if doc_files
 * @ingroup smug_graphics
 * @endif
 */

/**
 * @addtogroup smug_graphics
 * @{
 */

#ifndef SMUG_GRAPHICS_CAMERA_H
#define SMUG_GRAPHICS_CAMERA_H

#include <common.h>

/**
 * A versatile camera for viewing the world.
 */
typedef struct Camera
{
    float posX, posY;
    float rotation;
    float scaleX, scaleY;
} Camera;

/**
 * Create new Camera
 * @relates Camera
 */
Camera* Camera_new();

/**
 * Delete Camera
 * @relates Camera
 */
void Camera_delete(void* camera);

#endif /* SMUG_GRAPHICS_CAMERA_H */

/**@}*/
