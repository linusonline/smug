#ifndef ELEMENTS_MONSTER_PROTECTED_H
#define ELEMENTS_MONSTER_PROTECTED_H

#include <common.h>
#include <graphics/spritesheet.h>
#include <engine/gameobject.h>

#include <monster.h>

GameObject* newMonsterFromSheet(SpriteSheet* sheet, float width, float height, float posX, float posY, float offsetX, float offsetY, float hp);

#endif /* ELEMENTS_MONSTER_PROTECTED_H */
