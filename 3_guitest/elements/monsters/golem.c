#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <audio/soundfactory.h>

#include <monster.h>
#include <monster_protected.h>
#include <monsters/golem.h>

static SpriteSheet* golemSheet = NULL;
static Sound* hitSound = NULL;
static Sound* dieSound = NULL;

GameObject* newMonsterGolem(float posX, float posY)
{
    if (golemSheet == NULL)
    {
        golemSheet = SpriteSheet_new("5_res/characters/monsters/golem32x32.png", "5_res/characters/monsters/32x32.txt");
    }
    if (hitSound == NULL)
    {
        hitSound = SoundFactory_getSound("5_res/audio/monster-hit.wav");
    }
    if (dieSound == NULL)
    {
        dieSound = SoundFactory_getSound("5_res/audio/giant-die.wav");
    }

    return newMonsterFromSheet(golemSheet, 32, 32, posX, posY, -16, -32, 200, hitSound, dieSound);
}
