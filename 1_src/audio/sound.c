#include <common.h>
#include <audio/myfmod.h>
#include <audio/audio.h>
#include <audio/sound.h>

typedef struct _Sound
{
    FMOD_SOUND* fmodSound;
    FMOD_CHANNEL* channel;
} _Sound;

static FMOD_RESULT result = 0;
FMOD_SYSTEM* Audio_getSystem();

static BOOL _invariant(Sound* self)
{
    return (self != NULL && self->fmodSound != NULL) || self == NULL;
}

Sound* Sound_new(const char* fileName)
{
    smug_assert(Audio_getSystem() != NULL);
    Sound* newSound = allocate(Sound);
    result = FMOD_System_CreateSound(Audio_getSystem(), fileName, FMOD_HARDWARE, 0, &newSound->fmodSound);
    if (result != FMOD_OK)
    {
        FMOD_ERRCHECK(result);
        free(newSound);
        return NULL;
    }
    newSound->channel = NULL;
    smug_assert(_invariant(newSound));
    return newSound;
}

void Sound_delete(Sound* self)
{
    smug_assert(_invariant(self));
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
    smug_assert(_invariant(self));
    smug_assert(Audio_getSystem() != NULL);
    if (self == NULL)
    {
        WARNING("Sound is not loaded.");
        return;
    }
    result = FMOD_System_PlaySound(Audio_getSystem(), FMOD_CHANNEL_FREE, self->fmodSound, 0, &self->channel);
    FMOD_ERRCHECK(result);
    LOG(LOG_SOUND, "Played sound on channel %x", self->channel);
}

void Sound_stop(Sound* self)
{
    smug_assert(_invariant(self));
}

void Sound_setLoop(Sound* self, BOOL loop)
{
    smug_assert(_invariant(self));
    if (self == NULL)
    {
        WARNING("Sound is not loaded.");
        return;
    }
    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    result = FMOD_Sound_SetMode(self->fmodSound, mode);
    FMOD_ERRCHECK(result);
}
