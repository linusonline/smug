#include <audio/myfmod.h>

#include <utils/log.h>
#include <audio/soundfactory.h>

#include <audio/audio.h>

static FMOD_SYSTEM *fmodSystem = NULL;
static FMOD_RESULT result = 0;
static BOOL isInitialized = FALSE;

static BOOL _invariant()
{
    return (fmodSystem != NULL && isInitialized) ||
           (fmodSystem == NULL && !isInitialized);
}

// "Friend" of Sound.
FMOD_SYSTEM* Audio_getSystem()
{
    return fmodSystem;
}

BOOL Audio_initialize()
{
    smug_assert(!Audio_isInitialized());
    unsigned int version;

    result = FMOD_System_Create(&fmodSystem);
    FMOD_ERRCHECK(result);

    result = FMOD_System_GetVersion(fmodSystem, &version);
    FMOD_ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        ERROR("Error! You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        Audio_terminate();
        return FALSE;
    }

    result = FMOD_System_Init(fmodSystem, 32, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK)
    {
        FMOD_ERRCHECK(result);
        Audio_terminate();
        return FALSE;
    }
    SoundFactory_initialize();
    isInitialized = TRUE;
    return TRUE;
}

void Audio_terminate()
{
    smug_assert(_invariant());
    smug_assert(Audio_isInitialized());

    SoundFactory_terminate();
    result = FMOD_System_Close(fmodSystem);
    FMOD_ERRCHECK(result);
    result = FMOD_System_Release(fmodSystem);
    FMOD_ERRCHECK(result);
    fmodSystem = NULL;
    isInitialized = FALSE;

    smug_assert(_invariant());
}

BOOL Audio_isInitialized()
{
    smug_assert(_invariant());
    return isInitialized;
}
