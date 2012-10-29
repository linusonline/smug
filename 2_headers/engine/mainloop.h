/**
 * @file blocking_engine.h
 * @brief Header file for a blocking version of the engine module
 * @if doc_files
 * @ingroup smug_engine
 * @endif
 */

/**
 * @addtogroup smug_engine
 * @{
 */

#ifndef SMUG_ENGINE_MAINLOOP_H
#define SMUG_ENGINE_MAINLOOP_H

typedef void (*LogicCallback)(void);

void Mainloop_run(void);
void Mainloop_exit();

void Mainloop_setLogicCallback(LogicCallback callback);
void Mainloop_setLogicFps(float fps);
float Mainloop_getLogicFps();

#endif /* SMUG_ENGINE_MAINLOOP_H */

/**@}*/
