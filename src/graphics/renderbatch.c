#include <stdlib.h>
#include <string.h>

#include <common.h>
// #include <utils/log.h>
#include <GL/glfw.h>

#include <graphics/renderbatch.h>

static const unsigned char PRIMITIVES_PER_VERTEX = 2;
static const unsigned char PRIMITIVES_PER_TEXTURE_COORDINATE = 2;
static const unsigned char PRIMITIVES_PER_COLOR = 4;

static BOOL _invariant(RenderBatch* self)
{
    return
        self->addedElements >= 0 &&
        self->addedElements <= self->maxElements &&
        self->vertexArray != NULL &&
        self->textureArray != NULL ? self->colorArray == NULL : self->colorArray != NULL;
}

static BOOL _usesTexture(RenderBatch* self)
{
    return self->textureArray != NULL;
}

static void _expandArray(GLfloat** array, int primitivesPerElement, int newCapacity, int copyElements)
{
    GLfloat* newArray = allocatev(GLfloat, newCapacity * primitivesPerElement);
    memcpy(newArray,
           *array,
           sizeof(GLfloat) * copyElements * primitivesPerElement);
    free(*array);
    *array = newArray;
}

static void _expandArraysIfNeeded(RenderBatch* self, int neededSpace)
{
    int newCapacity = self->maxElements;
    while (newCapacity - self->addedElements < neededSpace)
    {
        newCapacity *= 2;
    }
    if (newCapacity == self->maxElements)
    {
        return;
    }

    _expandArray(&self->vertexArray, PRIMITIVES_PER_VERTEX, newCapacity, self->addedElements);
    if (_usesTexture(self))
    {
        _expandArray(&self->textureArray, PRIMITIVES_PER_TEXTURE_COORDINATE, newCapacity, self->addedElements);
    }
    else
    {
        _expandArray(&self->colorArray, PRIMITIVES_PER_COLOR, newCapacity, self->addedElements);
    }
    self->maxElements = newCapacity;
}

RenderBatch* RenderBatch_new(unsigned int initialSize, BOOL useTexture)
{
    smug_assert(initialSize > 0);
    RenderBatch* self = allocate(RenderBatch);
    self->vertexArray = allocatev(GLfloat, initialSize * PRIMITIVES_PER_VERTEX);
    if (useTexture)
    {
        self->textureArray = allocatev(GLfloat, initialSize * PRIMITIVES_PER_TEXTURE_COORDINATE);
        self->colorArray = NULL;
    }
    else
    {
        self->colorArray = allocatev(GLfloat, initialSize * PRIMITIVES_PER_COLOR);
        self->textureArray = NULL;
    }
    self->maxElements = initialSize;
    self->addedElements = 0;
    // setSize(ret, initialSize);
    return self;
}

void RenderBatch_delete(RenderBatch* self)
{
    free(self->vertexArray);
    if (_usesTexture(self))
    {
        free(self->textureArray);
    }
    else
    {
        free(self->colorArray);
    }
    free(self);
}

void RenderBatch_clear(RenderBatch* self)
{
    self->addedElements = 0;
}

int RenderBatch_getNumberOfAddedElements(RenderBatch* self)
{
    smug_assert(_invariant(self));
    return self->addedElements;
}

void RenderBatch_addTexturedRect(RenderBatch* self,
                                 float x1, float y1, float x2, float y2,
                                 float s1, float t1, float s2, float t2)
{
    smug_assert(_usesTexture(self));
    _expandArraysIfNeeded(self, 4);
    int start = self->addedElements * PRIMITIVES_PER_VERTEX;
    self->vertexArray[start + 0] = x1;
    self->vertexArray[start + 1] = y1;
    self->vertexArray[start + 2] = x1;
    self->vertexArray[start + 3] = y2;
    self->vertexArray[start + 4] = x2;
    self->vertexArray[start + 5] = y2;
    self->vertexArray[start + 6] = x2;
    self->vertexArray[start + 7] = y1;
    start = self->addedElements * PRIMITIVES_PER_TEXTURE_COORDINATE;
    self->textureArray[start + 0] = s1;
    self->textureArray[start + 1] = t1;
    self->textureArray[start + 2] = s1;
    self->textureArray[start + 3] = t2;
    self->textureArray[start + 4] = s2;
    self->textureArray[start + 5] = t2;
    self->textureArray[start + 6] = s2;
    self->textureArray[start + 7] = t1;
    self->addedElements += 4;
}

void RenderBatch_addColoredRect(RenderBatch* self,
                                float x1, float y1, float x2, float y2,
                                float r, float g, float b, float a)
{
    smug_assert(!_usesTexture(self));
    _expandArraysIfNeeded(self, 4);
    int start = self->addedElements * PRIMITIVES_PER_VERTEX;
    self->vertexArray[start + 0] = x1;
    self->vertexArray[start + 1] = y1;
    self->vertexArray[start + 2] = x1;
    self->vertexArray[start + 3] = y2;
    self->vertexArray[start + 4] = x2;
    self->vertexArray[start + 5] = y2;
    self->vertexArray[start + 6] = x2;
    self->vertexArray[start + 7] = y1;
    start = self->addedElements * PRIMITIVES_PER_COLOR;
    for (int offset = start; offset < start + 16;)
    {
        self->colorArray[offset++] = r;
        self->colorArray[offset++] = g;
        self->colorArray[offset++] = b;
        self->colorArray[offset++] = a;
    }
    self->addedElements += 4;
}

void RenderBatch_render(RenderBatch* self)
{
    glVertexPointer(PRIMITIVES_PER_VERTEX, GL_FLOAT, 0, self->vertexArray);
    if (_usesTexture(self))
    {
        glTexCoordPointer(PRIMITIVES_PER_TEXTURE_COORDINATE, GL_FLOAT, 0, self->textureArray);
    }
    else
    {
        glColorPointer(PRIMITIVES_PER_COLOR, GL_FLOAT, 0, self->colorArray);
    }
    glDrawArrays(GL_QUADS, 0, self->addedElements);
}

#ifdef GREY_BOX
float* RenderBatch_getVertexArray(RenderBatch* self)
{
    return self->vertexArray;
}

float* RenderBatch_getColorArray(RenderBatch* self)
{
    return self->colorArray;
}

float* RenderBatch_getTexCoordArray(RenderBatch* self)
{
    return self->textureArray;
}
#endif

