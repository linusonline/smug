#ifndef SMUG_INPUT_CONTROLLER_H
#define SMUG_INPUT_CONTROLLER_H

#include <common.h>

struct _Controller;
typedef struct _Controller Controller;

Controller* Controller_new(int numberOfAxes, int numberOfButtons, int numberOfPointers);
void Controller_delete(Controller* controller);

BOOL Controller_hasButton(Controller* self, int index);
BOOL Controller_hasAxis(Controller* self, int index);
BOOL Controller_hasPointer(Controller* self, int index);

BOOL Controller_isButtonPressed(Controller* self, int index);
void Controller_setButtonPressed(Controller* self, int index, BOOL pressed);

#endif /* SMUG_INPUT_CONTROLLER_H */
