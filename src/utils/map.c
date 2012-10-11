#include <utils/log.h>
#include <utils/binarytree.h>

#include <utils/map.h>

static BOOL _invariant(Map* self)
{
    return
        self != NULL &&
        self->keyCompare != NULL;
}

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
    smug_assert(_invariant(newMap));
    return newMap;
}

int Map_compareInts(void* intKey1, void* intKey2)
{
    return *((int*)intKey1) == *((int*)intKey2) ? 0 : (*((int*)intKey1) < *((int*)intKey2) ? -1 : 1);
}

int Map_comparePointers(void* ptrKey1, void* ptrKey2)
{
    return ptrKey1 == ptrKey2 ? 0 : ptrKey1 < ptrKey2 ? -1 : 1;
}

int* Map_newInt(int anInteger)
{
    int* i = allocate(int);
    *i = anInteger;
    return i;
}

void Map_deleteInt(int* intp)
{
    free(intp);
}

void Map_delete(Map* self)
{
    smug_assert(_invariant(self));
    // TODO: Traverse binary tree and delete contents with MapPair_delete.
    BinaryTree_delete(self->data);
    free(self);
}

BOOL Map_isEmpty(Map* self)
{
    smug_assert(_invariant(self));
    return BinaryTree_isEmpty(self->data);
}

void Map_set(Map* self, void* key, void* value)
{
    smug_assert(_invariant(self));
    dummyPair.key = key;

    MapPair* pair = BinaryTree_find(self->data, &dummyPair);
    if (pair == NULL)
    {
        DEBUG("Inserting key-value pair: %x -> %x", key, value);
        pair = MapPair_new(key, value);
        BinaryTree_insert(self->data, pair);
    }
    else
    {
        DEBUG("Setting key-value pair: %x -> %x", key, value);
        pair->value = value;
    }
}

void* Map_get(Map* self, void* key)
{
    smug_assert(_invariant(self));
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
    smug_assert(_invariant(self));
    BinaryTree_deleteAll(self->data, MapPair_deleteVoid);
}

void Map_doForEach(Map* self, void (*function)(void* key, void* value))
{
    smug_assert(_invariant(self));
    MapIterator* iter = Map_getIterator(self);
    MapPair* pair = (MapPair*)BinaryTreeIterator_getNext(iter);
    while (pair != NULL)
    {
        function(pair->key, pair->value);
        pair = (MapPair*)BinaryTreeIterator_getNext(iter);
    }
    MapIterator_delete(iter);
}

MapIterator* Map_getIterator(Map* self)
{
    smug_assert(_invariant(self));
    return BinaryTree_getIterator(self->data);
}

void* MapIterator_getNextValue(MapIterator* self)
{
    MapPair* p = (MapPair*)BinaryTreeIterator_getNext(self);
    return p->value;
}

void* MapIterator_getNextKey(MapIterator* self)
{
    MapPair* p = (MapPair*)BinaryTreeIterator_getNext(self);
    return p == NULL ? NULL : p->value;
}

MapPair* MapIterator_getNextPair(MapIterator* self)
{
    return (MapPair*)BinaryTreeIterator_getNext(self);
}

BOOL MapIterator_hasMore(MapIterator* self)
{
    return BinaryTreeIterator_hasMore(self);
}

void MapIterator_delete(MapIterator* self)
{
    BinaryTreeIterator_delete(self);
}
