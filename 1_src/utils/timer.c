#include <common.h>
#include <engine/engine.h>
#include <utils/log.h>

#include <utils/timer.h>

struct _Timer
{
    void(*callback)(void*);
    void* argument;
    TIME delay;
    TIME interval;
    TIME started;
    TIME lastCallback;
};

static Timer* _new(void(*callback)(void*), void* argument, TIME delay, TIME interval)
{
    Timer* newTimer = allocate(Timer);
    newTimer->callback = callback;
    newTimer->argument = argument;
    newTimer->delay = delay;
    newTimer->interval = interval;
    newTimer->started = -1.0;
    newTimer->lastCallback = -1.0;
    return newTimer;
}

Timer* Timer_new(void(*callback)(void*), void* argument, TIME delay)
{
    return _new(callback, argument, delay, -1.0);
}

Timer* Timer_newRepeated(void(*callback)(void*), void* argument, TIME delay, TIME interval)
{
    if (interval <= 0)
    {
        ERROR("Bad time interval.");
        return NULL;
    }
    return _new(callback, argument, delay, interval);
}

void Timer_delete(Timer* self)
{
    free(self);
}

void Timer_deleteVoid(void* self)
{
    Timer_delete((Timer*)self);
}

void Timer_start(Timer* self, TIME currentTime)
{
    self->started = currentTime;
}

BOOL Timer_check(Timer* self, TIME currentTime)
{
    smug_assert(self->started > 0);

    if (self->lastCallback > 0)
    {
        // A repeated timer which has already been run once.
        return currentTime >= self->lastCallback + self->interval;
    }
    else
    {
        // A timer that has not yet been run
        return currentTime >= self->started + self->delay;
    }
}

void Timer_call(Timer* self, TIME currentTime)
{
    if (self->interval < 0)
    {
        // One-shot timer should stop.
        self->started = -1.0;
    }
    else
    {
        // Repeated timer should continue.
        self->lastCallback = currentTime;
    }
    self->callback(self->argument);
}

BOOL Timer_isFinished(Timer* self)
{
    return self->started < 0.0;
}

