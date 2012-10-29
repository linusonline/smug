/**
 * @file hook.h
 * @brief Holds a simple struct for callable objects
 * @if doc_files
 * @ingroup smug_utils
 * @endif
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_HOOK_H
#define SMUG_UTILS_HOOK_H

#include <common.h>

typedef struct Hook2Params
{
    void* data1;
    void* data2;
    void (*function)(void*, void*);
} Hook2Params;

Hook2Params* Hook_new2Params(void (*function)(void*, void*), void* data1, void* data2);
void Hook_delete2Params(Hook2Params* self);
void Hook_call(Hook2Params* self);

#endif /* SMUG_UTILS_HOOK_H */

/**@}*/
