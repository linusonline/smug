#ifndef SMUG_ENGINE_BODY_H
#define SMUG_ENGINE_BODY_H

#include <common.h>
#include <utils/binarytree.h>

typedef struct Body
{
    float width;
    float height;
    float x;
    float y;
    BinaryTree* tags;
} Body;

Body* Body_newRectangle(float width, float height);
void Body_delete(Body* self);
void Body_setPosition(Body* self, float x, float y);
float Body_getX(Body* self);
float Body_getY(Body* self);
float Body_getWidth(Body* self);
float Body_getHeight(Body* self);
void Body_addTag(Body* self, int tag);
void Body_removeAllTags(Body* self);
BOOL Body_hasTag(Body* self, int tag);

BOOL Body_colliding(Body* b1, Body* b2);

#endif /* SMUG_ENGINE_BODY_H */
