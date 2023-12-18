#include <stdlib.h>
#include <stdio.h>

#include "paytm_audio_api.h"
#include "paytm_button_api.h"
#include "paytm_dev_api.h"

static int task_id = 0;

void buttoncb(void * p)
{
    int32_t vol = Paytm_GetVolume(NULL);
    int32_t rc = 0;
    char vol_file[32] = {0};
    buttonActMsg_t *msg = (buttonActMsg_t *)p; 
    RTI_LOG1("This is buttoncb, action is ");
    switch (msg->id)
    {
    case BUTTON_PLUS:
    	// Paytm_StopPlay();
        vol++;
        if(vol >= PAYTM_VOLUME_MAX){
            vol = PAYTM_VOLUME_MAX;
            sprintf(vol_file, "test/num%d.amr", vol);
            Paytm_SetAudioVolume(vol);
            // Paytm_PlayFile(LOC_EXTER_MEM, (char*)vol_file, vol);
        }else{
            Paytm_SetAudioVolume(vol);
            // Paytm_PlayFile(LOC_EXTER_MEM, (char*)vol_file, vol);
        }
        Paytm_TRACE("Volume up to vol %d", vol);
        Paytm_PlayFile(LOC_INTER_MEM, "and.amr", vol);
        // appAudioTipsPushQue(0, "音量加", 0, false);
        break;
    case BUTTON_MINUS:

        vol--;
        if(vol <= PAYTM_VOLUME_MIN){
            vol = PAYTM_VOLUME_MIN;
            sprintf(vol_file, "test/num%d.amr", vol);
            Paytm_SetAudioVolume(vol);
            // Paytm_PlayFile(LOC_EXTER_MEM, (char*)vol_file, vol);
        }else{
            Paytm_SetAudioVolume(vol);
            // Paytm_PlayFile(LOC_EXTER_MEM, (char*)vol_file, vol);
        }
        Paytm_TRACE("Volume down to vol %d", vol);
        Paytm_PlayFile(LOC_INTER_MEM, "and.amr", vol);
        break;
    case BUTTON_FUNCTION:
        Paytm_StopPlay();
        if(msg->state == STATE_BUTTON_SINGLE_CLICK){
            rc = Paytm_PlayFile(LOC_EXTER_MEM, "test/21.mp3", vol);
            Paytm_TRACE("Fun key single press, play rc = %d", rc);
        }else if(msg->state == STATE_BUTTON_DOUBLE_CLICK){
            rc = Paytm_PlayFile(LOC_EXTER_MEM, "test/21.mp3", vol);
            Paytm_TRACE("Fun key double press, play rc = %d", rc);
        }else{
            Paytm_TRACE("Fun key long press");
        }
        // Paytm_PlayFile(LOC_EXTER_MEM, "test/292KB.mp3",vol );
        Paytm_TRACE("Audio test vol %d", vol);
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

void buttoncb0(void * p)
{
    int32_t vol = Paytm_GetVolume(NULL);
    buttonActMsg_t *msg = (buttonActMsg_t *)p; 
    RTI_LOG1("This is buttoncb, action is ");
    switch (msg->id)
    {
    case BUTTON_PLUS:
        Paytm_TRACE("Volume up");
        Paytm_PlayFile(LOC_INTER_MEM, "maxvol.amr", 9);
        break;
    case BUTTON_MINUS:
        Paytm_TRACE("Volume down");
        Paytm_PlayFile(LOC_INTER_MEM, "minvol.amr", 9);
        break;
    case BUTTON_FUNCTION:
        if(msg->state == STATE_BUTTON_SINGLE_CLICK){
            Paytm_TRACE("Fun key single press");
        }else if(msg->state == STATE_BUTTON_DOUBLE_CLICK){
            Paytm_TRACE("Fun key double press");
        }else{
            Paytm_TRACE("Fun key long press");
        }
        Paytm_PlayFile(LOC_INTER_MEM, "and.amr", 9);
        break;
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
            if(msg.message == 2){
                Paytm_Reset(0);
            }
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