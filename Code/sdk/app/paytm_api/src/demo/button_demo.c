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
            Paytm_PlayFile(LOC_EXTER_MEM, "resources/sounds/en/maxvol.amr",vol );
        }else{
            Paytm_PlayFile(LOC_EXTER_MEM, "resources/sounds/en/minvol.amr",vol );
        }
        Paytm_TRACE("Volume up");
        // appAudioTipsPushQue(0, "音量加", 0, false);
        break;
    case BUTTON_MINUS:
        vol--;
        if(vol <= PAYTM_VOLUME_MIN){
            vol = PAYTM_VOLUME_MIN;
            Paytm_PlayFile(LOC_EXTER_MEM, "resources/sounds/en/minvol.amr",vol );
        }else{
            Paytm_PlayFile(LOC_EXTER_MEM, "resources/sounds/en/minvol.amr",vol );
        }
        Paytm_TRACE("Volume down");
        break;
    case BUTTON_FUNCTION:
        if(msg->state == STATE_BUTTON_SINGLE_CLICK){
            Paytm_TRACE("Fun key single press");
        }else if(msg->state == STATE_BUTTON_DOUBLE_CLICK){
            Paytm_TRACE("Fun key double press");
        }else{
            Paytm_TRACE("Fun key long press");
        }
        Paytm_PlayFileFromDir(LOC_EXTER_MEM, "resources/sounds/en", "Received.amr,10000.amr,and.amr,num90.amr", vol);
        //Paytm_PlayFile(LOC_INTER_MEM, "D:/test/Freq_sweep_61_0dB.mp3",20 );
        Paytm_TRACE("Audio test");
        break;
    case BUTTON_POWER:
        if(msg->state == STATE_BUTTON_LONG_PRESS){
            Paytm_PlayFile(LOC_EXTER_MEM, "resources/sounds/en/DI_powOff.amr",vol);
            osiSysPoweroff();
        }
        
    default:
        break;
    }
}
void ButtonTest(void)
{
    button_action_callback_register(buttoncb);

    Paytm_Button_events(true);
}