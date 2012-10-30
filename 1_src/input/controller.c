#include <common.h>
#include <input/controller.h>

typedef struct _Controller
{
    int numberOfAxes;
    int numberOfButtons;
    int numberOfPointers;
    BOOL* buttonsPressed;
} _Controller;

Controller* Controller_new(int numberOfAxes, int numberOfButtons, int numberOfPointers)
{
    Controller* newController = allocate(Controller);
    newController->numberOfAxes = numberOfAxes;
    newController->numberOfButtons = numberOfButtons;
    newController->numberOfPointers = numberOfPointers;
    newController->buttonsPressed = allocatev(BOOL, numberOfButtons);
    for (int i = 0; i < numberOfButtons; i++)
    {
        newController->buttonsPressed[i] = FALSE;
    }
    return newController;
}

void Controller_delete(Controller* controller)
{
    free(controller);
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

BOOL Controller_isButtonPressed(Controller* self, int index)
{
    smug_assert(Controller_hasButton(self, index));
    return self->buttonsPressed[index];
}

void Controller_setButtonPressed(Controller* self, int index, BOOL pressed)
{
    smug_assert(Controller_hasButton(self, index));
    self->buttonsPressed[index] = pressed;
}
