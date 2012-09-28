#include <GL/glfw.h>
#include <stdlib.h>

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
    if (glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_FULLSCREEN) != GL_TRUE)
    {
        // void glfwTerminate( void )
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glClearColor(0.5, 0.0, 0.5, 0.0);
}

void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap front and back rendering buffers. Implicitly calls glfwPollEvents to poll keys and other events.
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
