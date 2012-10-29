#include <stdlib.h>
#include <GL/glfw.h>

#include <common.h>
#include <utils/log.h>
#include <utils/map.h>
#include <input/controller.h>

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

static Map* keyboardBindings = NULL;        // keyboard enum -> ControllerIndex
static Map* mouseButtonBindings = NULL;     // mouse button enum -> ControllerIndex
static ControllerIndex* mousePositionPointerBinding = NULL;
static ControllerIndex* mouseWheelPointerBinding = NULL;

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
        ButtonCallback bc = Controller_getButtonCallback(ci->controller);
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
    PointerCallback pi = Controller_getPointerCallback(mousePositionPointerBinding->controller);
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

void Input_initialize()
{
    keyboardBindings = Map_new(Map_compareInts);
    mouseButtonBindings = Map_new(Map_compareInts);
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
    _deleteBindings(keyboardBindings);
    Map_delete(keyboardBindings);
    _deleteBindings(mouseButtonBindings);
    Map_delete(mouseButtonBindings);
}

void Input_linkControllerToKeyboardKey(Controller* controller, int buttonIndex, int keyid)
{
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
