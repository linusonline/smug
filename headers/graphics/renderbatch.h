#ifndef SMUG_GRAPHICS_RENDERBATCH_H
#define SMUG_GRAPHICS_RENDERBATCH_H

#include <common.h>

struct _RenderBatch;
typedef struct _RenderBatch RenderBatch;

RenderBatch* RenderBatch_new(unsigned int initialSize, BOOL useTexture);
void RenderBatch_delete(RenderBatch* self);
void RenderBatch_clear(RenderBatch* self);

int RenderBatch_getNumberOfAddedElements(RenderBatch* self);
void RenderBatch_addTexturedRect(RenderBatch* self,
                                 float x1, float y1, float x2, float y2,
                                 float s1, float t1, float s2, float t2);
void RenderBatch_addColoredRect(RenderBatch* self,
                                float x1, float y1, float x2, float y2,
                                float r, float g, float b, float a);

/** Requires OpenGL to be initialized, and GL_VERTEX_ARRAY and
 *  GL_TEXTURE_COORD_ARRAY to be enabled.
 */
void RenderBatch_render(RenderBatch* self);

#ifdef GREY_BOX
float* RenderBatch_getVertexArray(RenderBatch* self);
float* RenderBatch_getColorArray(RenderBatch* self);
float* RenderBatch_getTexCoordArray(RenderBatch* self);
#endif

#endif /* SMUG_GRAPHICS_RENDERBATCH_H */
