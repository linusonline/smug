#include <GL/glfw.h>
#include <stdlib.h>
#include <graphics/vertexarray.h>

void GLFWCALL windowResize(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
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
    initVertexArray();
}



void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap front and back rendering buffers
        glfwSwapBuffers();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
}

int main()
{
    initGlWindow();
    runMainLoop();
    glfwTerminate();
    return 0;
}

// void drawStuff()
// {
    // glVertexPointer(2, GL_FLOAT, 0, vertexData);
    // // if (use_texture)
    // // {
        // // glTexCoordPointer( 2, GL_FLOAT, 0, currentBatch->data->textureData );
    // // }
    // glColorPointer(4, GL_FLOAT, 0, colorData);

    // glDrawArrays(GL_QUADS, 0, batchSize);
// }
