#include <utils/map.h>
#include <utils/binarytree.h>

typedef struct MapPair
{
  void* key;
  void* value;
} MapPair;

static MapPair* MapPair_new(void* key, void* value)
{
    MapPair* newPair = allocate(MapPair);
    newPair->key = key;
    newPair->value = value;
    return newPair;
}

static void MapPair_delete(MapPair* self)
{
    free(self);
}

static void MapPair_deleteVoid(void* self)
{
    MapPair_delete((MapPair*)self);
}

/** Comparison function for maps
 *
 * Compares the keys of two key-value pairs with the map's key
 * comparison function.
 *
 * @relatesalso Map
 * @param map Pointer to the Map containing the tree
 * @param first The first MapPair
 * @param second The second MapPair
 * @return -1, 0 or 1
 */
static int _pairCompare(void* map, void* first, void* second)
{
    return ((Map*)map)->keyCompare(((MapPair*)first)->key, ((MapPair*)second)->key);
}

static MapPair dummyPair;

Map* Map_new(int (*keyCompare)(void* key1, void* key2))
{
    Map* newMap = allocate(Map);
    newMap->keyCompare = keyCompare;
    newMap->data = BinaryTree_newPredicated(_pairCompare, newMap);
    return newMap;
}

void Map_delete(Map* map)
{
    // TODO: Traverse binary tree and delete contents with MapPair_delete.
    BinaryTree_delete(map->data);
    free(map);
}

BOOL Map_isEmpty(Map* self)
{
    return BinaryTree_isEmpty(self->data);
}

void Map_set(Map* self, void* key, void* value)
{
    dummyPair.key = key;

    MapPair* pair = BinaryTree_find(self->data, &dummyPair);
    if (pair == NULL)
    {
        pair = MapPair_new(key, value);
        BinaryTree_insert(self->data, pair);
    }
    else
    {
        pair->value = value;
    }
}

void* Map_get(Map* self, void* key)
{
    dummyPair.key = key;

    MapPair* pair = BinaryTree_find(self->data, &dummyPair);

    if (pair == NULL)
    {
        return NULL;
    }

    return pair->value;
}

void Map_removeAll(Map* self)
{
    BinaryTree_deleteAll(self->data, MapPair_deleteVoid);
}
