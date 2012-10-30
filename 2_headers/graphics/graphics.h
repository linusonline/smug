#ifndef SMUG_GRAPHICS_GRAPHICS_H
#define SMUG_GRAPHICS_GRAPHICS_H

#include <common.h>
#include <graphics/renderqueue.h>
#include <graphics/camera.h>

void Graphics_initialize(int windowWidth, int windowHeight, BOOL fullscreen);
void Graphics_terminate();
BOOL Graphics_isInitialized();

void Graphics_setBackgroundColor(float red, float green, float blue);
void Graphics_setCoordinateSystemForWindow(float left, float top, float width, float height);
Camera* Graphics_getCamera(void);
void Graphics_setWindowResizeCallback(void (*callback)(int w, int h));

void Graphics_render(RenderQueue* renderQueue);
void Graphics_refreshWindow();

#endif /* SMUG_GRAPHICS_GRAPHICS_H */

/**@}*/
