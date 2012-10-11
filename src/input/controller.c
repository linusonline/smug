#include <common.h>
#include <input/controller.h>

typedef struct _Controller
{
    ButtonCallback buttonCallback;
    PointerCallback pointerCallback;
    int numberOfAxes;
    int numberOfButtons;
    int numberOfPointers;
} _Controller;

Controller* Controller_new(int numberOfAxes, int numberOfButtons, int numberOfPointers)
{
    Controller* newController = allocate(Controller);
    newController->numberOfAxes = numberOfAxes;
    newController->numberOfButtons = numberOfButtons;
    newController->numberOfPointers = numberOfPointers;
    newController->buttonCallback = NULL;
    newController->pointerCallback = NULL;
}

void Controller_delete(Controller* controller)
{
    free(controller);
}

void Controller_setButtonCallback(Controller* self, ButtonCallback buttonCallback)
{
    self->buttonCallback = buttonCallback;
}

void Controller_setPointerCallback(Controller* self, PointerCallback pointerCallback)
{
    self->pointerCallback = pointerCallback;
}

ButtonCallback Controller_getButtonCallback(Controller* self)
{
    return self->buttonCallback;
}

PointerCallback Controller_getPointerCallback(Controller* self)
{
    return self->pointerCallback;
}

BOOL Controller_hasButton(Controller* self, int index)
{
    return index >= 0 && index < self->numberOfButtons;
}

BOOL Controller_hasAxis(Controller* self, int index)
{
    return index >= 0 && index < self->numberOfAxes;
}

BOOL Controller_hasPointer(Controller* self, int index)
{
    return index >= 0 && index < self->numberOfPointers;
}
