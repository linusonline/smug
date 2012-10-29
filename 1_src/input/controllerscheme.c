#include <input/controllerscheme.h>
#include <utils/map.h>

typedef struct _ControllerScheme
{
    Map* buttonCallbacks; // Controller -> ButtonCallback
    Map* pointerCallbacks; // Controller -> PointerCallback
} _ControllerScheme;

ControllerScheme* ControllerScheme_new()
{
    ControllerScheme* newScheme = allocate(ControllerScheme);
    newScheme->buttonCallbacks = Map_new(Map_comparePointers);
    newScheme->pointerCallbacks = Map_new(Map_comparePointers);
    return newScheme;
}

void ControllerScheme_delete(ControllerScheme* self)
{
    Map_delete(self->buttonCallbacks);
    Map_delete(self->pointerCallbacks);
    free(self);
}

void ControllerScheme_setButtonCallbackForController(ControllerScheme* cs, Controller* c, ButtonCallback buttonCallback)
{
    Map_set(cs->buttonCallbacks, (void*)c, (void*)buttonCallback);
}

ButtonCallback ControllerScheme_getButtonCallback(ControllerScheme* cs, Controller* c)
{
    return (ButtonCallback)Map_get(cs->buttonCallbacks, c);
}

void ControllerScheme_setPointerCallbackForController(ControllerScheme* cs, Controller* c, PointerCallback pointerCallback)
{
    Map_set(cs->pointerCallbacks, c, (void*)pointerCallback);
}

PointerCallback ControllerScheme_getPointerCallback(ControllerScheme* cs, Controller* c)
{
    return (PointerCallback)Map_get(cs->pointerCallbacks, c);
}
