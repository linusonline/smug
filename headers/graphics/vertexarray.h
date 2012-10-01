#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glfw.h>

/* Requires openGL to be initialized. */

void VertexArray_init(void);
void VertexArray_addRect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
int VertexArray_getNumberOfAddedVertices(void);
void VertexArray_clear(void);
void VertexArray_release(void);

#ifdef GREY_BOX
GLfloat* getVertexArray();
#endif

#endif /* VERTEXARRAY_H */
