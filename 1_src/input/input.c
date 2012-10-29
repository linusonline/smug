#include <stdlib.h>
#include <GL/glfw.h>

#include <common.h>
#include <utils/log.h>
#include <utils/map.h>
#include <utils/linkedlist.h>
#include <input/controller.h>
#include <input/controllerscheme.h>

#include <input/input.h>

typedef struct ControllerIndex
{
    Controller* controller;
    int index;
} ControllerIndex;

static ControllerIndex* ControllerIndex_new(Controller* controller, int index)
{
    ControllerIndex* ci = allocate(ControllerIndex);
    ci->controller = controller;
    ci->index = index;
    return ci;
}

static void ControllerIndex_delete(ControllerIndex* self)
{
    free(self);
}

// These data represent a "binding set". TODO: Allow binding sets to be saved and loaded.
static Map* keyboardBindings = NULL;        // keyboard enum -> ControllerIndex
static Map* mouseButtonBindings = NULL;     // mouse button enum -> ControllerIndex
static ControllerIndex* mousePositionPointerBinding = NULL;
static ControllerIndex* mouseWheelPointerBinding = NULL;

static BOOL isInitialized = FALSE;

static LinkedList* controllerSchemeStack = NULL; // ControllerScheme

static ControllerScheme* _getDefaultScheme()
{
    return (ControllerScheme*)LinkedList_getLast(controllerSchemeStack);
}

static void _keyboardCallback(int keyid, int state)
{
    int userState;
    switch (state)
    {
        case GLFW_PRESS:
            userState = SMUG_KEY_PRESS;
            break;
        case GLFW_RELEASE:
            userState = SMUG_KEY_RELEASE;
            break;
        default:
            return;
    }
    ControllerIndex* ci = (ControllerIndex*)Map_get(keyboardBindings, &keyid);
    if (ci != NULL)
    {
        ButtonCallback bc = ControllerScheme_getButtonCallback(_getDefaultScheme(), ci->controller);
        if (bc == NULL)
        {
            ERROR("Key %i was mapped to button %i on controller %x, but no button callback was set for that controller!", keyid, ci->index, ci->controller);
            return;
        }
        bc(ci->controller, ci->index, userState);
    }
    else
    {
        LOG(LOG_INPUT, "%s on unmapped key %i", (userState == SMUG_KEY_PRESS ? "Press" : "Release"), keyid);
    }
}

static int oldMouseXPos = 0;
static int oldMouseYPos = 0;
static BOOL mousePositionSet = FALSE;
static void _mousePositionCallback(int xPos, int yPos)
{
    // Yes, absolute coordinates...
    smug_assert(mousePositionPointerBinding != NULL);
    PointerCallback pi = ControllerScheme_getPointerCallback(_getDefaultScheme(), mousePositionPointerBinding->controller);
    if (pi == NULL)
    {
        ERROR("Mouse position was mapped to pointer %i on controller %x, but no pointer callback was set for that controller!", mousePositionPointerBinding->index, mousePositionPointerBinding->controller);
        return;
    }
    if (mousePositionSet)
    {
        pi(mousePositionPointerBinding->controller, mousePositionPointerBinding->index, xPos - oldMouseXPos, yPos - oldMouseYPos);
    }
    else
    {
        pi(mousePositionPointerBinding->controller, mousePositionPointerBinding->index, 0, 0);
        mousePositionSet = TRUE;
    }
    oldMouseXPos = xPos;
    oldMouseYPos = yPos;
}

void Input_pushControllerScheme(ControllerScheme* newScheme)
{
    smug_assert(isInitialized);
    LinkedList_addLast(controllerSchemeStack, newScheme);
}

ControllerScheme* Input_popControllerScheme()
{
    smug_assert(isInitialized);
    // User is not allowed to pop the default scheme.
    smug_assert(LinkedList_length(controllerSchemeStack) > 1);
    return LinkedList_popLast(controllerSchemeStack);
}

void Input_initialize()
{
    smug_assert(!isInitialized);
    keyboardBindings = Map_new(Map_compareInts);
    mouseButtonBindings = Map_new(Map_compareInts);
    // Create default controller scheme.
    controllerSchemeStack = LinkedList_new();
    LinkedList_addLast(controllerSchemeStack, ControllerScheme_new());
    isInitialized = TRUE;
}

static void _deleteBindings(Map* bindings)
{
    MapIterator* iter = Map_getIterator(bindings);
    MapPair* mp;
    while (MapIterator_hasMore(iter))
    {
        mp = MapIterator_getNextPair(iter);
        Map_deleteInt((int*)mp->key);
        if (mp->value != NULL)
        {
            ControllerIndex_delete((ControllerIndex*)mp->value);
        }
    }
    MapIterator_delete(iter);
}

