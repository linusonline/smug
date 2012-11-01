#ifndef SMUG_MYFMOD_H
#define SMUG_MYFMOD_H

#ifndef __CYGWIN32__
# define __CYGWIN32__
# define SMUG_UNDEF__CYGWIN32__
#endif

#include <fmod.h>
#include <fmod_errors.h>

#ifdef SMUG_UNDEF__CYGWIN32__
# undef __CYGWIN32__
#endif

#define FMOD_ERRCHECK(result) \
do \
{ \
    if (result != FMOD_OK) \
    { \
        ERROR("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result)); \
    } \
} while (0)

#endif /* SMUG_MYFMOD_H */
