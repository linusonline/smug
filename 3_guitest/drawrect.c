#include <GL/glfw.h>
#include <stdlib.h>
#include <assert.h>

#include <graphics/renderbatch.h>
#include <common.h>

void GLFWCALL windowResize(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, (GLdouble)height, 0.0);
}

void drawStuff(RenderBatch* renderBatch)
{
    RenderBatch_addColoredRect(renderBatch, 0, 0, 100, 100, 0, 0, 0.5, 0, 1.0);
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
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

}

void runMainLoop()
{
    RenderBatch* renderBatch = RenderBatch_new(4, FALSE);
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        drawStuff(renderBatch);
        RenderBatch_render(renderBatch, 0);

        // Swap front and back rendering buffers
        glfwSwapBuffers();

        RenderBatch_clear(renderBatch);

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
    RenderBatch_delete(renderBatch);
}

void deinit()
{
    glfwTerminate();
}

int main()
{
    initGlWindow();
    runMainLoop();
    deinit();
    return 0;
}
