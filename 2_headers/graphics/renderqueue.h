#ifndef SMUG_GRAPHICS_RENDERQUEUE_H
#define SMUG_GRAPHICS_RENDERQUEUE_H

#include <common.h>
#include <graphics/renderbatch.h>
#include <graphics/drawable.h>

struct _RenderQueue;
typedef struct _RenderQueue RenderQueue;

RenderQueue* RenderQueue_new(void);
void RenderQueue_delete(RenderQueue* self);
void RenderQueue_clear(RenderQueue* self);
void RenderQueue_addDrawable(RenderQueue* self, Drawable* drawable, float positionX, float positionY);

/** Requires OpenGL to be initialized, and GL_VERTEX_ARRAY and
 *  GL_TEXTURE_COORD_ARRAY to be enabled.
 */
void RenderQueue_render(RenderQueue* self);

#ifdef GREY_BOX
#include <utils/map.h>
Map* RenderQueue_getBatches(RenderQueue* self);
#endif /* GREY_BOX */

#endif /* SMUG_GRAPHICS_RENDERQUEUE_H */
