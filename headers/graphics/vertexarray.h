#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glfw.h>

void initVertexArray(void);
void addRectToVertexArray(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2);
int getNumberOfAddedVertices(void);
void releaseVertexArray(void);

#endif /* VERTEXARRAY_H */
