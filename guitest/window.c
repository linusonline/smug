#include <GL/glfw.h>

int main()
{
    // int glfwInit( void )
    if (glfwInit() != GL_TRUE)
    {
        return 1;
    }

    // int glfwOpenWindow( int width, int height,
    //      int redbits, int greenbits, int bluebits,
    //      int alphabits, int depthbits, int stencilbits,
    //      int mode )
    if (glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW) != GL_TRUE)
    {
        // void glfwTerminate( void )
        glfwTerminate();
        return 2;
    }
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
    glfwTerminate();
    return 0;
}