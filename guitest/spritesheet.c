#include <GL/glfw.h>
#include <stdlib.h>
#include <assert.h>

#include <graphics/renderbatch.h>
#include <graphics/texture.h>
#include <utils/log.h>
#include <utils/stdout_console.h>
#include <common.h>

#define p2tx(pixel) Texture_pixelToTextureCoordX(spriteSheet, pixel)
#define p2ty(pixel) Texture_pixelToTextureCoordY(spriteSheet, pixel)

static Image* testImage = NULL;
static Image* spritesImage = NULL;
static Texture* testTexture = NULL;
static Texture* spriteSheet = NULL;
static RenderBatch* testRenderBatch = NULL;
static RenderBatch* spritesRenderBatch = NULL;

static Console* console = NULL;

static void GLFWCALL windowResize(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, (GLdouble)height, 0.0);
}

static void drawSprites(RenderBatch* rb)
{
    int spriteWidth = 16;
    int spriteHeight = 18;
    int scale = 7;
    RenderBatch_addTexturedRect(rb,
        0,   100,       spriteWidth * scale, 100 + spriteHeight * scale, 0.0,
        p2tx(16), p2ty(0.0), p2tx(32), p2ty(18));
    RenderBatch_addTexturedRect(rb,
        120, 100, 120 + spriteWidth * scale, 100 + spriteHeight * scale, 0.0,
        p2tx(64), p2ty(0.0), p2tx(80), p2ty(18));
    RenderBatch_addTexturedRect(rb,
        240, 100, 240 + spriteWidth * scale, 100 + spriteHeight * scale, 0.0,
        p2tx(0), p2ty(0.0), p2tx(16), p2ty(18));
    RenderBatch_addTexturedRect(rb,
        360, 100, 360 + spriteWidth * scale, 100 + spriteHeight * scale, 0.0,
        p2tx(32), p2ty(0.0), p2tx(48), p2ty(18));
}

static void drawTest(RenderBatch* rb)
{
    RenderBatch_addTexturedRect(rb, 0, 0, 100, 100, 0.0, 0.0, 0.0, 1.0, 1.0);
}

static void afterDrawing()
{
    RenderBatch_clear(testRenderBatch);
    RenderBatch_clear(spritesRenderBatch);
}

static void init()
{
    console = StdoutConsole_new();
    Log_init(console);

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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    testRenderBatch = RenderBatch_new(4, TRUE);
    spritesRenderBatch = RenderBatch_new(4, TRUE);

    testImage = Image_new();
    Image_loadFromFile(testImage, "res/test2.png");
    testTexture = Texture_newFromImage(testImage);
    Image_delete(testImage);
    testImage = NULL;

    spritesImage = Image_new();
    Image_loadFromFile(spritesImage, "res/units.png");
    spriteSheet = Texture_newFromImage(spritesImage);
    Image_delete(spritesImage);
    spritesImage = NULL;
}

static void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        drawTest(testRenderBatch);
        Texture_activate(testTexture);
        RenderBatch_render(testRenderBatch);

        drawSprites(spritesRenderBatch);
        Texture_activate(spriteSheet);
        RenderBatch_render(spritesRenderBatch);

        // Swap front and back rendering buffers
        glfwSwapBuffers();

        afterDrawing();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
}

static void deinit()
{
    RenderBatch_delete(testRenderBatch);
    RenderBatch_delete(spritesRenderBatch);
    Texture_delete(testTexture);
    Texture_delete(spriteSheet);
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
