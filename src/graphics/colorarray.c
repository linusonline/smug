#include <graphics/colorarray.h>
#include <stdlib.h>
#include <string.h>

static const int COLOR_ARRAY_INITIAL_SIZE = 30;

static GLfloat* colorArray = NULL;
static int colorArrayMaxSize = 0;
static int colorArraySize = 0;
static int addedColors = 0;

#define allocate(type) ((type*)malloc(sizeof(type)))
#define allocatev(type, size) ((type*)malloc(sizeof(type) * size))

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

void initColorArray()
{
    colorArray = allocatev(GLfloat, COLOR_ARRAY_INITIAL_SIZE);
    colorArrayMaxSize = COLOR_ARRAY_INITIAL_SIZE;
    setGlArray();
}

void addColorToArray(GLfloat r, GLfloat g, GLfloat b)
{
    expandColorArrayIfNeeded(3);
    colorArray[colorArraySize++] = r;
    colorArray[colorArraySize++] = g;
    colorArray[colorArraySize++] = b;
    addedColors++;
}

void addUnicolorRectToColorArray(GLfloat r, GLfloat g, GLfloat b)
{
    addColorToArray(r, g, b);
    addColorToArray(r, g, b);
    addColorToArray(r, g, b);
    addColorToArray(r, g, b);
}

int getNumberOfAddedColors()
{
    return addedColors;
}

void clearColorArray()
{
    colorArraySize = 0;
    addedColors = 0;
}

void releaseColorArray()
{
    free(colorArray);
}
