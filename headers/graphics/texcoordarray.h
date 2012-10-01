#ifndef TEXCOORDARRAY_H
#define TEXCOORDARRAY_H

#include <GL/glfw.h>

/* Requires openGL to be initialized. */

void TexCoordArray_init(void);
void TexCoordArray_addRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
int TexCoordArray_getNumberOfAddedPairs(void);
void TexCoordArray_clear(void);
void TexCoordArray_release(void);

#ifdef GREY_BOX
GLfloat* getTexCoordArray();
#endif

#endif /* TEXCOORDARRAY_H */
