#ifndef SMUG_INPUT_CONTROLLERSCHEME_H
#define SMUG_INPUT_CONTROLLERSCHEME_H

#include <input/controller.h>

typedef void (*ButtonCallback)(Controller* controller, int buttonIndex, int state);
typedef void (*PointerCallback)(Controller* controller, int pointerIndex, int xDelta, int yDelta);

struct _ControllerScheme;
typedef struct _ControllerScheme ControllerScheme;

ControllerScheme* ControllerScheme_new(void);
void ControllerScheme_delete(ControllerScheme* self);

void ControllerScheme_setButtonCallbackForController(ControllerScheme* cs, Controller* c, ButtonCallback buttonCallback);
ButtonCallback ControllerScheme_getButtonCallback(ControllerScheme* cs, Controller* c);

void ControllerScheme_setPointerCallbackForController(ControllerScheme* cs, Controller* c, PointerCallback pointerCallback);
PointerCallback ControllerScheme_getPointerCallback(ControllerScheme* cs, Controller* c);

#endif /* SMUG_INPUT_CONTROLLERSCHEME_H */
