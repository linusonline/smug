#include <math.h>
#include <GL/glfw.h>
#include <common.h>
#include <utils/log.h>
#include <utils/linkedlist.h>
#include <graphics/sprite.h>

#include <graphics/spriteanimation.h>

typedef struct _SpriteAnimation
{
    LinkedList* sprites;
    LinkedList* durations;
    TIME starttime;
    BOOL started;
} _SpriteAnimation;

static void _deleteInt(void* anInt)
{
    free((int*)anInt);
}

static TIME* _allocTime(TIME init)
{
    TIME* i = allocate(TIME);
    *i = init;
    return i;
}

static TIME _sumDurations(LinkedList* durations)
{
    smug_assert(durations != NULL);
    TIME* duration = (TIME*)LinkedList_getFirst(durations);
    TIME sum = 0;
    while (duration != NULL)
    {
        sum += *duration;
        duration = (TIME*)LinkedList_getNext(durations);
    }
    return sum;
}

static TIME _timeMod(TIME time1, TIME time2)
{
    return time1 - (floor(time1 / time2) * time2);
}

SpriteAnimation* SpriteAnimation_newEmpty()
{
    SpriteAnimation* newAnimation = allocate(SpriteAnimation);
    newAnimation->starttime = 0;
    newAnimation->started = FALSE;
    newAnimation->sprites = LinkedList_new();
    newAnimation->durations = LinkedList_new();
    return newAnimation;
}

void SpriteAnimation_addFrame(SpriteAnimation* self, Sprite* sprite, TIME duration)
{
    smug_assert(sprite != NULL);
    LinkedList_addLast(self->sprites, sprite);
    LinkedList_addLast(self->durations, _allocTime(duration));
}

void SpriteAnimation_start(SpriteAnimation* self)
{
    self->starttime = glfwGetTime();
    self->started = TRUE;
}

void SpriteAnimation_startAt(SpriteAnimation* self, TIME time)
{
    self->starttime = glfwGetTime() - time;
    self->started = TRUE;
}

Sprite* SpriteAnimation_getCurrentSprite(SpriteAnimation* self)
{
    if (!self->started || LinkedList_isEmpty(self->durations))
    {
        return NULL;
    }
    TIME diff = glfwGetTime() - self->starttime;
    diff = _timeMod(diff, _sumDurations(self->durations));
    Sprite* sprite = (Sprite*)LinkedList_getFirst(self->sprites);
    TIME* duration = (TIME*)LinkedList_getFirst(self->durations);
    diff -= *duration;
    while (diff > 0)
    {
        duration = (TIME*)LinkedList_getNext(self->durations);
        sprite = (Sprite*)LinkedList_getNext(self->sprites);
        smug_assert(sprite != NULL && duration != NULL);
        diff -= *duration;
    }
    return sprite;
}

void SpriteAnimation_delete(SpriteAnimation* self)
{
    LinkedList_delete(self->sprites);
    LinkedList_deleteContents(self->durations, _deleteInt);
    LinkedList_delete(self->durations);
    free(self);
}
