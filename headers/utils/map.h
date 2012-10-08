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

typedef struct Map {
    struct BinaryTree* data;
    int (*keyCompare)(void* key1, void* key2);
} Map;

Map* Map_new(int (*keyCompare)(void* key1, void* key2));

void Map_delete(Map* self);

BOOL Map_isEmpty(Map* self);

void Map_set(Map* self, void* key, void* data);

void* Map_get(Map* self, void* key);

// void Map_remove(BinaryTree* self, void* key);

void Map_removeAll(Map* self);

#endif /* SMUG_UTILS_MAP_H */

/**@}*/
