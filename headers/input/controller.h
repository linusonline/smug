#ifndef SMUG_INPUT_CONTROLLER_H
#define SMUG_INPUT_CONTROLLER_H

#include <common.h>

struct Controller;

typedef void (*ButtonCallback)(struct Controller* controller, int buttonIndex, int state);
typedef void (*PointerCallback)(struct Controller* controller, int pointerIndex, int xDelta, int yDelta);

typedef struct Controller
{
    ButtonCallback buttonCallback;
    PointerCallback pointerCallback;
    int numberOfAxes;
    int numberOfButtons;
    int numberOfPointers;
} Controller;

Controller* Controller_new(int numberOfAxes, int numberOfButtons, int numberOfPointers);
void Controller_delete(Controller* controller);

void Controller_setButtonCallback(Controller* self, ButtonCallback buttonCallback);
void Controller_setPointerCallback(Controller* self, PointerCallback pointerCallback);

ButtonCallback Controller_getButtonCallback(Controller* self);
PointerCallback Controller_getPointerCallback(Controller* self);

BOOL Controller_hasButton(Controller* self, int index);
BOOL Controller_hasAxis(Controller* self, int index);
BOOL Controller_hasPointer(Controller* self, int index);

#endif /* SMUG_INPUT_CONTROLLER_H */
