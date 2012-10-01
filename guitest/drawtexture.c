#include <GL/glfw.h>
#include <stdlib.h>
#include <assert.h>

#include <graphics/vertexarray.h>
#include <graphics/texcoordarray.h>
#include <graphics/texture.h>
#include <common.h>

static Image* image = NULL;
static Texture* texture = NULL;

void GLFWCALL windowResize(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, (GLdouble)height, 0.0);
}

void drawStuff()
{
    VertexArray_addRect(0, 0, 200, 200);
    TexCoordArray_addRect(0.0, 0.0, 1.0, 1.0);
}

void afterDrawing()
{
    TexCoordArray_clear();
    VertexArray_clear();
}

void init()
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
    TexCoordArray_init();

    image = Image_new();
    Image_loadFromFile(image, "res/box.png");
    texture = Texture_newFromImage(image);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_TEXTURE_2D);
}

void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        drawStuff();
        assert(VertexArray_getNumberOfAddedVertices() == TexCoordArray_getNumberOfAddedPairs());
        Texture_activate(texture);
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
    TexCoordArray_release();
    VertexArray_release();
    glfwTerminate();
}

int main()
{
    init();
    runMainLoop();
    deinit();
    return 0;
}
