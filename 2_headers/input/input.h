#ifndef SMUG_INPUT_INPUT_H
#define SMUG_INPUT_INPUT_H

#include <common.h>

#include <input/controller.h>
#include <input/controllerscheme.h>

static const int SMUG_KEY_PRESS = 0;
static const int SMUG_KEY_RELEASE = 1;

void Input_initialize();
void Input_terminate();

void Input_linkControllerToKeyboardKey(Controller* controller, int buttonIndex, int keyid);
void Input_unlinkControllersFromKeyboardKey(int keyid);

void Input_linkControllerToMousePosition(Controller* controller, int pointerIndex);
void Input_unlinkControllersFromMousePosition();

void Input_setButtonCallbackForController(Controller* c, ButtonCallback buttonCallback);
ButtonCallback Input_getButtonCallback(Controller* c);

void Input_setPointerCallbackForController(Controller* c, PointerCallback pointerCallback);
PointerCallback Input_getPointerCallback(Controller* c);

void Input_pushControllerScheme(ControllerScheme* newScheme);
ControllerScheme* Input_popControllerScheme(void);

#endif /* SMUG_INPUT_INPUT_H */
