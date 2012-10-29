/*===============================================================================================
 PlaySound Example
 Copyright (c), Firelight Technologies Pty, Ltd 2004-2011.

 Modified by Linus Lindholm

 This example shows how to simply load and play multiple sounds.  This is about the simplest
 use of FMOD.
 This makes FMOD decode the into memory when it loads.  If the sounds are big and possibly take
 up a lot of ram, then it would be better to use the FMOD_CREATESTREAM flag so that it is
 streamed in realtime as it plays.
===============================================================================================*/
#include <stdio.h>

#define __CYGWIN32__
#include <fmod.h>
#include <fmod_errors.h>
#undef __CYGWIN32__

#include <allsmug.h>

#define ERRCHECK(result) \
do \
{ \
    if (result != FMOD_OK) \
    { \
        ERROR("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result)); \
        exit(-1); \
    } \
} while (0)

FMOD_SYSTEM      *fmodSystem;
FMOD_SOUND       *sound1, *sound2, *sound3;
FMOD_CHANNEL     *channel = 0;
FMOD_RESULT       result;
int               key;
unsigned int      version;

Controller* theController;

static void _logicCallback()
{
    FMOD_System_Update(fmodSystem);

    unsigned int ms = 0;
    unsigned int lenms = 0;
    int          playing = 0;
    int          paused = 0;
    int          channelsplaying = 0;

    if (channel)
    {
        FMOD_SOUND *currentsound = 0;

        result = FMOD_Channel_IsPlaying(channel, &playing);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }

        result = FMOD_Channel_GetPaused(channel, &paused);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }

        result = FMOD_Channel_GetPosition(channel, &ms, FMOD_TIMEUNIT_MS);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }

        FMOD_Channel_GetCurrentSound(channel, &currentsound);
        if (currentsound)
        {
            result = FMOD_Sound_GetLength(currentsound, &lenms, FMOD_TIMEUNIT_MS);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
            {
                ERRCHECK(result);
            }
        }
    }

    result = FMOD_Sound_GetLength(sound1, &lenms, FMOD_TIMEUNIT_MS);
    if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
    {
        ERRCHECK(result);
    }

    FMOD_System_GetChannelsPlaying(fmodSystem, &channelsplaying);

    printf("Time %02d:%02d:%02d/%02d:%02d:%02d : %s : Channels Playing %2d\r", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100, paused ? "Paused " : playing ? "Playing" : "Stopped", channelsplaying);
}

static void _buttonCallback(Controller* controller, int buttonIndex, int state)
{
    smug_assert(controller == theController);
    if (state == SMUG_KEY_RELEASE)
    {
        return;
    }
    switch (buttonIndex)
    {
        case 0:
            result = FMOD_System_PlaySound(fmodSystem, FMOD_CHANNEL_FREE, sound1, 0, &channel);
            ERRCHECK(result);
            break;
        case 1:
            result = FMOD_System_PlaySound(fmodSystem, FMOD_CHANNEL_FREE, sound2, 0, &channel);
            ERRCHECK(result);
            break;
        case 2:
            result = FMOD_System_PlaySound(fmodSystem, FMOD_CHANNEL_FREE, sound3, 0, &channel);
            ERRCHECK(result);
            break;
        case 3:
            Mainloop_exit();
            break;
    }
}

int main(int argc, char *argv[])
{
    Log_initStdOut();
    Graphics_initialize(640, 480, FALSE);

    Input_initialize();
    theController = Controller_new(0, 4, 0);
    Input_setButtonCallbackForController(theController, _buttonCallback);
    Input_linkControllerToKeyboardKey(theController, 0, '1');
    Input_linkControllerToKeyboardKey(theController, 1, '2');
    Input_linkControllerToKeyboardKey(theController, 2, '3');
    Input_linkControllerToKeyboardKey(theController, 3, 'X');

    Engine_init();
    Mainloop_setLogicCallback(_logicCallback);

    /*
        Create a System object and initialize.
    */
    result = FMOD_System_Create(&fmodSystem);
    ERRCHECK(result);

    result = FMOD_System_GetVersion(fmodSystem, &version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }

    result = FMOD_System_Init(fmodSystem, 32, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(fmodSystem, "5_res/audio/media/drumloop.wav", FMOD_HARDWARE, 0, &sound1);
    ERRCHECK(result);

    result = FMOD_Sound_SetMode(sound1, FMOD_LOOP_OFF); /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
    ERRCHECK(result);                                   /* so turn it off here.  We could have also just put FMOD_LOOP_OFF in the above CreateSound call. */

    result = FMOD_System_CreateSound(fmodSystem, "5_res/audio/media/jaguar.wav", FMOD_SOFTWARE, 0, &sound2);
    ERRCHECK(result);

    result = FMOD_System_CreateSound(fmodSystem, "5_res/audio/media/swish.wav", FMOD_HARDWARE, 0, &sound3);
    ERRCHECK(result);

    printf("===================================================================\n");
    printf("PlaySound Example.  Copyright (c) Firelight Technologies 2004-2011.\n");
    printf("===================================================================\n");
    printf("\n");
    printf("Press '1' to play a mono sound using hardware mixing\n");
    printf("Press '2' to play a mono sound using software mixing\n");
    printf("Press '3' to play a stereo sound using hardware mixing\n");
    printf("Press 'Esc' to quit\n");
    printf("\n");

    Mainloop_run();

    /*
        Shut down
    */
    result = FMOD_Sound_Release(sound1);
    ERRCHECK(result);
    result = FMOD_Sound_Release(sound2);
    ERRCHECK(result);
    result = FMOD_Sound_Release(sound3);
    ERRCHECK(result);
    result = FMOD_System_Close(fmodSystem);
    ERRCHECK(result);
    result = FMOD_System_Release(fmodSystem);
    ERRCHECK(result);

    return 0;
}