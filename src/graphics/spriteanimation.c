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
    TIME pausedAt;
    TIME cachedDuration;
    BOOL cachedDurationCurrent;
    BOOL loop;
    SpriteAnimationStopCallback stopCallback;
    void* callbackData;
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

static TIME _sumDurations(SpriteAnimation* self)
{
    if (self->cachedDurationCurrent)
    {
        return self->cachedDuration;
    }
    smug_assert(self->durations != NULL);
    TIME* duration = (TIME*)LinkedList_getFirst(self->durations);
    TIME sum = 0;
    while (duration != NULL)
    {
        sum += *duration;
        duration = (TIME*)LinkedList_getNext(self->durations);
    }
    self->cachedDuration = sum;
    self->cachedDurationCurrent = TRUE;
    return sum;
}

static TIME _timeMod(TIME time1, TIME time2)
{
    return time1 - (floor(time1 / time2) * time2);
}

static TIME _getCurrentTimeDiff(SpriteAnimation* self)
{
    TIME diff = glfwGetTime() - self->starttime;
    if (!self->loop && diff > _sumDurations(self))
    {
        self->pausedAt = _sumDurations(self);
        self->started = FALSE;
        self->stopCallback(self, self->callbackData);
        return self->pausedAt;
    }
    return _timeMod(diff, _sumDurations(self));
}

/* timeDiff must not be longer than sum of frame durations. */
static Sprite* _getSpriteForTimeDiff(SpriteAnimation* self, TIME timeDiff)
{
    if (LinkedList_isEmpty(self->durations))
    {
        WARNING("Got Sprite from empty SpriteAnimation!");
        return NULL;
    }
    Sprite* sprite = (Sprite*)LinkedList_getFirst(self->sprites);
    smug_assert(sprite != NULL);
    TIME* duration = (TIME*)LinkedList_getFirst(self->durations);
    timeDiff -= *duration;
    while (timeDiff > 0)
    {
        duration = (TIME*)LinkedList_getNext(self->durations);
        sprite = (Sprite*)LinkedList_getNext(self->sprites);
        smug_assert(sprite != NULL && duration != NULL);
        timeDiff -= *duration;
    }
    return sprite;
}

SpriteAnimation* SpriteAnimation_newEmpty()
{
    SpriteAnimation* newAnimation = allocate(SpriteAnimation);
    newAnimation->starttime = 0;
    newAnimation->started = FALSE;
    newAnimation->sprites = LinkedList_new();
    newAnimation->durations = LinkedList_new();
    newAnimation->pausedAt = 0;
    newAnimation->cachedDuration = 0;
    newAnimation->cachedDurationCurrent = FALSE;
    newAnimation->loop = TRUE;
    newAnimation->stopCallback = NULL;
    newAnimation->callbackData = NULL;
    return newAnimation;
}

void SpriteAnimation_addFrame(SpriteAnimation* self, Sprite* sprite, TIME duration)
{
    smug_assert(sprite != NULL);
    LinkedList_addLast(self->sprites, sprite);
    LinkedList_addLast(self->durations, _allocTime(duration));
    self->cachedDurationCurrent = FALSE;
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

void SpriteAnimation_doLoop(SpriteAnimation* self, BOOL loop)
{
    self->loop = loop;
}

void SpriteAnimation_pause(SpriteAnimation* self)
{
    smug_assert(self->started);
    self->pausedAt = _getCurrentTimeDiff(self);
    self->started = FALSE;
    LOG(LOG_ANIMATION, "Paused at time diff: %f", self->pausedAt);
}

void SpriteAnimation_reset(SpriteAnimation* self)
{
    self->starttime = glfwGetTime();
    self->pausedAt = 0.0;
    LOG(LOG_ANIMATION, "Reset animation.");
}

Sprite* SpriteAnimation_getCurrentSprite(SpriteAnimation* self)
{
    if (self->started)
    {
        Sprite* s = _getSpriteForTimeDiff(self, _getCurrentTimeDiff(self));
        return s;
    }
    else
    {
        Sprite* s = _getSpriteForTimeDiff(self, self->pausedAt);
        return s;
    }
}

Sprite* SpriteAnimation_getSpriteAtIndex(SpriteAnimation* self, int index)
{
    smug_assert(index >= 0 && index < LinkedList_length(self->sprites));
    Sprite* sprite = (Sprite*)LinkedList_getFirst(self->sprites);
    while (index > 0)
    {
        sprite = (Sprite*)LinkedList_getNext(self->sprites);
        index--;
    }
    return sprite;
}

void SpriteAnimation_delete(SpriteAnimation* self)
{
    LinkedList_delete(self->sprites);
    LinkedList_doList(self->durations, _deleteInt);
    LinkedList_delete(self->durations);
    free(self);
}

void SpriteAnimation_setStopCallback(SpriteAnimation* self, SpriteAnimationStopCallback callback, void* anything)
{
    self->stopCallback = callback;
    self->callbackData = anything;
}
