#include <utils/hook.h>

Hook2Params* Hook_new2Params(void (*function)(void*, void*), void* data1, void* data2)
{
    Hook2Params* newHook = allocate(Hook2Params);
    newHook->data1 = data1;
    newHook->data2 = data2;
    newHook->function = function;
    return newHook;
}

void Hook_delete2Params(Hook2Params* self)
{
    free(self);
}

void Hook_delete2ParamsVoid(void* self)
{
    Hook_delete2Params((Hook2Params*)self);
}

void Hook_call(Hook2Params* self)
{
    self->function(self->data1, self->data2);
}
