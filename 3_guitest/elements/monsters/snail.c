#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <audio/soundfactory.h>

#include <monster.h>
#include <monster_protected.h>
#include <monsters/snail.h>

static SpriteSheet* snailSheet = NULL;
static Sound* hitSound = NULL;
static Sound* dieSound = NULL;

GameObject* newMonsterSnail(float posX, float posY)
{
    if (snailSheet == NULL)
    {
        snailSheet = SpriteSheet_new("5_res/characters/monsters/snell16x16.png", "5_res/characters/monsters/16x16.txt");
    }
    if (hitSound == NULL)
    {
        hitSound = SoundFactory_getSound("5_res/audio/monster-hit.wav");
    }
    if (dieSound == NULL)
    {
        dieSound = SoundFactory_getSound("5_res/audio/insect-die.wav");
    }

    return newMonsterFromSheet(snailSheet, 16, 16, posX, posY, -8, -16, 20, hitSound, dieSound);
}
