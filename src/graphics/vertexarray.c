#include <graphics/vertexarray.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static const int VERTEX_ARRAY_INITIAL_SIZE = 32;

static GLfloat* vertexArray = NULL;
static int vertexArrayMaxSize = 0;
static int vertexArraySize = 0;
static int addedVertices = 0;

#define allocate(type) ((type*)malloc(sizeof(type)))
#define allocatev(type, size) ((type*)malloc(sizeof(type) * size))

static BOOL _invariant()
{
    return  vertexArray != 0 &&
            vertexArrayMaxSize >= VERTEX_ARRAY_INITIAL_SIZE &&
            vertexArrayMaxSize >= vertexArraySize &&
            addedVertices * 2 == vertexArraySize;
}

static void setGlArray()
{
    glVertexPointer(2, GL_FLOAT, 0, vertexArray);
}

static void expandVertexArrayIfNeeded(int neededSpace)
{
    if (vertexArrayMaxSize - vertexArraySize < neededSpace)
    {
        GLfloat* newVertexArray = allocatev(GLfloat, vertexArrayMaxSize * 2);
        memcpy(newVertexArray, vertexArray, sizeof(GLfloat) * vertexArraySize);
        free(vertexArray);
        vertexArray = newVertexArray;
        vertexArrayMaxSize *= 2;
        setGlArray();
    }
}

static void addVertexToArray(GLfloat x, GLfloat y)
{
    expandVertexArrayIfNeeded(2);
    vertexArray[vertexArraySize++] = x;
    vertexArray[vertexArraySize++] = y;
    addedVertices++;
}

void VertexArray_init()
{
    if (vertexArray != NULL)
    {
        VertexArray_release();
    }
    vertexArray = allocatev(GLfloat, VERTEX_ARRAY_INITIAL_SIZE);
    vertexArrayMaxSize = VERTEX_ARRAY_INITIAL_SIZE;
    assert(_invariant);
    glEnableClientState(GL_VERTEX_ARRAY);
    setGlArray();
}

void VertexArray_addRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    addVertexToArray(x1, y1);
    addVertexToArray(x1, y2);
    addVertexToArray(x2, y2);
    addVertexToArray(x2, y1);
    assert(_invariant);
}

int VertexArray_getNumberOfAddedVertices()
{
    assert(_invariant);
    return addedVertices;
}

void VertexArray_clear()
{
    vertexArraySize = 0;
    addedVertices = 0;
    assert(_invariant);
}

void VertexArray_release()
{
    VertexArray_clear();
    glDisableClientState(GL_VERTEX_ARRAY);
    free(vertexArray);
    vertexArray = NULL;
    vertexArrayMaxSize = 0;
}

#ifdef GREY_BOX
GLfloat* getVertexArray()
{
    return vertexArray;
}
#endif
