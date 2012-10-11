#include <stdlib.h>

#include <common.h>

#include <utils/linkedlist_type.h>
#include <utils/linkedlist.h>

/**
 * A struct for the node type in a linked list.
 * Create new nodes with Node_new, delete them with Node_destroy. Set the content of the node by setting Node->item directly.
 * @sa ::LinkedList
 */
typedef struct _Node {
    void* item;         /**< Pointer to the actual data in the node. */
    Node* next;     /**< Next node in list. Null if end of list. */
    Node* prev;     /**< Previous node in list. Null if start of list */
} _Node;

/**
 * A struct for a linked list.
 *
 * Navigate the list by getting the Node->first pointer, and then using ->next on it until you get null.
 *
 * @sa ::Node
 */
typedef struct _LinkedList {
    Node* first;         /**< First node in list. */
    Node* last;         /**< Last node in list */
    int length;
    Node* current;
} _LinkedList;

static Node* Node_new()
{
    Node* node = allocate(Node);

    node->item = NULL;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

static void Node_delete(Node* node)
{
    free(node);
}

static void _clear(LinkedList* self)
{
    if (self->length == 0)
    {
        return;
    }
    for (Node* node = self->first->next; node != NULL; node = node->next)
    {
        // Start checking the second node in the list; delete the node before it.
        // When we get to NULL, only the last node in the list is left.
        Node_delete(node->prev);
    }
    Node_delete(self->last);
    self->length = 0;
    self->first = NULL;
    self->last = NULL;
    self->current = NULL;

    // Alternative implementation
/*     Node* node = list->first;
    Node* next_node;

    while(NULL != node)
    {
        next_node = node->next;
        Node_delete(node);
        node = next_node;
    }

    list->first = NULL;
    list->last = NULL;
    list->length = 0; */
}

static BOOL _invariant(LinkedList* self)
{
    BOOL ret = (self != NULL &&
            (   (self->first == NULL &&
                self->last == NULL)
                ||
                (self->first != NULL &&
                self->last != NULL &&
                self->first->prev == NULL &&
                self->last->next == NULL)
            )
        );
    return ret;
}

LinkedList* LinkedList_new()
{
    LinkedList* new_list;

    new_list = allocate(LinkedList);

    new_list->first = NULL;
    new_list->last = NULL;
    new_list->length = 0;
    new_list->current = NULL;

    smug_assert(_invariant(new_list));

    return new_list;
}

void LinkedList_delete(LinkedList* self)
{
    smug_assert(_invariant(self));

    _clear(self);
    free(self);
}

void LinkedList_addLast(LinkedList* self, void* item)
{
    smug_assert(_invariant(self));

    Node* node = Node_new();

    if (self->last)
    {
        node->prev = self->last;
        self->last->next = node;
    }
    else
    {
        self->first = node;
    }

    self->last = node;
    node->item = item;
    self->length++;
}

void LinkedList_addFirst(LinkedList* self, void* item)
{
    smug_assert(_invariant(self));

    Node* node = Node_new();

    if (self->first)
    {
        node->next = self->first;
        self->first->prev = node;
    }
    else
    {
        self->last = node;
    }

    self->first = node;
    node->item = item;
    self->length++;
}

void LinkedList_insertAfter(LinkedList* self, Node* node, void* item)
{
    smug_assert(_invariant(self));

    Node* newNode = Node_new();
    newNode->item = item;
    newNode->next = node->next;
    newNode->prev = node;
    node->next = newNode;
}

void LinkedList_insertBefore(LinkedList* self, Node* node, void* item)
{
    smug_assert(_invariant(self));

    Node* newNode = Node_new();
    newNode->item = item;
    newNode->next = node;
    newNode->prev = node->prev;
    node->prev = newNode;
}

void* LinkedList_getLast(LinkedList* self)
{
    smug_assert(_invariant(self));
    return LinkedList_isEmpty(self) ? NULL : self->last->item;
}

void* LinkedList_popLast(LinkedList* self)
{
    smug_assert(_invariant(self));
    if (LinkedList_isEmpty(self))
    {
        return NULL;
    }
    void* data = self->last->item;
    LinkedList_remove(self, self->last);
    return data;
}

void* LinkedList_getFirst(LinkedList* self)
{
    smug_assert(_invariant(self));
    if (LinkedList_isEmpty(self))
    {
        return NULL;
    }
    else
    {
        self->current = self->first;
        return self->first->item;
    }
}

void* LinkedList_getNext(LinkedList* self)
{
    smug_assert(_invariant(self));
    if (self->current == NULL)
    {
        return NULL;
    }
    self->current = self->current->next;
    return self->current == NULL ? NULL : self->current->item;
}

void* LinkedList_popFirst(LinkedList* self)
{
    smug_assert(_invariant(self));
    if (LinkedList_isEmpty(self))
    {
        return NULL;
    }
    void* data = self->first->item;
    LinkedList_remove(self, self->first);
    return data;
}

BOOL LinkedList_isEmpty(LinkedList* self)
{
    smug_assert(_invariant(self));

    return (NULL == self->first && NULL == self->last);
}

int LinkedList_length(LinkedList* self)
{
    smug_assert(_invariant(self));

    return self->length;
}

void LinkedList_remove(LinkedList* self, Node* node)
{
    smug_assert(_invariant(self));

    if (node == self->first)
    {
        self->first = node->next;
    }
    else
    {
        node->prev->next = node->next;
    }

    if (node == self->last)
    {
        self->last = node->prev;
    }
    else
    {
        node->next->prev = node->prev;
    }

    Node_delete(node);
    self->length--;
    self->current = NULL;
}

void LinkedList_removeAll(LinkedList* self)
{
    smug_assert(_invariant(self));
    _clear(self);
    smug_assert(_invariant(self));
}

BOOL LinkedList_removeItem(LinkedList* self, void* item)
{
    smug_assert(_invariant(self));

    Node* node = self->first;
    while(NULL != node)
    {
        if (node->item == item)
            {
                LinkedList_remove(self, node);
                return TRUE;
            }
        else
            {
                node = node->next;
            }
    }
    return FALSE;
}

void LinkedList_doList(LinkedList* self, void (*func)(void*))
{
    smug_assert(_invariant(self));

    Node* node = self->first;
    while(NULL != node)
    {
        func(node->item);
        node = node->next;
    }
}

void LinkedList_doListIf(LinkedList* self, void (*func)(void*), BOOL(*pred)(void*))
{
    smug_assert(_invariant(self));

    Node* node = self->first;
    while(NULL != node)
    {
        if (pred(node->item))
        {
            func(node->item);
        }
        node = node->next;
    }
}

LinkedList* LinkedList_getThose(LinkedList* self, BOOL(*pred)(void*))
{
    smug_assert(_invariant(self));

    LinkedList* newList = LinkedList_new();
    Node* node = self->first;
    while (NULL != node)
    {
        if (pred(node->item))
        {
            LinkedList_addLast(newList, node->item);
        }
        node = node->next;
    }
    return newList;
}

LinkedList* LinkedList_map(LinkedList* self, void* (*func)(void*))
{
    smug_assert(_invariant(self));

    LinkedList* newList = LinkedList_new();
    Node* node = self->first;
    while (NULL != node)
    {
        LinkedList_addLast(newList, func(node->item));
        node = node->next;
    }
    return newList;
}

BOOL LinkedList_forAll(LinkedList* self, BOOL(*pred)(void*))
{
    smug_assert(_invariant(self));

    Node* node = self->first;
    while(NULL != node)
    {
        if (!pred(node->item))
        {
            return FALSE;
        }
        node = node->next;
    }
    return TRUE;
}

BOOL LinkedList_exists(LinkedList* self, BOOL(*pred)(void*))
{
    smug_assert(_invariant(self));

    Node* node = self->first;
    while(NULL != node)
    {
        if (pred(node->item))
        {
            return TRUE;
        }
        node = node->next;
    }
    return FALSE;
}

static void* pointerToCompare;
static BOOL _pointerCompare(void* ptr)
{
    return pointerToCompare == ptr;
}

BOOL LinkedList_elementExists(LinkedList* self, void* elem)
{
    smug_assert(_invariant(self));
    pointerToCompare = elem;
    return LinkedList_exists(self, _pointerCompare);
}

void LinkedList_concat(LinkedList* self, LinkedList* other)
{
    // TODO: A bit dangerous. What should one do with 'other' after this?
    smug_assert(_invariant(self));
    smug_assert(_invariant(other));
    self->last->next = other->first;
    other->first->prev = self->last;
}

void LinkedList_interleave(LinkedList* self, void* item, void* (*itemCopier)(void*))
{
    Node* iter;

    smug_assert(_invariant(self));

    iter = self->first;
    iter = iter->next;

    while (NULL != iter)
    {
        LinkedList_insertBefore(self, iter, itemCopier(item));
        iter = iter->next;
    }
}

LinkedList* LinkedList_deepCopy(LinkedList* self, void* (*itemCopier)(void*))
{
    LinkedList* newList;
    Node* iter;

    smug_assert(_invariant(self));

    newList = LinkedList_new();
    iter = self->first;

    while (NULL != iter)
    {
        LinkedList_addLast(newList, itemCopier(iter->item));
        iter = iter->next;
    }
    return newList;
}

void LinkedList_deleteContents(LinkedList* self, void (*deleter)(void*))
{
    smug_assert(_invariant(self));
    LinkedList_doList(self, deleter);
    _clear(self);
}
