#include <stdlib.h>
#include <time.h>

#include <common.h>
#include <engine/engine.h>
#include <audio/audio.h>

static Sound* loopSound = NULL;
#define RANDOM_SOUNDS 2
static Sound* randomSounds[RANDOM_SOUNDS];

static const TIME MIN_INTERVAL = 1.0;
static const TIME MAX_INTERVAL = 10.0;

static TIME _randomInterval()
{
    return MIN_INTERVAL + (TIME)(rand() % (int)((MAX_INTERVAL - MIN_INTERVAL) * 100.0)) / 100.0;
}

static Sound* _getRandomSound()
{
    return randomSounds[rand() % RANDOM_SOUNDS];
}

static void _playNextRandomSound(void* ignored)
{
    Sound_play(_getRandomSound());
    Engine_delayCallback(_playNextRandomSound, NULL, _randomInterval());
}

void Ambience_init()
{
    srand((int)time(NULL));
    loopSound = Sound_new("5_res/audio/amb-forest.flac");
    randomSounds[0] = Sound_new("5_res/audio/amb-bird-1.flac");
    randomSounds[1] = Sound_new("5_res/audio/amb-bird-3.flac");
    // randomSounds[1] = Sound_new("5_res/audio/amb-bird-2.flac");
}

void Ambience_play()
{
    Sound_setLoop(loopSound, TRUE);
    Sound_play(loopSound);
    Engine_delayCallback(_playNextRandomSound, NULL, _randomInterval());
    Engine_delayCallback(_playNextRandomSound, NULL, _randomInterval());
}
