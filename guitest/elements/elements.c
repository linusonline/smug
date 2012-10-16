#include <GL/glfw.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <graphics/renderqueue.h>
#include <graphics/spriteanimation.h>
#include <graphics/camera.h>
#include <graphics/graphics.h>
#include <input/input.h>
#include <utils/log.h>
#include <utils/stdout_console.h>
#include <common.h>
#include <graphics/drawable.h>

#include <maps.h>

static const int INITIAL_WINDOW_WIDTH = 640;
static const int INITIAL_WINDOW_HEIGHT = 480;
static int windowWidth = 640;
static int windowHeight = 480;

static RenderQueue* renderQueue = NULL;

static Drawable** world = NULL;
static Drawable* character;

static SpriteAnimation* avatarAnim;

static Console* console = NULL;

static Controller* theController = NULL;

static Camera* camera;

#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3

static const int WORLD_WIDTH = 640;     // This world happens to be just one screen big.
static const int WORLD_HEIGHT = 480;

static void drawStuff(RenderQueue* rq)
{
    for (int i = 0; i < map1Size(); i++)
    {
        RenderQueue_addDrawable(rq, world[i]);
    }
}

static void afterDrawing()
{
    RenderQueue_clear(renderQueue);
}

static void createBackground()
{
    world = createMap1();
}

static void createAvatar(int posX, int posY)
{
}

static int moveHorizontally = 0;
static int moveVertically = 0;

static void _buttonCallback(Controller* controller, int buttonIndex, int state)
{
    smug_assert(controller == theController);
    int reverse = 1;
    if (state == SMUG_KEY_RELEASE)
    {
        reverse = -1;
    }
    switch (buttonIndex)
    {
        case BUTTON_UP:
            moveVertically += -1 * reverse;
            break;
        case BUTTON_DOWN:
            moveVertically += 1 * reverse;
            break;
        case BUTTON_LEFT:
            moveHorizontally += -1 * reverse;
            break;
        case BUTTON_RIGHT:
            moveHorizontally += 1 * reverse;
            break;
        default:
            smug_assert(FALSE);
    }
}

static void _resizeCallback(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

static void doStuff()
{
}

static void init()
{
    console = StdoutConsole_new();
    smug_assert(console != NULL);
    Log_init(console);
    Log_setLevel(LOG_ALL);

    camera = Camera_new();
    camera->posX = INITIAL_WINDOW_WIDTH / 2;
    camera->posY = INITIAL_WINDOW_HEIGHT / 2;

    Graphics_initialize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, FALSE);
    Graphics_setCoordinateSystemForWindow(-WORLD_WIDTH/2, -WORLD_HEIGHT/2, WORLD_WIDTH, WORLD_HEIGHT);
    Graphics_setBackgroundColor(0.5, 0.0, 0.5);
    Graphics_useCamera(camera);
    Graphics_setWindowResizeCallback(_resizeCallback);

    Input_initialize();
    theController = Controller_new(0, 10, 0);
    Controller_setButtonCallback(theController, _buttonCallback);
    Input_linkControllerToKeyboardKey(theController, BUTTON_UP, GLFW_KEY_UP);
    Input_linkControllerToKeyboardKey(theController, BUTTON_DOWN, GLFW_KEY_DOWN);
    Input_linkControllerToKeyboardKey(theController, BUTTON_LEFT, GLFW_KEY_LEFT);
    Input_linkControllerToKeyboardKey(theController, BUTTON_RIGHT, GLFW_KEY_RIGHT);

    renderQueue = RenderQueue_new();

    createBackground();
    createAvatar(320, 240);
}

static void runMainLoop()
{
    BOOL running = TRUE;
    while (running)
    {
        doStuff();
        drawStuff(renderQueue);
        Graphics_render(renderQueue);

        // Swap front and back rendering buffers
        Graphics_refreshWindow();

        afterDrawing();

        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }
}

static void deinit()
{
    RenderQueue_delete(renderQueue);

    deleteMap1();

    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_UP);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_DOWN);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_LEFT);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_RIGHT);
    Controller_delete(theController);

    Graphics_terminate();
    Input_terminate();
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
