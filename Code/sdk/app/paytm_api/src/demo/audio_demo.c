#include "paytm_audio_api.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "osi_api.h"

void audio_cb(void* p)
{
    urcEvent_e event = (urcEvent_e)p;
    switch (event)
    {
    case URC_AUDIO_PLAY_STARTED:
        Paytm_TRACE("Audio play started!");
        break;
    case URC_AUDIO_PLAY_STOPPED:
        Paytm_TRACE("Audio play stopped!");
        break;
    case URC_AUDIO_PLAY_COMPLETED:
        Paytm_TRACE("Audio play completed!");
        break;
    case URC_AUDIO_PLAY_ERROR:
        Paytm_TRACE("Audio play error!");
        break;
    case URC_AUDIO_PLAY_SYSTEM_ERR_STR:
        Paytm_TRACE("Audio play system error str!");
        break;
    default:
        break;
    }
}

void audioPlayDemo(void)
{
    Paytm_SetAudioVolume(6);

    playback_state_callback_register(audio_cb);

    Paytm_PlayFile(LOC_EXTER_MEM, "test.wav", 4);

    Paytm_PlayFileFromDir(LOC_EXTER_MEM, "D:\\audios\\", "1k_0db.mp3", 8);
}
