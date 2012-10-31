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
    if (ci == NULL)
    {
        LOG(LOG_INPUT, "%s on unmapped key %i", (userState == SMUG_KEY_PRESS ? "Press" : "Release"), keyid);
        return;
    }

    Controller_setButtonPressed(ci->controller, ci->index, userState == SMUG_KEY_PRESS);

    ControllerScheme* scheme = (ControllerScheme*)LinkedList_getFirst(controllerSchemeStack);
    for (;;)
    {
        ButtonCallback callback = ControllerScheme_getButtonCallback(scheme, ci->controller);
        if (callback == NULL)
        {
            ERROR("Key %i was mapped to button %i on controller %x, but no button callback was set for that controller!", keyid, ci->index, ci->controller);
            return;
        }
        if (callback(ci->controller, ci->index, userState))
        {
            break;
        }
        else
        {
            scheme = (ControllerScheme*)LinkedList_getNext(controllerSchemeStack);
            if (scheme == NULL)
            {
                WARNING("Button %i on controller %x was pressed, but it was not handled in any controller scheme!", ci->index, ci->controller);
                break;
            }
        }
    }
}

static void _mousePositionCallback(int xPos, int yPos)
{
    smug_assert(mousePositionPointerBinding != NULL);
    ControllerScheme* scheme = (ControllerScheme*)LinkedList_getFirst(controllerSchemeStack);
    for (;;)
    {
        PointerCallback callback = ControllerScheme_getPointerCallback(scheme, mousePositionPointerBinding->controller);
        if (callback == NULL)
        {
            ERROR("Mouse position was mapped to pointer %i on controller %x, but no pointer callback was set for that controller!", mousePositionPointerBinding->index, mousePositionPointerBinding->controller);
            return;
        }
        if (callback(mousePositionPointerBinding->controller, mousePositionPointerBinding->index, xPos, yPos))
        {
            break;
        }
        else
        {
            scheme = (ControllerScheme*)LinkedList_getNext(controllerSchemeStack);
            if (scheme == NULL)
            {
                WARNING("Pointer %i on controller %x was moved, but it was not handled in any controller scheme!", mousePositionPointerBinding->index, mousePositionPointerBinding->controller);
                break;
            }
        }
    }
}

void Input_pushControllerScheme(ControllerScheme* newScheme)
{
    smug_assert(isInitialized);
    LinkedList_addFirst(controllerSchemeStack, newScheme);
}

ControllerScheme* Input_popControllerScheme()
{
    smug_assert(isInitialized);
    // User is not allowed to pop the default scheme.
    smug_assert(LinkedList_length(controllerSchemeStack) > 1);
    return LinkedList_popFirst(controllerSchemeStack);
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

void Input_terminate()
{
    glfwSetKeyCallback(NULL);
    glfwSetMousePosCallback(NULL);
    smug_assert(isInitialized);
    _deleteBindings(keyboardBindings);
    Map_delete(keyboardBindings);
    _deleteBindings(mouseButtonBindings);
    Map_delete(mouseButtonBindings);
    // Delete default controller scheme.
    ControllerScheme_delete(LinkedList_popLast(controllerSchemeStack));
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
