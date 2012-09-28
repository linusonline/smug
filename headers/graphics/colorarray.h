#ifndef COLORARRAY_H
#define COLORARRAY_H

#include <GL/glfw.h>

void initColorArray(void);
void addColorToArray(GLfloat r, GLfloat g, GLfloat b);
void addUnicolorRectToColorArray(GLfloat r, GLfloat g, GLfloat b);
int getNumberOfAddedColors(void);
void clearColorArray(void);
void releaseColorArray(void);

#endif /* COLORARRAY_H */
