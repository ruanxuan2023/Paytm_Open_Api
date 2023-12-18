#include "paytm_audio_api.h"
#include "paytm_bt_audio_api.h"
#include "paytm_button_api.h"
#include "paytm_led_api.h"

#include <stdlib.h>
#include <stdio.h>

int task_id = 0;

void bt_button_cb(void * p)
{
    int32_t rc = 0;
    buttonActMsg_t *msg = (buttonActMsg_t *)p; 

    switch (msg->id)
    {
    case BUTTON_PLUS:
        Paytm_TRACE("BT volume up");
        Paytm_BT_Audio_Set_Cmd(BT_VOLUME_UP);
        break;
    case BUTTON_MINUS:
        Paytm_TRACE("BT volume down");
        Paytm_BT_Audio_Set_Cmd(BT_VOLUME_DOWN);
        break;
    case BUTTON_FUNCTION:
        if(msg->state == STATE_BUTTON_SINGLE_CLICK){
            Paytm_TRACE("BT switch to next song");
            Paytm_BT_Audio_Set_Cmd(BT_NEXT_SONG);
        }else if(msg->state == STATE_BUTTON_DOUBLE_CLICK){
            Paytm_TRACE("BT switch to previous song");
            Paytm_BT_Audio_Set_Cmd(BT_PREVIOUS_SONG);
        }
        break;
    case BUTTON_POWER:
        Paytm_TRACE("Power key press");
        break;
        
    default:
        break;
    }
}

void bt_pwk_cb(void* p)
{
    if(*(int*)p == STATE_BUTTON_SINGLE_CLICK){
        Paytm_SendMessage_From_ISR(task_id, 1, 55, 129);
    }else if(*(int*)p == STATE_BUTTON_LONG_PRESS){
        Paytm_TRACE("Pwk long press");

    }
}

static void msgTask(void* p)
{
    ST_MSG msg = {0};
    static bool stop_flag = false;

    while (1)
    {
        if(Paytm_GetMessage(task_id, &msg) == 0)
        {
            Paytm_TRACE("Recv msg [%d %d %d]", msg.message, msg.param1, msg.param2);
            if(msg.message == 1){
                stop_flag = !stop_flag;
                Paytm_TRACE("Pwk single press, %s play audio", stop_flag ? "stop" : "restart");
                Paytm_BT_Audio_Set_Cmd(BT_PAUSE_PLAY);
            }
        }

        Paytm_delayMilliSeconds(100);
    }
    
}

void bt_audio_demo(void)
{
    if(Paytm_BT_Audio_Init() != 0)
    {
        Paytm_TRACE("BT audio init fail!");
        return;
    }
    Paytm_LED_SetColor(LED_GREEN, 1);
    Paytm_PlayFile(LOC_INTER_MEM, "welc.amr", 8);

    Paytm_delayMilliSeconds(5 * 1000);
    if(!Paytm_BT_Audio_Enable(true)){
        Paytm_TRACE("BT audio switch to bt fail!");
    }
    
    button_action_callback_register(bt_button_cb);
    powerkey_action_callback_register(bt_pwk_cb);
    task_id = Paytm_CreateTask("1", msgTask, NULL, 120, 2 * 1024);

    Paytm_Button_events(true);

    Paytm_TRACE("BT audio init success!");
}   