#ifndef SMUG_INPUT_INPUT_H
#define SMUG_INPUT_INPUT_H

#include <common.h>
#include <utils/log.h>
#include <input/input.h>

#include <input/controller.h>

static const int SMUG_KEY_PRESS = 0;
static const int SMUG_KEY_RELEASE = 1;

void Input_initialize();
void Input_terminate();

void Input_linkControllerToKeyboardKey(Controller* controller, int buttonIndex, int keyid);
void Input_unlinkControllersFromKeyboardKey(int keyid);

void Input_linkControllerToMousePosition(Controller* controller, int pointerIndex);
void Input_unlinkControllersFromMousePosition();

#endif /* SMUG_INPUT_INPUT_H */
