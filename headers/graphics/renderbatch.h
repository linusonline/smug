#ifndef SMUG_GRAPHICS_RENDERBATCH_H
#define SMUG_GRAPHICS_RENDERBATCH_H

#include <common.h>

typedef struct RenderBatch
{
    unsigned int maxElements;
    unsigned int addedElements;
    float* vertexArray;
    float* colorArray;
    float* textureArray;
} RenderBatch;

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
void RenderBatch_render(RenderBatch* self);

#endif /* SMUG_GRAPHICS_RENDERBATCH_H */
