#include "paytm_audio_api.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "osi_api.h"

void audio_cb(void* p)
{
    Paytm_TRACE("Audio callback!");
}

void audioPlayDemo(void)
{
    Paytm_SetAudioVolume(6);

    playback_state_callback_register(audio_cb);

    Paytm_PlayFile(LOC_EXTER_MEM, "test.wav", 4);

    Paytm_PlayFileFromDir(LOC_EXTER_MEM, "D:\\audios\\", "1k_0db.mp3", 8);
}
