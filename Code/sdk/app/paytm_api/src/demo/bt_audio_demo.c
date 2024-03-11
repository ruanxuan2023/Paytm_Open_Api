#include "paytm_audio_api.h"
#include "paytm_bt_audio_api.h"
#include "paytm_button_api.h"
#include "paytm_led_api.h"
#include "paytm_dev_info_api.h"

#include <stdlib.h>
#include <stdio.h>
#include <osi_api.h>

int task_id = 0;

static char volume = 16;

static void bt_audio_power_up(void);
static void bt_audio_power_down(void);
static uint8_t bt_power_state = 0;


void bt_button_cb(void * p)
{
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
        if(bt_power_state == 0){
            break;
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
        if(bt_power_state == 0){
            break;
        }
        Paytm_BT_Set_volume(volume);
        break;
    case BUTTON_PLUS_MINUS:
        Paytm_TRACE("BT SWITCH");
        if(bt_power_state){
            bt_audio_power_down();
        }else{
            bt_audio_power_up();
        }
        break;
    case BUTTON_FUNCTION:
        if(msg->state == STATE_BUTTON_SINGLE_CLICK){
            Paytm_TRACE("BT switch to next song");
            // Paytm_BT_Audio_Set_Cmd(BT_CMD_NEXT_SONG, NULL, 0);
        }else if(msg->state == STATE_BUTTON_DOUBLE_CLICK){
            Paytm_TRACE("BT switch to previous song");
            // Paytm_BT_Audio_Set_Cmd(BT_CMD_PREVIOUS_SONG, NULL, 0);
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
    uint8_t *mac_addr = msg->mac;
    Paytm_TRACE("State: %d", msg->state);
    switch (msg->state)
    {
    case SB_BT_DEVICE_CONNECTION_CONNECTED:
        // accepte device connect
        Paytm_BT_Accept_Connection(mac_addr);
        break;
    case SB_BT_DEVICE_CONNECTION_PAIRED:
        break;
    default:
        break;
    }
}

static void bt_audio_power_up(void){
    uint8_t mac[MAC_ADDR_LEN] = {0};
    uint8 bt_sw_version[32+1] = {0};

    Paytm_TRACE("BT audio POWER up");

    if(bt_power_state == 1){
        return;
    }

    Paytm_BT_Callback_Register(prvBtEventCB);

    Paytm_BT_Powerup();
    
    Paytm_delayMilliSeconds(1000);
    Paytm_BT_GetSDKVersion(bt_sw_version, sizeof(bt_sw_version));
    Paytm_TRACE("BT firmware version: %s", bt_sw_version);
    
    // Paytm_BT_Accept_Connection(NULL);
    char bt_name[32] = {0x00};
    char bt_read_name[32] = {0x00};
    char bt_imei[32] = {0x00};
    
    int ret = 0;
    uint32 get_len = 0;

    ret = Paytm_BT_Set_Timeout(10, 60, 0); Paytm_TRACE("Paytm_BT_Set_Timeout ret: %d", ret);
    Paytm_LED_SetColor(LED_GREEN, 1);

    Paytm_TRACE("BT audio init success!");
    
    Paytm_GetIMEI(bt_imei);        
    sprintf(bt_name, "Paytm_SoundBox_%d%d%d%d", bt_imei[11] - '0', bt_imei[12] - '0', bt_imei[13] - '0', bt_imei[14] - '0');
    bt_name[19] = '\0';
    ret = Paytm_BT_Get_Name(bt_read_name, &get_len);  Paytm_TRACE("Paytm_BT_Get_Name ret: %d, name_len = %d", ret, get_len);

    if(Paytm_strncmp(bt_name, bt_read_name, Paytm_strlen((char*)bt_name)) != 0){
        ret = Paytm_BT_Set_Name(bt_name); Paytm_TRACE("Paytm_BT_Set_Name ret: %d", ret);
    }

    Paytm_BT_Set_volume(Paytm_BT_Get_Volume());
    ret = Paytm_BT_Get_Mac_Address(mac);

    if (ret != 0)
    {
        Paytm_TRACE("Read mac address ERROR");
    } else {
        Paytm_TRACE_HEX_BUFFER("BT Mac address:", mac, MAC_ADDR_LEN);
    }
    bt_power_state = 1;
    Paytm_delayMilliSeconds(2000);
    // Paytm_BT_Powerdown();

    bt_paired_device_t *plist = NULL;
    plist = Paytm_BT_Get_PairedBT_DeviceList();
        
    if(plist){
        bt_paired_device_t *pcur = plist;
        while (pcur)
        {
            Paytm_TRACE("name: %s lastconnect: %ld", pcur->name, pcur->last_connected);
            Paytm_TRACE_HEX_BUFFER("List mac:", pcur->mac, 6);
            pcur = pcur->next;
        }
    }else{
        RTI_LOG("Get paired list ERROR!!!!!!!");
    }
        
    Paytm_Free_DeviceList(plist);
    return;
}


static void bt_audio_power_down(void){
    Paytm_TRACE("BT audio POWER down");
    if(bt_power_state ==  0){
        return;
    }
    Paytm_BT_Callback_Register(NULL);
    Paytm_BT_Powerdown();
    bt_power_state = 0;
}

void bt_audio_demo(void)
{
    button_action_callback_register(bt_button_cb);
    powerkey_action_callback_register(bt_pwk_cb);
    task_id = Paytm_CreateTask("1", PWKMsgTask, NULL, 120, 2 * 1024);
    Paytm_Button_events(true);

    if(Paytm_BT_Audio_Init() != 0)
    {
        Paytm_TRACE("BT audio init fail!");
    }
}   
