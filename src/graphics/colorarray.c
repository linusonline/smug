#include <graphics/colorarray.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static const int COLOR_ARRAY_INITIAL_SIZE = 30;

static GLfloat* colorArray = NULL;
static int colorArrayMaxSize = 0;
static int colorArraySize = 0;
static int addedColors = 0;

#define allocate(type) ((type*)malloc(sizeof(type)))
#define allocatev(type, size) ((type*)malloc(sizeof(type) * size))

static BOOL _invariant()
{
    return  colorArray != 0 &&
            colorArrayMaxSize >= COLOR_ARRAY_INITIAL_SIZE &&
            colorArrayMaxSize >= colorArraySize &&
            addedColors * 2 == colorArraySize;
}

static void setGlArray()
{
    glColorPointer(3, GL_FLOAT, 0, colorArray);
}

static void expandColorArrayIfNeeded(int neededSpace)
{
    if (colorArrayMaxSize - colorArraySize < neededSpace)
    {
        GLfloat* newColorArray = allocatev(GLfloat, colorArrayMaxSize * 2);
        memcpy(newColorArray, colorArray, sizeof(GLfloat) * colorArraySize);
        free(colorArray);
        colorArray = newColorArray;
        colorArrayMaxSize *= 2;
        setGlArray();
    }
}

void ColorArray_init()
{
    if (colorArray != NULL)
    {
        ColorArray_release();
    }
    colorArray = allocatev(GLfloat, COLOR_ARRAY_INITIAL_SIZE);
    colorArrayMaxSize = COLOR_ARRAY_INITIAL_SIZE;
    assert(_invariant);
    glEnableClientState(GL_COLOR_ARRAY);
    setGlArray();
}

void ColorArray_addColor(GLfloat r, GLfloat g, GLfloat b)
{
    expandColorArrayIfNeeded(3);
    colorArray[colorArraySize++] = r;
    colorArray[colorArraySize++] = g;
    colorArray[colorArraySize++] = b;
    addedColors++;
    assert(_invariant);
}

void ColorArray_addUnicolorRect(GLfloat r, GLfloat g, GLfloat b)
{
    ColorArray_addColor(r, g, b);
    ColorArray_addColor(r, g, b);
    ColorArray_addColor(r, g, b);
    ColorArray_addColor(r, g, b);
    assert(_invariant);
}

int ColorArray_getNumberOfAddedColors()
{
    assert(_invariant);
    return addedColors;
}

void ColorArray_clear()
{
    colorArraySize = 0;
    addedColors = 0;
    assert(_invariant);
}

void ColorArray_release()
{
    ColorArray_clear();
    glDisableClientState(GL_COLOR_ARRAY);
    free(colorArray);
    colorArray = NULL;
    colorArrayMaxSize = 0;
}

#ifdef GREY_BOX
GLfloat* getColorArray()
{
    return colorArray;
}
#endif
