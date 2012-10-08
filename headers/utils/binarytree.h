/**
 * @file binarytree.h
 * @brief Defines a binary tree type (and a node type for it).
 * @if doc_files
 * @ingroup smug_utils
 * @endif
 */

/**
 * @addtogroup smug_utils
 * @{
 */

#ifndef SMUG_UTILS_BINARYTREE_H
#define SMUG_UTILS_BINARYTREE_H

#include <common.h>

typedef struct BinaryTree {
    struct BinaryTreeNode* root;
    int (*compare)(void*, void*);
    int (*comparePredicated)(void*, void*, void*);
    void* predicateData;
} BinaryTree;

/**
 * Creates and returns a new empty node.
 *
 * @relates Node
 * @return A pointer to the node just created.
 */
// BinaryTreeNode* BinaryTreeNode_new(void);

/**
 * Destroys a node struct
 *
 * @relates Node
 * @param node A pointer to the node to be deleted.
 */
// void BinaryTreeNode_delete(BinaryTreeNode* self);

BinaryTree* BinaryTree_new(int (*compare)(void*, void*));

BinaryTree* BinaryTree_newPredicated(int (*comparePredicated)(void*, void*, void*), void* predicateData);

void BinaryTree_delete(BinaryTree* self);

BOOL BinaryTree_isEmpty(BinaryTree* self);

// int BinaryTree_nrOfElements(BinaryTree* self);

void BinaryTree_insert(BinaryTree* self, void* item);

void* BinaryTree_find(BinaryTree* self, void* item);

// void BinaryTree_remove(BinaryTree* self, void* item);

void BinaryTree_removeAll(BinaryTree* self);

void BinaryTree_deleteAll(BinaryTree* self, void (*deleter)(void* item));

#endif /* SMUG_UTILS_BINARYTREE_H */

/**@}*/
