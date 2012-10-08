/**
 * @file map.h
 * @brief Defines a map type.
 * @if doc_files
 * @ingroup smug_utils
 * @endif
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_MAP_H
#define SMUG_UTILS_MAP_H

#include <common.h>
#include <utils/binarytree.h>

typedef struct Map
{
    struct BinaryTree* data;
    int (*keyCompare)(void* key1, void* key2);
} Map;

typedef struct MapPair
{
    void* key;
    void* value;
} MapPair;

typedef BinaryTreeIterator MapIterator;

Map* Map_new(int (*keyCompare)(void* key1, void* key2));

int Map_compareInts(void* intKey1, void* intKey2);

void Map_delete(Map* self);

BOOL Map_isEmpty(Map* self);

void Map_set(Map* self, void* key, void* data);

void* Map_get(Map* self, void* key);

// void Map_remove(BinaryTree* self, void* key);

void Map_removeAll(Map* self);

void Map_doForEach(Map* self, void (*function)(void* key, void* value));

MapIterator* Map_getIterator(Map* self);

void* MapIterator_getNextKey(MapIterator* self);

void* MapIterator_getNextValue(MapIterator* self);

MapPair* MapIterator_getNextPair(MapIterator* self);

BOOL MapIterator_hasMore(MapIterator* self);

void MapIterator_delete(MapIterator* self);

#endif /* SMUG_UTILS_MAP_H */

/**@}*/
