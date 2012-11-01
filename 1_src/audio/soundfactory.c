#include <string.h>
#include <common.h>
#include <utils/map.h>
#include <audio/audio.h>
#include <audio/soundfactory.h>

static Map* sounds = NULL; // char* -> Sound*

static char* _copyString(const char* string)
{
    char* newString = allocatev(char, strlen(string)+1);
    return strcpy(newString, string);
}

void SoundFactory_initialize()
{
    smug_assert(sounds == NULL);
    sounds = Map_new(Map_compareStrings);
}

void SoundFactory_terminate()
{
    SoundFactory_deleteAllSounds();
    Map_delete(sounds);
    sounds = NULL;
}

Sound* SoundFactory_getSound(const char* fileName)
{
    smug_assert(sounds != NULL);
    char* name = _copyString(fileName);
    Sound* sound = Map_get(sounds, name);
    if (sound == NULL)
    {
        sound = Sound_new(fileName);
        if (sound == NULL)
        {
            free(name);
            return NULL;
        }
        Map_set(sounds, name, sound);
    }
    else
    {
        free(name);
    }
    return sound;
}

static void _deleteStringAndSound(void* string, void* sound)
{
    free((char*)string);
    Sound_delete((Sound*)sound);
}

void SoundFactory_deleteAllSounds()
{
    smug_assert(sounds != NULL);
    Map_doForEach(sounds, _deleteStringAndSound);
    Map_removeAll(sounds);
}
