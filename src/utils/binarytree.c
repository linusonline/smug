#include <common.h>
#include <utils/binarytree.h>

typedef struct BinaryTreeNode {
    void* item;
    struct BinaryTreeNode* parent;
    struct BinaryTreeNode* left;
    struct BinaryTreeNode* right;
    // int height;  // Opti
} BinaryTreeNode;

static BinaryTreeNode* BinaryTreeNode_new(void* item)
{
    BinaryTreeNode* newNode = allocate(BinaryTreeNode);
    newNode->item = item;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

static void BinaryTreeNode_delete(BinaryTreeNode* self)
{
    free(self);
}

static BOOL _invariantRec(BinaryTreeNode* node)
{
    return (node->left == NULL ||
            (node->left->parent == node && _invariantRec(node->left))) &&
           (node->right == NULL ||
            (node->right->parent == node && _invariantRec(node->right)));
}

static BOOL _invariant(BinaryTree* self)
{
    return
        self != NULL &&
        (self->compare != NULL || self->comparePredicated != NULL) &&
        (self->root == NULL ||
        (self->root->parent == NULL && _invariantRec(self->root)));
}

static BOOL _isPredicated(BinaryTree* self)
{
    return self->comparePredicated != NULL;
}

static int _compare(BinaryTree* self, void* item1, void* item2)
{
    return _isPredicated(self) ? self->comparePredicated(self->predicateData, item1, item2) : self->compare(item1, item2);
}

static int _isLeftOrRightChild(BinaryTreeNode* node)
{
    if (node->parent == NULL)
    {
        return 0;
    }
    if (node->parent->left == node)
    {
        return -1;
    }
    if (node->parent->right == node)
    {
        return 1;
    }
    smug_assert(FALSE);
    return 0;
}

static int _findNode(BinaryTree* self, void* item, BinaryTreeNode** node)
{
    *node = NULL;
    if (BinaryTree_isEmpty(self))
    {
        return 0;
    }

    BinaryTreeNode* current = self->root;
    int comp;
    int leftOrRight = 0;
    while (TRUE)
    {
        comp = _compare(self, item, current->item);
        if (comp == 0)
        {
            *node = current;
            return leftOrRight;
        }
        if (comp < 0)
        {
            if (current->left == NULL)
            {
                return leftOrRight;
            }
            current = current->left;
            leftOrRight = -1;
        }
        if (comp > 0)
        {
            if (current->right == NULL)
            {
                return leftOrRight;
            }
            current = current->right;
            leftOrRight = 1;
        }
    }
}

static void _deleteWithChildren(BinaryTreeNode* node)
{
    if (node->left != NULL)
    {
        _deleteWithChildren(node->left);
    }
    if (node->right != NULL)
    {
        _deleteWithChildren(node->right);
    }
    free(node);
}

static void _deleteContentsWithChildren(BinaryTreeNode* node, void (*deleter)(void* item))
{
    if (node->left != NULL)
    {
        _deleteContentsWithChildren(node->left, deleter);
    }
    if (node->right != NULL)
    {
        _deleteContentsWithChildren(node->right, deleter);
    }
    deleter(node->item);
    free(node);
}

BinaryTree* BinaryTree_newPredicated(int (*comparePredicated)(void*, void*, void*), void* predicateData)
{
    smug_assert(comparePredicated != NULL);
    BinaryTree* newTree = allocate(BinaryTree);
    newTree->root = NULL;
    newTree->compare = NULL;
    newTree->comparePredicated = comparePredicated;
    newTree->predicateData = predicateData;
    smug_assert(_invariant(newTree));
    return newTree;
}

BinaryTree* BinaryTree_new(int (*compare)(void*, void*))
{
    smug_assert(compare != NULL);
    BinaryTree* newTree = allocate(BinaryTree);
    newTree->root = NULL;
    newTree->compare = compare;
    newTree->comparePredicated = NULL;
    newTree->predicateData = NULL;
    smug_assert(_invariant(newTree));
    return newTree;
}

void BinaryTree_delete(BinaryTree* self)
{
    smug_assert(_invariant(self));
    BinaryTree_removeAll(self);
    free(self);
}

void BinaryTree_insert(BinaryTree* self, void* item)
{
    smug_assert(_invariant(self));
    if (BinaryTree_isEmpty(self))
    {
        self->root = BinaryTreeNode_new(item);
        smug_assert(_invariant(self));
        return;
    }
    BinaryTreeNode* current = self->root;
    int comp;
    while (TRUE)
    {
        comp = _compare(self, item, current->item);
        if (comp == 0)
        {
            return;
        }
        if (comp < 0)
        {
            if (current->left == NULL)
            {
                current->left = BinaryTreeNode_new(item);
                current->left->parent = current;
                smug_assert(_invariant(self));
                return;
            }
            current = current->left;
        }
        if (comp > 0)
        {
            if (current->right == NULL)
            {
                current->right = BinaryTreeNode_new(item);
                current->right->parent = current;
                smug_assert(_invariant(self));
                return;
            }
            current = current->right;
        }
    }
}

void* BinaryTree_find(BinaryTree* self, void* item)
{
    smug_assert(_invariant(self));
    BinaryTreeNode* node;
    _findNode(self, item, &node);
    return node == NULL ? NULL : node->item;
}

// void BinaryTree_remove(BinaryTree* self, void* item)
// {
    // BinaryTreeNode* node;
    // int leftOrRight = _findNode(self, item, &node);
    // if (node == NULL)
    // {
        // return;
    // }
    // if (node->left == NULL && node->right == NULL)
    // {
        // if (leftOrRight < 0)
        // {
            // node->parent->left = NULL;
        // }
        // if (leftOrRight > 0)
        // {
            // node->parent->right = NULL;
        // }
        // if (leftOrRight == 0)
        // {
            // free(self->root);
            // self->root = NULL;
        // }
        // free(node);
    // }
    // if (node->left != NULL)
    // {
        // BinaryTreeNode* nodeToMove = node->left;
        // while (nodeToMove->right != NULL)
        // {
            // nodeToMove = nodeToMove->right;
        // }
        // nodeToMove->parent->right = NULL;
        // nodeToMove->parent = node->parent;
        // nodeToMove->left = node->left;
        // nodeToMove->right = node->right;
        // nodeToMove->left->parent = nodeToMove;
        // nodeToMove->right->parent = nodeToMove;
        // int _leftOrRightChild(node)
        // if ( < 0)
        // {
            // node->parent->left = nodeToMove;
        // }
    // }
    // else
    // {
    // }
// }

// int _leftOrRightChild(BinaryTreeNode* node)
// {
    // if (node->parent == NULL)
    // {
        // return 0;
    // }
    // else if (node->parent->left == node)
    // {
        // return -1;
    // }
    // else if (node->parent->right == node)
    // {
        // return 1;
    // }
    // else
    // {
        // return 0;
    // }
// }

BOOL BinaryTree_isEmpty(BinaryTree* self)
{
    smug_assert(_invariant(self));
    return self->root == NULL;
}

void BinaryTree_removeAll(BinaryTree* self)
{
    smug_assert(_invariant(self));
    if (!BinaryTree_isEmpty(self))
    {
        _deleteWithChildren(self->root);
        self->root = NULL;
        smug_assert(_invariant(self));
    }
}

void BinaryTree_deleteAll(BinaryTree* self, void (*deleter)(void* item))
{
    smug_assert(_invariant(self));
    if (!BinaryTree_isEmpty(self))
    {
        _deleteContentsWithChildren(self->root, deleter);
        self->root = NULL;
        smug_assert(_invariant(self));
    }
}

BinaryTreeIterator* BinaryTree_getIterator(BinaryTree* self)
{
    BinaryTreeIterator* newIter = allocate(BinaryTreeIterator);
    newIter->current = self->root;
    while (newIter->current->left != NULL)
    {
        newIter->current = newIter->current->left;
    }
}

void* BinaryTreeIterator_getNext(BinaryTreeIterator* self)
{
    void* item = self->current->item;
    // We have already traversed all the left children of the node. Find the
    // lowest node in the right subtree, if there is one.
    if (self->current->right != NULL)
    {
        self->current = self->current->right;
        while (self->current->left != NULL)
        {
            self->current = self->current->left;
        }
    }
    else
    {
        // If there is no right subtree, move up the parent chain until we get
        // to the root or a left-child node.
        while (_isLeftOrRightChild(self->current) == 1)
        {
            self->current = self->current->parent;
        }
        if (_isLeftOrRightChild(self->current) == -1)
        {
            self->current = self->current->parent;
        }
        else
        {
            self->current = NULL; // No more elements.
        }
    }
    return item;
}

BOOL BinaryTreeIterator_hasMore(BinaryTreeIterator* self)
{
    return self->current != NULL;
}

void BinaryTreeIterator_delete(BinaryTreeIterator* self)
{
    free(self);
}
