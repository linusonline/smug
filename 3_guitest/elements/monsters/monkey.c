#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <audio/soundfactory.h>

#include <monster.h>
#include <monster_protected.h>
#include <monsters/monkey.h>

static SpriteSheet* monkeySheet = NULL;
static Sound* hitSound = NULL;
static Sound* dieSound = NULL;

GameObject* newMonsterMonkey(float posX, float posY)
{
    if (monkeySheet == NULL)
    {
        monkeySheet = SpriteSheet_new("5_res/characters/monsters/minkey32x32.png", "5_res/characters/monsters/32x32.txt");
    }
    if (hitSound == NULL)
    {
        hitSound = SoundFactory_getSound("5_res/audio/monster-hit.wav");
    }
    if (dieSound == NULL)
    {
        dieSound = SoundFactory_getSound("5_res/audio/giant-die.wav");
    }

    return newMonsterFromSheet(monkeySheet, 32, 32, posX, posY, -16, -32, 100, hitSound, dieSound);
}
