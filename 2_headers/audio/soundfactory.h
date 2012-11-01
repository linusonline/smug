#ifndef SMUG_AUDIO_SOUNDFACTORY_H
#define SMUG_AUDIO_SOUNDFACTORY_H

#include <common.h>
#include <audio/sound.h>

void SoundFactory_initialize();
void SoundFactory_terminate();
Sound* SoundFactory_getSound(const char* fileName);
void SoundFactory_deleteAllSounds();

#endif /* SMUG_AUDIO_SOUNDFACTORY_H */
