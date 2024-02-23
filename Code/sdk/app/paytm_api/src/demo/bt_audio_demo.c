#include "paytm_audio_api.h"
#include "paytm_bt_audio_api.h"
#include "paytm_button_api.h"
#include "paytm_led_api.h"
#include "paytm_dev_info_api.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef BT_EN
int task_id = 0;

static char volume = 16;

void bt_button_cb(void * p)
{
    static bool switch_0 = false;
    // int32_t rc = 0;
    buttonActMsg_t *msg = (buttonActMsg_t *)p; 

    switch (msg->id)
    {
    case BUTTON_PLUS:
        Paytm_TRACE("BT volume up");
        // Paytm_BT_Audio_Set_Cmd(BT_CMD_VOLUME_UP, NULL, 0);
        volume++;
        if(volume > 32){
            volume = 32;
        }
        Paytm_BT_Set_volume(volume);
        break;
    case BUTTON_MINUS:
        Paytm_TRACE("BT volume down");
        // Paytm_BT_Audio_Set_Cmd(BT_CMD_VOLUME_DOWN, NULL, 0);
        volume--;
        if(volume < 0){
            volume = 0;
        }
        Paytm_BT_Set_volume(volume);
        break;
    case BUTTON_FUNCTION:
        if(msg->state == STATE_BUTTON_SINGLE_CLICK){
            Paytm_TRACE("BT switch to next song");
            // Paytm_BT_Audio_Set_Cmd(BT_CMD_NEXT_SONG, NULL, 0);
        }else if(msg->state == STATE_BUTTON_DOUBLE_CLICK){
            Paytm_TRACE("BT switch to previous song");
            // Paytm_BT_Audio_Set_Cmd(BT_CMD_PREVIOUS_SONG, NULL, 0);
        }else if(msg->state == STATE_BUTTON_LONG_PRESS){
            if(switch_0){
                switch_0 = false;
                Paytm_Set_Battery_Led(0, 1, 0);

                Paytm_TRACE("BT powerdown");
                Paytm_BT_Powerdown();
                Paytm_delayMilliSeconds(1000);
                Paytm_Set_Battery_Led(0, 0, 1);
            }else{
                switch_0 = true;
                Paytm_Set_Battery_Led(0, 1, 1);

                Paytm_TRACE("BT reset");
                Paytm_BT_Powerup();
                Paytm_delayMilliSeconds(2000);
                Paytm_Set_Battery_Led(0, 0, 1);
            }
        }
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
        Paytm_SendMessage_From_ISR(task_id, 2, 55, 129);
    }
}

static void PWKMsgTask(void* p)
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
                // Paytm_BT_Audio_Set_Cmd(BT_CMD_PAUSE_PLAY, NULL, 0);
                // appAudioPaEn(false, true);
                Paytm_BT_Audio_Enable(false);
                Paytm_PlayFileSync(LOC_INTER_MEM, "welc.mp3", 7);
                Paytm_BT_Audio_Enable(true);
                // Paytm_BT_Audio_Set_Cmd(BT_CMD_PAUSE_PLAY, NULL, 0);
                osiThreadSleepMs(300);
                // appAudioPaEn(true, true); 
            }else{
                // appAudioPaEn(false, true); 
                osiSysReset();
            }
        }
        Paytm_delayMilliSeconds(100);
    }
}

static void prvBtEventCB(void *param){
    bluetooth_device_msg_t *msg = (bluetooth_device_msg_t *)param;
    Paytm_TRACE("State: %d", msg->state);
}
void bt_audio_demo(void)
{
    uint8 bt_sw_version[32+1] = {0};
    Paytm_BT_Callback_Register(prvBtEventCB);
    if(Paytm_BT_Audio_Init() != 0)
    {
        Paytm_TRACE("BT audio init fail!");
        return;
    }

    Paytm_BT_GetSDKVersion(bt_sw_version, sizeof(bt_sw_version)-1);
    Paytm_TRACE("BT module software version: %s", bt_sw_version);
    
    // Paytm_BT_Accept_Connection(NULL);
    char bt_name[32] = {0x00};
    char bt_read_name[32] = {0x00};
    uint8_t bt_imei[32] = {0x00};
    int ret = 0;
    uint32 get_len = 0;

    ret = Paytm_BT_Set_Timeout(10, 60, 0); Paytm_TRACE("Paytm_BT_Set_Timeout ret: %d", ret);
    Paytm_LED_SetColor(LED_GREEN, 1);
    
    button_action_callback_register(bt_button_cb);
    powerkey_action_callback_register(bt_pwk_cb);
    task_id = Paytm_CreateTask("1", PWKMsgTask, NULL, 120, 2 * 1024);

    Paytm_Button_events(true);

    Paytm_TRACE("BT audio init success!");
    
    Paytm_GetIMEI((char *)bt_imei);        
    sprintf(bt_name, "Paytm_SoundBox_%d%d%d%d", bt_imei[11] - '0', bt_imei[12] - '0', bt_imei[13] - '0', bt_imei[14] - '0');
    bt_name[19] = '\0';
    ret = Paytm_BT_Get_Name(bt_read_name, &get_len);  Paytm_TRACE("Paytm_BT_Get_Name ret: %d, name_len = %d", ret, get_len);

    if(Paytm_strncmp(bt_name, bt_read_name, Paytm_strlen((char*)bt_name)) != 0){
        ret = Paytm_BT_Set_Name(bt_name); Paytm_TRACE("Paytm_BT_Set_Name ret: %d", ret);
    }
    // Paytm_BT_Get_PairedBT_DeviceList();
    Paytm_delayMilliSeconds(2000);
    // Paytm_BT_Powerdown();

    while (1)
    {
        Paytm_delayMilliSeconds(2000);
    }
    
    Paytm_ExitTask();
}   
#endif