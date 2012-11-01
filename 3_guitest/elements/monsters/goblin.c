#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <audio/soundfactory.h>

#include <monster.h>
#include <monster_protected.h>
#include <monsters/goblin.h>

static SpriteSheet* goblinSheet = NULL;
static Sound* hitSound = NULL;
static Sound* dieSound = NULL;

GameObject* newMonsterGoblin(float posX, float posY)
{
    if (goblinSheet == NULL)
    {
        goblinSheet = SpriteSheet_new("5_res/characters/monsters/trolley16x32.png", "5_res/characters/monsters/16x32.txt");
    }
    if (hitSound == NULL)
    {
        hitSound = SoundFactory_getSound("5_res/audio/monster-hit.wav");
    }
    if (dieSound == NULL)
    {
        dieSound = SoundFactory_getSound("5_res/audio/insect-die.wav");
    }

    return newMonsterFromSheet(goblinSheet, 16, 32, posX, posY, -8, -32, 70, hitSound, dieSound);
}
