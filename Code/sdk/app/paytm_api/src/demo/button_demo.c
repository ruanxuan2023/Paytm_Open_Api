#include <stdlib.h>
#include <stdio.h>

#include "paytm_audio_api.h"
#include "paytm_button_api.h"

void buttoncb(void * p)
{
    int32_t vol = Paytm_GetVolume(NULL);
    buttonActMsg_t *msg = (buttonActMsg_t *)p; 
    RTI_LOG1("This is buttoncb, action is ");
    switch (msg->id)
    {
    case BUTTON_PLUS:
        vol++;
        if(vol >= PAYTM_VOLUME_MAX){
            vol = PAYTM_VOLUME_MAX;
            //Paytm_PlayFile(LOC_INTER_MEM, "D:/test/vol_max.wav",vol );
        }else{
            //Paytm_PlayFile(LOC_INTER_MEM, "D:/test/vol_up.wav",vol );
        }
        Paytm_TRACE("Volume up");
        // appAudioTipsPushQue(0, "音量加", 0, false);
        break;
    case BUTTON_MINUS:
        vol--;
        if(vol <= PAYTM_VOLUME_MIN){
            vol = PAYTM_VOLUME_MIN;
            //Paytm_PlayFile(LOC_INTER_MEM, "D:/test/vol_min.wav",vol );
        }else{
            //Paytm_PlayFile(LOC_INTER_MEM, "D:/test/vol_down.wav",vol );
        }
        Paytm_TRACE("Volume down");
        break;
    case BUTTON_FUNCTION:
        /* code */
        RTI_LOG("mp3 play test");
        //Paytm_PlayFile(LOC_INTER_MEM, "D:/test/Freq_sweep_61_0dB.mp3",20 );
        Paytm_TRACE("Audio test");
        break;
    case BUTTON_POWER:
        if(msg->state == STATE_BUTTON_LONG_PRESS){
            Paytm_PlayFile(LOC_INTER_MEM, "D:/test/power_off.wav",vol);
        }
        osiSysPoweroff();
    default:
        break;
    }
}
void ButtonTest(void)
{
    button_action_callback_register(buttoncb);

    Paytm_Button_events(true);
}