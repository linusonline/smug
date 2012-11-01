#include <audio/myfmod.h>

#include <utils/log.h>
#include <audio/soundfactory.h>

#include <audio/audio.h>

typedef struct _Sound
{
    FMOD_SOUND* fmodSound;
    FMOD_CHANNEL* channel;
} _Sound;

static FMOD_SYSTEM *fmodSystem = NULL;
static FMOD_RESULT result = 0;
static BOOL isInitialized = FALSE;

static BOOL _invariant()
{
    return (fmodSystem != NULL && isInitialized) ||
           (fmodSystem == NULL && !isInitialized);
}

static BOOL _invariantSound(Sound* self)
{
    return _invariant() && ((self != NULL && self->fmodSound != NULL) || self == NULL);
}

#define FMOD_ERRCHECK(result) \
do \
{ \
    if (result != FMOD_OK) \
    { \
        ERROR("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result)); \
    } \
} while (0)

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

Sound* Sound_new(const char* fileName)
{
    smug_assert(_invariant());
    smug_assert(Audio_isInitialized());

    Sound* newSound = allocate(Sound);
    result = FMOD_System_CreateSound(fmodSystem, fileName, FMOD_HARDWARE, 0, &newSound->fmodSound);
    if (result != FMOD_OK)
    {
        FMOD_ERRCHECK(result);
        free(newSound);
        return NULL;
    }
    newSound->channel = NULL;
    smug_assert(_invariantSound(newSound));
    return newSound;
}

void Sound_delete(Sound* self)
{
    smug_assert(_invariantSound(self));
    smug_assert(Audio_isInitialized());
    if (self == NULL)
    {
        return;
    }
    result = FMOD_Sound_Release(self->fmodSound);
    FMOD_ERRCHECK(result);
    free(self);
}

void Sound_play(Sound* self)
{
    smug_assert(_invariantSound(self));
    smug_assert(Audio_isInitialized());
    if (self == NULL)
    {
        WARNING("Sound is not loaded.");
        return;
    }
    result = FMOD_System_PlaySound(fmodSystem, FMOD_CHANNEL_FREE, self->fmodSound, 0, &self->channel);
    FMOD_ERRCHECK(result);
    LOG(LOG_SOUND, "Played sound on channel %x", self->channel);
}

void Sound_stop(Sound* self)
{
    smug_assert(_invariantSound(self));
    smug_assert(Audio_isInitialized());
}

void Sound_setLoop(Sound* self, BOOL loop)
{
    smug_assert(_invariantSound(self));
    smug_assert(Audio_isInitialized());
    if (self == NULL)
    {
        WARNING("Sound is not loaded.");
        return;
    }
    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    result = FMOD_Sound_SetMode(self->fmodSound, mode);
    FMOD_ERRCHECK(result);
}
