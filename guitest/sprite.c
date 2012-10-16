#include <stdlib.h>
#include <assert.h>
#include <GL/glfw.h>

#include <common.h>

#include <graphics/renderbatch.h>
#include <graphics/texture.h>
#include <graphics/sprite.h>

#include <utils/log.h>
#include <utils/stdout_console.h>

static Image* spritesImage = NULL;
static Texture* spriteSheet = NULL;
static RenderBatch* spritesRenderBatch = NULL;
static Sprite* rightSprite = NULL;
static Sprite* upSprite = NULL;
static Sprite* downSprite = NULL;

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
    Sprite_addRenderData(rightSprite, spritesRenderBatch, 40, 150, 160, 180);
    Sprite_addRenderData(downSprite, spritesRenderBatch, 240, 150, 160, 180);
    Sprite_addRenderData(upSprite, spritesRenderBatch, 440, 150, 160, 180);
}

static void afterDrawing()
{
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

    spritesRenderBatch = RenderBatch_new(4, TRUE);

    spritesImage = Image_new();
    Image_loadFromFile(spritesImage, "res/units.png");
    spriteSheet = Texture_newFromImage(spritesImage);
    Image_delete(spritesImage);
    spritesImage = NULL;

    rightSprite = Sprite_newFromPixelCoords(spriteSheet, 0, 0, 16, 18);
    upSprite = Sprite_newFromPixelCoords(spriteSheet, 64, 0, 16, 18);
    downSprite = Sprite_newFromPixelCoords(spriteSheet, 32, 0, 16, 18);
}

static void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

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
    RenderBatch_delete(spritesRenderBatch);
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
