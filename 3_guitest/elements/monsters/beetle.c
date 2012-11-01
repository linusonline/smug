#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <audio/soundfactory.h>

#include <monster.h>
#include <monster_protected.h>
#include <monsters/beetle.h>

static SpriteSheet* beetleSheet = NULL;
static Sound* hitSound = NULL;
static Sound* dieSound = NULL;

GameObject* newMonsterBeetle(float posX, float posY)
{
    if (beetleSheet == NULL)
    {
        beetleSheet = SpriteSheet_new("5_res/characters/monsters/beetle32x32.png", "5_res/characters/monsters/32x32.txt");
    }
    if (hitSound == NULL)
    {
        hitSound = SoundFactory_getSound("5_res/audio/monster-hit.wav");
    }
    if (dieSound == NULL)
    {
        dieSound = SoundFactory_getSound("5_res/audio/insect-die.wav");
    }

    return newMonsterFromSheet(beetleSheet, 32, 32, posX, posY, -16, -32, 100, hitSound, dieSound);
}
