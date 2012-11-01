#ifndef SMUG_AUDIO_SOUND_H
#define SMUG_AUDIO_SOUND_H

struct _Sound;
typedef struct _Sound Sound;

Sound* Sound_new(const char* fileName);
void Sound_delete(Sound* self);
void Sound_play(Sound* self);
void Sound_stop(Sound* self);
void Sound_setLoop(Sound* self, BOOL loop);

#endif /* SMUG_AUDIO_SOUND_H */
