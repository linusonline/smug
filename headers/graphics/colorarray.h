#ifndef COLORARRAY_H
#define COLORARRAY_H

#include <GL/glfw.h>

void ColorArray_init(void);
void ColorArray_addColor(GLfloat r, GLfloat g, GLfloat b);
void ColorArray_addUnicolorRect(GLfloat r, GLfloat g, GLfloat b);
int ColorArray_getNumberOfAddedColors(void);
void ColorArray_clear(void);
void ColorArray_release(void);

#ifdef GREY_BOX
GLfloat* getColorArray();
#endif

#endif /* COLORARRAY_H */
