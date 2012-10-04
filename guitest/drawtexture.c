#include <GL/glfw.h>
#include <stdlib.h>
#include <assert.h>

#include <graphics/renderbatch.h>
#include <graphics/texture.h>
#include <utils/log.h>
#include <utils/stdout_console.h>
#include <common.h>

static Image* image = NULL;
static Texture* texture = NULL;
static RenderBatch* renderBatch = NULL;

static Console* console = NULL;

void GLFWCALL windowResize(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, (GLdouble)height, 0.0);
}

void drawStuff()
{
    RenderBatch_addTexturedRect(renderBatch, 0, 0, 200, 200, 0.0, 0.0, 1.0, 1.0);
    RenderBatch_addTexturedRect(renderBatch, 200, 100, 300, 200, 1.0, 0.0, 0.0, 1.0);
}

void afterDrawing()
{
    RenderBatch_clear(renderBatch);
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

    renderBatch = RenderBatch_new(4, TRUE);

    image = Image_new();
    Image_loadFromFile(image, "res/box.png");
    texture = Texture_newFromImage(image);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    console = StdoutConsole_new();
    Log_init(console);
    Log_setLevel(LOG_ALL);
}

void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        drawStuff();
        Texture_activate(texture);
        RenderBatch_render(renderBatch);

        // Swap front and back rendering buffers
        glfwSwapBuffers();

        afterDrawing();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
}

void deinit()
{
    RenderBatch_delete(renderBatch);
    glfwTerminate();
    Log_terminate();
    StdoutConsole_delete(console);
}

int main()
{
    init();
    runMainLoop();
    deinit();
    return 0;
}
