#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <graphics/texcoordarray.h>
#include <common.h>

static const int TEXCOORD_ARRAY_INITIAL_SIZE = 32;

static GLfloat* texCoordArray = NULL;
static int texCoordArrayMaxSize = 0;
static int texCoordArraySize = 0;
static int addedPairs = 0;

static BOOL _invariant()
{
    return  texCoordArray != 0 &&
            texCoordArrayMaxSize >= TEXCOORD_ARRAY_INITIAL_SIZE &&
            texCoordArrayMaxSize >= texCoordArraySize &&
            addedPairs * 2 == texCoordArraySize;
}

static void setGlArray()
{
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordArray);
}

static void expandTexCoordArrayIfNeeded(int neededSpace)
{
    if (texCoordArrayMaxSize - texCoordArraySize < neededSpace)
    {
        GLfloat* newTexCoordArray = allocatev(GLfloat, texCoordArrayMaxSize * 2);
        memcpy(newTexCoordArray, texCoordArray, sizeof(GLfloat) * texCoordArraySize);
        free(texCoordArray);
        texCoordArray = newTexCoordArray;
        texCoordArrayMaxSize *= 2;
        setGlArray();
    }
}

static void addPairToArray(GLfloat x, GLfloat y)
{
    expandTexCoordArrayIfNeeded(2);
    texCoordArray[texCoordArraySize++] = x;
    texCoordArray[texCoordArraySize++] = y;
    addedPairs++;
}

void TexCoordArray_init()
{
    if (texCoordArray != NULL)
    {
        TexCoordArray_release();
    }
    texCoordArray = allocatev(GLfloat, TEXCOORD_ARRAY_INITIAL_SIZE);
    texCoordArrayMaxSize = TEXCOORD_ARRAY_INITIAL_SIZE;
    assert(_invariant);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    setGlArray();
}

void TexCoordArray_addRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    addPairToArray(x1, y1);
    addPairToArray(x1, y2);
    addPairToArray(x2, y2);
    addPairToArray(x2, y1);
    assert(_invariant);
}

int TexCoordArray_getNumberOfAddedPairs()
{
    assert(_invariant);
    return addedPairs;
}

void TexCoordArray_clear()
{
    texCoordArraySize = 0;
    addedPairs = 0;
    assert(_invariant);
}

void TexCoordArray_release()
{
    TexCoordArray_clear();
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    free(texCoordArray);
    texCoordArray = NULL;
    texCoordArrayMaxSize = 0;
}

#ifdef GREY_BOX
GLfloat* getTexCoordArray()
{
    return texCoordArray;
}
#endif
