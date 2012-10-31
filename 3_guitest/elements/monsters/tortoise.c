#include <common.h>
#include <engine/gameobject.h>
#include <graphics/spritesheet.h>
#include <audio/audio.h>

#include <monster.h>
#include <monster_protected.h>
#include <monsters/tortoise.h>

static SpriteSheet* shellySheet = NULL;
static Sound* hitSound = NULL;

GameObject* newMonsterTortoise(float posX, float posY)
{
    if (shellySheet == NULL)
    {
        shellySheet = SpriteSheet_new("5_res/characters/monsters/shelly32x32.png", "5_res/characters/monsters/32x32.txt");
    }
    if (hitSound == NULL)
    {
        hitSound = Sound_new("5_res/audio/flyswatter.wav");
    }

    return newMonsterFromSheet(shellySheet, 32, 32, posX, posY, -16, -32, 150);
}
