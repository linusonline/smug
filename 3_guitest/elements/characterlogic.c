#include <objects.h>
#include <characterlogic.h>

#define toCharLogic(object) ((CharacterLogic*)getSpecificObjectData(object))

BOOL CharacterLogic_takeDamage(GameObject* character, float damage)
{
    CharacterLogic* data = toCharLogic(character);
    BOOL dead = data->hp - damage <= 0;
    data->hp = max(0, data->hp - damage);
    return dead;
}
