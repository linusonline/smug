#ifndef SMUG_UTILS_TIMER_H
#define SMUG_UTILS_TIMER_H

struct _Timer;
typedef struct _Timer Timer;

Timer* Timer_new(void(*callback)(void*), void* argument, TIME delay);
Timer* Timer_newRepeated(void(*callback)(void*), void* argument, TIME delay, TIME interval);
void Timer_delete(Timer* self);
void Timer_deleteVoid(void* self);
void Timer_start(Timer* self, TIME currentTime);
BOOL Timer_check(Timer* self, TIME currentTime); // Check if it's time to make the callback.
void Timer_call(Timer* self, TIME currentTime);
BOOL Timer_isFinished(Timer* self);

#endif /* SMUG_UTILS_TIMER_H */
