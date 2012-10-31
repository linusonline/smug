#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <audio/audio.h>

#include <monster.h>
#include <monster_protected.h>
#include <monsters/tortoise.h>

static SpriteSheet* shellySheet = NULL;
static Sound* hitSound = NULL;
static Sound* dieSound = NULL;

GameObject* newMonsterTortoise(float posX, float posY)
{
    if (shellySheet == NULL)
    {
        shellySheet = SpriteSheet_new("5_res/characters/monsters/shelly32x32.png", "5_res/characters/monsters/32x32.txt");
    }
    if (hitSound == NULL)
    {
        hitSound = Sound_new("5_res/audio/monster-hit.wav");
    }
    if (dieSound == NULL)
    {
        dieSound = Sound_new("5_res/audio/insect-die.wav");
    }

    return newMonsterFromSheet(shellySheet, 32, 32, posX, posY, -16, -32, 150, hitSound, dieSound);
}
