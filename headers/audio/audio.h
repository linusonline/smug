#ifndef SMUG_AUDIO_H
#define SMUG_AUDIO_H

#include <common.h>

struct _Sound;
typedef struct _Sound Sound;

BOOL Audio_initialize(void);
void Audio_terminate(void);
BOOL Audio_isInitialized(void);

Sound* Sound_new(const char* fileName);
void Sound_delete(Sound* self);
void Sound_play(Sound* self);
void Sound_stop(Sound* self);
void Sound_setLoop(Sound* self, BOOL loop);

#endif /* SMUG_AUDIO_H */
