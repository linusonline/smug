#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <audio/soundfactory.h>

#include <monster.h>
#include <monster_protected.h>
#include <monsters/fireskull.h>

static SpriteSheet* fireskullSheet = NULL;
static Sound* hitSound = NULL;
static Sound* dieSound = NULL;

GameObject* newMonsterFireskull(float posX, float posY)
{
    if (fireskullSheet == NULL)
    {
        fireskullSheet = SpriteSheet_new("5_res/characters/monsters/fireskull16x32.png", "5_res/characters/monsters/16x32.txt");
    }
    if (hitSound == NULL)
    {
        hitSound = SoundFactory_getSound("5_res/audio/monster-hit.wav");
    }
    if (dieSound == NULL)
    {
        dieSound = SoundFactory_getSound("5_res/audio/monster-die.wav");
    }

    return newMonsterFromSheet(fireskullSheet, 16, 32, posX, posY, -8, -32, 50, hitSound, dieSound);
}
