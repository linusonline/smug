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
        self->compare != NULL &&
        (self->root == NULL ||
        (self->root->parent == NULL && _invariantRec(self->root)));
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
        comp = self->compare(item, current->item);
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

BinaryTree* BinaryTree_new(int (*compare)(void*, void*))
{
    smug_assert(compare != NULL);
    BinaryTree* newTree = allocate(BinaryTree);
    newTree->root = NULL;
    newTree->compare = compare;
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
        comp = self->compare(item, current->item);
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
