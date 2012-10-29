#include <common.h>
#include <input/controller.h>

typedef struct _Controller
{
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
