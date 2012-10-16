#include <GL/glfw.h>

#include <common.h>
#include <graphics/camera.h>
#include <graphics/graphics.h>
#include <graphics/drawable.h>
#include <input/input.h>
#include <utils/log.h>
#include <utils/stdout_console.h>
#include <engine/engine.h>
#include <engine/mainloop.h>

#include <maps.h>
#include <avatar.h>

static const int INITIAL_WINDOW_WIDTH = 640;
static const int INITIAL_WINDOW_HEIGHT = 480;
static int windowWidth = 640;
static int windowHeight = 480;

static Drawable** world = NULL;
static Drawable* avatar;

static Console* console = NULL;

static Controller* theController = NULL;

static Camera* camera;

#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3
#define BUTTON_EXIT 4

static const int WORLD_WIDTH = 640;     // This world happens to be just one screen big.
static const int WORLD_HEIGHT = 480;

static int moveHorizontally = 0;
static int moveVertically = 0;

static void alignAvatar()
{
    if (moveHorizontally != 0)
    {
        moveHorizontally < 0 ? setAvatarLeft() : setAvatarRight();
    }
    else if (moveVertically != 0)
    {
        moveVertically < 0 ? setAvatarUp() : setAvatarDown();
    }
    if (moveHorizontally != 0 || moveVertically != 0)
    {
        avatarWalk(TRUE);
    }
    else
    {
        avatarWalk(FALSE);
    }
}

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
            alignAvatar();
            break;
        case BUTTON_DOWN:
            moveVertically += 1 * reverse;
            alignAvatar();
            break;
        case BUTTON_LEFT:
            moveHorizontally += -1 * reverse;
            alignAvatar();
            break;
        case BUTTON_RIGHT:
            moveHorizontally += 1 * reverse;
            alignAvatar();
            break;
        case BUTTON_EXIT:
            Mainloop_exit();
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

static void _logicCallback()
{
    if (!glfwGetWindowParam(GLFW_OPENED))
    {
        Mainloop_exit();
    }
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
    Input_linkControllerToKeyboardKey(theController, BUTTON_EXIT, GLFW_KEY_ESC);

    Mainloop_setLogicCallback(_logicCallback);

    world = createMap1();
    avatar = getAvatar(24, 32, 320, 240);
    Engine_init();
    Engine_addObjects(world, 0, map1Size());
    Engine_addObject(avatar);
}

static void deinit()
{
    deleteMap1();
    deleteAvatar();

    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_UP);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_DOWN);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_LEFT);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_RIGHT);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_ESC);
    Controller_delete(theController);

    Engine_removeAllObjects();
    Engine_terminate();
    Graphics_terminate();
    Input_terminate();
    Log_terminate();
    StdoutConsole_delete(console);
}

int main()
{
    init();
    Mainloop_run();
    deinit();
    return 0;
}
