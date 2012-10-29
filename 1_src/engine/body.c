#include <common.h>

#include <engine/body.h>
#include <utils/binarytree.h>
#include <utils/map.h>

static int* allocInt(int n)
{
    int* i = allocate(int);
    *i = n;
    return i;
}

static void deleteInt(void* n)
{
    free((int*)n);
}

Body* Body_newRectangle(float width, float height)
{
    Body* newBody = allocate(Body);
    newBody->width = width;
    newBody->height = height;
    newBody->x = 0;
    newBody->y = 0;
    newBody->tags = BinaryTree_new(Map_compareInts);
    return newBody;
}

void Body_delete(Body* self)
{
    BinaryTree_deleteAll(self->tags, deleteInt);
    BinaryTree_delete(self->tags);
    free(self);
}

void Body_setPosition(Body* self, float x, float y)
{
    self->x = x;
    self->y = y;
}

float Body_getX(Body* self)
{
    return self->x;
}

float Body_getY(Body* self)
{
    return self->y;
}

float Body_getWidth(Body* self)
{
    return self->width;
}

float Body_getHeight(Body* self)
{
    return self->height;
}

void Body_addTag(Body* self, int tag)
{
    if (BinaryTree_find(self->tags, &tag) == NULL)
    {
        BinaryTree_insert(self->tags, allocInt(tag));
    }
}

void Body_removeAllTags(Body* self)
{
    BinaryTree_deleteAll(self->tags, deleteInt);
}

BOOL Body_hasTag(Body* self, int tag)
{
    return BinaryTree_find(self->tags, &tag) != NULL;
}

BOOL Body_colliding(Body* b1, Body* b2)
{
    // return ( ((b2->x > b1->x &&
               // b2->x < b1->x + b1->width) ||
              // (b2->x + b2->width > b1->x &&
               // b2->x + b2->width < b1->x + b1->width)) &&
             // ((b2->y > b1->y &&
               // b2->y < b1->y + b1->height) ||
              // (b2->y + b2->height > b1->y &&
               // b2->y + b2->height < b1->y + b1->height)) );
    return b1->x + b1->width > b2->x &&
           b1->x < b2->x + b2->width &&
           b1->y + b1->height > b2->y &&
           b1->y < b2->y + b2->height;
}
