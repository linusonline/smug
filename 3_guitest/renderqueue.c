#include <GL/glfw.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <graphics/renderqueue.h>
#include <graphics/image.h>
#include <graphics/texture.h>
#include <graphics/sprite.h>
#include <utils/log.h>
#include <utils/stdout_console.h>
#include <common.h>

static Texture* testTexture = NULL;
static Texture* spriteSheet = NULL;
static RenderQueue* renderQueue = NULL;

static Sprite* testSprite = NULL;
static Sprite* rightSprite = NULL;
static Sprite* downSprite = NULL;
static Sprite* upSprite = NULL;

static Drawable* test1 = NULL;
static Drawable* test2 = NULL;
static Drawable* rightCannon1 = NULL;
static Drawable* rightCannon2 = NULL;
static Drawable* rightCannon3 = NULL;
static Drawable* downCannon = NULL;
static Drawable* upCannon = NULL;

static Console* console = NULL;

static void GLFWCALL windowResize(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, (GLdouble)height, 0.0);
}

static void drawStuff(RenderQueue* rq)
{
    RenderQueue_addDrawable(renderQueue, test1, 16, 16);
    RenderQueue_addDrawable(renderQueue, test2, 32, 32);
    RenderQueue_addDrawable(renderQueue, rightCannon1, 64, 64);
    RenderQueue_addDrawable(renderQueue, rightCannon2, 96, 64);
    RenderQueue_addDrawable(renderQueue, rightCannon3, 128, 64);
    RenderQueue_addDrawable(renderQueue, downCannon, 160, 64);
    RenderQueue_addDrawable(renderQueue, upCannon, 192, 64);
}

static void afterDrawing()
{
    RenderQueue_clear(renderQueue);
}

static void init()
{
    console = StdoutConsole_new();
    smug_assert(console != NULL);
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

    renderQueue = RenderQueue_new();

    Image* testImage = Image_new();
    Image_loadFromFile(testImage, "5_res/test2.png");
    testTexture = Texture_newFromImage(testImage);
    Image_delete(testImage);
    testImage = NULL;

    testSprite = Sprite_newFromPixelCoords(testTexture, 0, 0, 8, 8);

    Image* spritesImage = Image_new();
    Image_loadFromFile(spritesImage, "5_res/units.png");
    spriteSheet = Texture_newFromImage(spritesImage);
    Image_delete(spritesImage);
    spritesImage = NULL;

    rightSprite = Sprite_newFromPixelCoords(spriteSheet, 0, 0, 16, 18);
    upSprite = Sprite_newFromPixelCoords(spriteSheet, 64, 0, 16, 18);
    downSprite = Sprite_newFromPixelCoords(spriteSheet, 32, 0, 16, 18);

    test1 = Drawable_newFromSprite(testSprite);
    Drawable_setSize(test1, 16, 16);

    test2 = Drawable_newFromSprite(testSprite);
    Drawable_setSize(test2, 32, 32);

    rightCannon1 = Drawable_newFromSprite(rightSprite);
    Drawable_setSize(rightCannon1, 16, 16);

    rightCannon2 = Drawable_newFromSprite(rightSprite);
    Drawable_setSize(rightCannon2, 16, 16);

    rightCannon3 = Drawable_newFromSprite(rightSprite);
    Drawable_setSize(rightCannon3, 16, 16);

    downCannon = Drawable_newFromSprite(downSprite);
    Drawable_setSize(downCannon, 16, 16);

    upCannon = Drawable_newFromSprite(upSprite);
    Drawable_setSize(upCannon, 16, 16);
}

static void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        // OpenGL rendering goes here...
        glClear(GL_COLOR_BUFFER_BIT);

        drawStuff(renderQueue);
        RenderQueue_render(renderQueue);

        // Swap front and back rendering buffers
        glfwSwapBuffers();

        afterDrawing();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
}

static void deinit()
{
    RenderQueue_delete(renderQueue);

    Texture_delete(testTexture);
    Texture_delete(spriteSheet);

    Drawable_delete(test1);
    Drawable_delete(test2);
    Drawable_delete(rightCannon1);
    Drawable_delete(rightCannon2);
    Drawable_delete(rightCannon3);
    Drawable_delete(downCannon);
    Drawable_delete(upCannon);

    Sprite_delete(testSprite);
    Sprite_delete(rightSprite);
    Sprite_delete(downSprite);
    Sprite_delete(upSprite);

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
