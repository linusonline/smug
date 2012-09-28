#include <GL/glfw.h>
#include <stdlib.h>
#include <assert.h>
#include <graphics/vertexarray.h>
#include <graphics/colorarray.h>

void GLFWCALL windowResize(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, (GLdouble)height, 0.0);
}

void drawStuff()
{
    VertexArray_addRect(0, 0, 100, 100);
    ColorArray_addUnicolorRect(0, 0.5, 0);
}

void afterDrawing()
{
    ColorArray_clear();
    VertexArray_clear();
}

void initGlWindow()
{
    // int glfwInit( void )
    if (glfwInit() != GL_TRUE)
    {
        exit(EXIT_FAILURE);
    }

    // int glfwOpenWindow( int width, int height,
    //      int redbits, int greenbits, int bluebits,
    //      int alphabits, int depthbits, int stencilbits,
    //      int mode )
    if (glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW) != GL_TRUE)
    {
        // void glfwTerminate( void )
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowSizeCallback(windowResize);
    glClearColor(0.5, 0.0, 0.5, 0.0);
    VertexArray_init();
    ColorArray_init();
}

void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        drawStuff();
        assert(VertexArray_getNumberOfAddedVertices() == ColorArray_getNumberOfAddedColors());
        glDrawArrays(GL_QUADS, 0, VertexArray_getNumberOfAddedVertices());

        // Swap front and back rendering buffers
        glfwSwapBuffers();

        afterDrawing();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
}

void deinit()
{
    ColorArray_release();
    VertexArray_release();
    glfwTerminate();
}

int main()
{
    initGlWindow();
    runMainLoop();
    deinit();
    return 0;
}
