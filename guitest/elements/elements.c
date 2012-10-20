#include <GL/glfw.h>

#include <common.h>
#include <graphics/camera.h>
#include <graphics/graphics.h>
#include <graphics/drawable.h>
#include <input/input.h>
#include <utils/log.h>
#include <utils/stdout_console.h>
#include <utils/linkedlist.h>
#include <engine/engine.h>
#include <engine/mainloop.h>
#include <engine/collisiondetector.h>

#include <maps.h>
#include <avatar.h>
#include <monster.h>
#include <attack.h>
#include <objects.h>

static const int INITIAL_WINDOW_WIDTH = 640;
static const int INITIAL_WINDOW_HEIGHT = 480;
static int windowWidth = 640;
static int windowHeight = 480;

static GameObject** world = NULL;
static GameObject* avatar;

static Console* console = NULL;

static Controller* theController = NULL;

static Camera* camera;

static Monster monsters[13];

#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3
#define BUTTON_EXIT 4
#define BUTTON_ATTACK 5

static const int WORLD_WIDTH = 640;     // This world happens to be just one screen big.
static const int WORLD_HEIGHT = 480;

static int moveHorizontally = 0;
static int moveVertically = 0;

static float avatarSpeed = 100; // Units per second.

static int avatarFacing = BUTTON_DOWN;

static LinkedList* objectsToDelete;

static void setAllLeft()
{
    setAvatarLeft();
    avatarFacing = BUTTON_LEFT;
    for (int i = 0; i < 13; i++)
    {
        setMonsterLeft(monsters[i]);
    }
}

static void setAllRight()
{
    avatarFacing = BUTTON_RIGHT;
    setAvatarRight();
    for (int i = 0; i < 13; i++)
    {
        setMonsterRight(monsters[i]);
    }
}

static void setAllUp()
{
    avatarFacing = BUTTON_UP;
    setAvatarUp();
    for (int i = 0; i < 13; i++)
    {
        setMonsterUp(monsters[i]);
    }
}

static void setAllDown()
{
    avatarFacing = BUTTON_DOWN;
    setAvatarDown();
    for (int i = 0; i < 13; i++)
    {
        setMonsterDown(monsters[i]);
    }
}

static void alignAvatar()
{
    if (moveHorizontally != 0)
    {
        moveHorizontally < 0 ? setAllLeft() : setAllRight();
    }
    else if (moveVertically != 0)
    {
        moveVertically < 0 ? setAllUp() : setAllDown();
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

static void _attackEndCallback(SpriteAnimation* attack, void* callbackData)
{
    LinkedList_addLast(objectsToDelete, callbackData);
}

static void attack()
{
    float offsetX;
    float offsetY;
    switch (avatarFacing)
    {
        case BUTTON_LEFT:
            offsetX = -28;
            offsetY = -16;
            break;
        case BUTTON_RIGHT:
            offsetX = 28;
            offsetY = -16;
            break;
        case BUTTON_UP:
            offsetX = 0;
            offsetY = -48;
            break;
        case BUTTON_DOWN:
            offsetX = 0;
            offsetY = 16;
            break;
    }
    GameObject* attack = createAttack(GameObject_getX(avatar) + offsetX, GameObject_getY(avatar) + offsetY);
    SpriteAnimation_setStopCallback(Drawable_getSpriteAnimation(GameObject_getDrawable(attack)), _attackEndCallback, attack);
    Engine_addObject(attack);
}

static void deleteOldObjects()
{
    GameObject* go = (GameObject*)LinkedList_getFirst(objectsToDelete);
    while (go != NULL)
    {
        Engine_removeObject(go);
        deleteAttack(go);
        go = (GameObject*)LinkedList_getNext(objectsToDelete);
    }
    LinkedList_removeAll(objectsToDelete);
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
        case BUTTON_ATTACK:
            if (state == SMUG_KEY_PRESS)
            {
                attack();
            }
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
    float speedFraction = avatarSpeed / Mainloop_getLogicFps();
    GameObject_setPos(avatar,
        moveHorizontally * speedFraction + GameObject_getX(avatar),
        moveVertically * speedFraction + GameObject_getY(avatar));
    Drawable_setZ(GameObject_getDrawable(avatar), GameObject_getY(avatar));

    deleteOldObjects();
}

static void _collisionCallback(GameObject* obj1, GameObject* obj2)
{
    if (obj1 == avatar || obj2 == avatar)
    {
        GameObject* other = obj1 == avatar ? obj2 : obj1;
        if (Body_hasTag(GameObject_getBody(other), OBJECT_MONSTER))
        {
            LOG(LOG_USER1, "Hit by monster!");
        }
    }
    if (Body_hasTag(GameObject_getBody(obj1), OBJECT_ATTACK) ||
        Body_hasTag(GameObject_getBody(obj2), OBJECT_ATTACK))
    {
        GameObject* other = Body_hasTag(GameObject_getBody(obj1), OBJECT_ATTACK) ? obj2 : obj1;
        if (Body_hasTag(GameObject_getBody(other), OBJECT_MONSTER))
        {
            LOG(LOG_USER1, "Hit monster with attack!");
        }
    }
}

static void init()
{
    console = StdoutConsole_new();
    smug_assert(console != NULL);
    Log_init(console);
    Log_activateScopes(LOG_USER1);

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
    Input_linkControllerToKeyboardKey(theController, BUTTON_ATTACK, GLFW_KEY_SPACE);

    Mainloop_setLogicCallback(_logicCallback);

    world = createMap1();
    avatar = getAvatar(24, 32, 320, 240);
    Engine_init();
    Engine_addObjects(world, 0, map1Size());
    Engine_addObject(avatar);

    monsters[0] = newMonster(MONSTER_SHELLY, 32, 32);
    monsters[1] = newMonster(MONSTER_SHROOM, 32, 128);
    monsters[2] = newMonster(MONSTER_MINKEY, 32, 256);
    monsters[3] = newMonster(MONSTER_GOLEM, 128, 32);
    monsters[4] = newMonster(MONSTER_GOLEM, 128, 128);
    monsters[5] = newMonster(MONSTER_SNELL, 128, 256);
    monsters[6] = newMonster(MONSTER_TROLLEY, 256, 32);
    monsters[7] = newMonster(MONSTER_SKELETON, 256, 128);
    monsters[8] = newMonster(MONSTER_FIRESKULL, 256, 256);
    monsters[9] = newMonster(MONSTER_FIRESKULL, 32, 384);
    monsters[10] = newMonster(MONSTER_FIRESKULL, 128, 384);
    monsters[11] = newMonster(MONSTER_FIRESKULL, 256, 384);
    monsters[12] = newMonster(MONSTER_BEE, 384, 32);

    for (int i = 0; i < 13; i++)
    {
        Engine_addObject(monsters[i].monsterObject);
    }

    objectsToDelete = LinkedList_new();
    CollisionDetector_collideTags(0, 0, _collisionCallback);
}

static void deinit()
{
    deleteMap1();
    deleteAvatar();
    for (int i = 0; i < 12; i++)
    {
        deleteMonster(monsters[i]);
    }

    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_UP);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_DOWN);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_LEFT);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_RIGHT);
    Input_unlinkControllersFromKeyboardKey(GLFW_KEY_ESC);
    Controller_delete(theController);

    deleteOldObjects();
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