void Input_terminate()
{
    smug_assert(isInitialized);
    _deleteBindings(keyboardBindings);
    Map_delete(keyboardBindings);
    _deleteBindings(mouseButtonBindings);
    Map_delete(mouseButtonBindings);
    // Delete default controller scheme.
    ControllerScheme_delete(LinkedList_popFirst(controllerSchemeStack));
    LinkedList_delete(controllerSchemeStack);
    isInitialized = FALSE;
}

void Input_linkControllerToKeyboardKey(Controller* controller, int buttonIndex, int keyid)
{
    smug_assert(isInitialized);
    smug_assert(Controller_hasButton(controller, buttonIndex));
    ControllerIndex* ci = (ControllerIndex*)Map_get(keyboardBindings, &keyid);
    if (ci != NULL)
    {
        WARNING("Linking key ID %i to button %i on controller %x, but one or both of them were already mapped!", keyid, buttonIndex, controller);
        WARNING("Only one-to-one mappings allowed! Deleting old mapping.");
        ControllerIndex_delete(ci);
        // TODO: Small memory leak here, of the old integer key in the map.
    }
    LOG(LOG_INPUT, "Adding mapping for key ID %i to button %i on controller %x", keyid, buttonIndex, controller);
    Map_set(keyboardBindings, Map_newInt(keyid), ControllerIndex_new(controller, buttonIndex));
    glfwSetKeyCallback(_keyboardCallback);
}

void Input_unlinkControllersFromKeyboardKey(int keyid)
{
    smug_assert(isInitialized);
    ControllerIndex* ci = (ControllerIndex*)Map_get(keyboardBindings, &keyid);
    if (ci != NULL)
    {
        ControllerIndex_delete(ci);
        Map_set(keyboardBindings, Map_newInt(keyid), NULL);
        MapIterator* iter = Map_getIterator(keyboardBindings);
        while (MapIterator_hasMore(iter))
        {
            if (MapIterator_getNextValue(iter) != NULL)
            {
                MapIterator_delete(iter);
                return;
            }
        }
        MapIterator_delete(iter);
        glfwSetKeyCallback(NULL);
    }
    else
    {
        WARNING("Tried to unlink key ID %i, but it was not mapped to anything!", keyid);
    }
}

void Input_linkControllerToMousePosition(Controller* controller, int pointerIndex)
{
    smug_assert(isInitialized);
    smug_assert(Controller_hasPointer(controller, pointerIndex));
    if (mousePositionPointerBinding != NULL)
    {
        WARNING("Linking mouse position to pointer %i on controller %x, but one or both of them were already mapped!", pointerIndex, controller);
        WARNING("Only one-to-one mappings allowed! Deleting old mapping.");
        ControllerIndex_delete(mousePositionPointerBinding);
    }
    mousePositionPointerBinding = ControllerIndex_new(controller, pointerIndex);
    glfwSetMousePosCallback(_mousePositionCallback);
}

void Input_unlinkControllersFromMousePosition()
{
    smug_assert(isInitialized);
    if (mousePositionPointerBinding != NULL)
    {
        ControllerIndex_delete(mousePositionPointerBinding);
        mousePositionPointerBinding = NULL;
        glfwSetMousePosCallback(NULL);
    }
    else
    {
        WARNING("Tried to unlink mouse position, but it was not mapped to anything!");
    }
}

void Input_setButtonCallbackForController(Controller* c, ButtonCallback buttonCallback)
{
    ControllerScheme_setButtonCallbackForController(_getDefaultScheme(), c, buttonCallback);
}

ButtonCallback Input_getButtonCallback(Controller* c)
{
    return ControllerScheme_getButtonCallback(_getDefaultScheme(), c);
}

void Input_setPointerCallbackForController(Controller* c, PointerCallback pointerCallback)
{
    ControllerScheme_setPointerCallbackForController(_getDefaultScheme(), c, pointerCallback);
}

PointerCallback Input_getPointerCallback(Controller* c)
{
    return ControllerScheme_getPointerCallback(_getDefaultScheme(), c);
}

// void _mouseButtonCallback(int mouseButton, int state)
// {
    // GLFW_MOUSE_BUTTON_1 -> GLFW_MOUSE_BUTTON_8
    // GLFW_MOUSE_BUTTON_LEFT
    // GLFW_MOUSE_BUTTON_RIGHT
    // GLFW_MOUSE_BUTTON_MIDDLE

    // state => GLFW_PRESS, GLFW_RELEASE
// }
// void _mouseWheelCallback(int position)
// {
// }

// glfwSetMouseButtonCallback(_mouseButtonCallback)
// glfwSetMouseWheelCallback(_mouseWheelCallback)
