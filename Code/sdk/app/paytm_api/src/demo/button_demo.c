#include <stdlib.h>
#include <stdio.h>

#include "paytm_audio_api.h"
#include "paytm_button_api.h"

static int task_id = 0;

void buttoncb(void * p)
{
    int32_t vol = Paytm_GetVolume(NULL);
    buttonActMsg_t *msg = (buttonActMsg_t *)p; 
    RTI_LOG1("This is buttoncb, action is ");
    switch (msg->id)
    {
    case BUTTON_PLUS:
    	Paytm_StopPlay();
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
    	Paytm_StopPlay();
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
         Paytm_StopPlay();
        Paytm_PlayFileFromDir(LOC_EXTER_MEM, "resources/sounds/en", "Received.amr,10000.amr,and.amr,num90.amr", vol);
       
        // Paytm_PlayFile(LOC_EXTER_MEM, "test/292KB.mp3",vol );
        Paytm_TRACE("Audio test");
        break;
    case BUTTON_POWER:
        if(msg->state == STATE_BUTTON_LONG_PRESS){
            Paytm_PlayFile(LOC_EXTER_MEM, "resources/sounds/en/DI_powOff.amr",vol);
            osiSysPoweroff();
        }
        // Paytm_PlayFile(LOC_EXTER_MEM, "test/BatCrtkl.wav",vol );
        // Paytm_StopPlay();
        
    default:
        break;
    }
}
void ButtonTest(void)
{
    button_action_callback_register(buttoncb);

    Paytm_Button_events(true);
}

void pwkCb(void* p)
{
    if(*(int*)p == STATE_BUTTON_SINGLE_CLICK){
        Paytm_TRACE("Pwk single press");
        Paytm_SendMessage_From_ISR(task_id, 1, 55, 129);
    }else if(*(int*)p == STATE_BUTTON_LONG_PRESS){
        Paytm_TRACE("Pwk long press");
        Paytm_SendMessage_From_ISR(task_id, 2, 47, 266);
    }
}


static void msgTask(void* p)
{
    ST_MSG msg = {0};

    while (1)
    {
        if(Paytm_GetMessage(task_id, &msg) == 0)
        {
            Paytm_TRACE("Recv msg [%d %d %d]", msg.message, msg.param1, msg.param2);
        }

        Paytm_delayMilliSeconds(100);
    }
    
}

void pwkDemo(void)
{
    Paytm_Set_PowerKey_LongPress_Time(2000);
    powerkey_action_callback_register(pwkCb);


    task_id = Paytm_CreateTask("1", msgTask, NULL, 120, 1 * 1024);
}