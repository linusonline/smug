#ifndef SMUG_INPUT_CONTROLLER_H
#define SMUG_INPUT_CONTROLLER_H

#include <common.h>

struct _Controller;
typedef struct _Controller Controller;

typedef void (*ButtonCallback)(Controller* controller, int buttonIndex, int state);
typedef void (*PointerCallback)(Controller* controller, int pointerIndex, int xDelta, int yDelta);

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
