#include <GL/glfw.h>

#include <common.h>
#include <utils/log.h>
#include <graphics/graphics.h>
#include <graphics/renderqueue.h>
#include <engine/engine.h>

#include <engine/mainloop.h>

static BOOL fpsSet = FALSE;
static TIME logicDelay = 0.0;
static TIME SMALLEST_SLEEP_TIME = 1.0/500;

static TIME discreteTime;

static BOOL keepRunning = TRUE;

static LogicCallback logicCallback = NULL;

static TIME _getTime()
{
    return glfwGetTime();
}

static TIME _getDiscreteTime()
{
    return discreteTime;
}

static void _stepDiscreteTime()
{
    discreteTime = _getTime();
}

void Mainloop_exit()
{
    keepRunning = FALSE;
}

void Mainloop_setLogicCallback(LogicCallback callback)
{
    logicCallback = callback;
}

void Mainloop_run()
{
    smug_assert(Engine_isInitialized());
    smug_assert(Graphics_isInitialized());

    NOTIFY("Running engine...");
    Log_indent();

    // Setup timing variables
    TIME lastFpsCheck = _getTime();
    int fps = 0;
    if (!fpsSet)
    {
        Mainloop_setLogicFps(30.0f);
    }
    TIME nextLogicTick;
    TIME time;
    nextLogicTick = _getTime();

    RenderQueue* renderQueue = RenderQueue_new();

    while (keepRunning)
    {
        time = _getTime();

        if (time >= nextLogicTick)
        {
            // This is the "logic" part of the loop, running at its own FPS.
            nextLogicTick += logicDelay;
            _stepDiscreteTime();
            if (logicCallback != NULL)
            {
                logicCallback();
            }
        }
        Engine_drawObjects(renderQueue);
        Graphics_render(renderQueue);
        Graphics_refreshWindow();
        RenderQueue_clear(renderQueue);

        fps++;

        if (_getTime() - lastFpsCheck >= 1.0)
        {
            NOTIFY("Fps: %i", fps);
            fps = 0;
            lastFpsCheck = _getTime();
        }

        // So far, we let the graphics run at the same fps as the logic.
        if (nextLogicTick - time > SMALLEST_SLEEP_TIME)
        {
            glfwSleep(nextLogicTick - time);
        }
    }

    RenderQueue_delete(renderQueue);
    Log_dedent();
    NOTIFY("Engine stopped.");
}

void Mainloop_setLogicFps(float fps)
{
    logicDelay = 1.0 / fps;
    fpsSet = TRUE;
}
