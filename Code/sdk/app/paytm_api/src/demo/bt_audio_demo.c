#include "paytm_audio_api.h"
#include "paytm_bt_audio_api.h"
#include "paytm_button_api.h"
#include "paytm_led_api.h"
#include "paytm_dev_info_api.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <osi_api.h>


int task_id = 0;

static char volume = 16;
static uint8 mac_paire[MAC_ADDR_LEN] = {0};
static uint8_t connect_mac[MAC_ADDR_LEN] = {0};

static void bt_audio_power_up(void);
static void bt_audio_power_down(void);

void bt_button_cb(void * p)
{
    uint8_t mac[MAC_ADDR_LEN] = {0};
    uint64_t last_connect = 0;
    buttonActMsg_t *msg = (buttonActMsg_t *)p;
    
    uint8_t test_mac[MAC_ADDR_LEN] = {0};

    switch (msg->id)
    {
    case BUTTON_PLUS:
        Paytm_TRACE("Button plus");
        bool is_paired = Paytm_BT_device_Is_paired(connect_mac);
        Paytm_TRACE_HEX_BUFFER("Connect mac", connect_mac, MAC_ADDR_LEN);
        Paytm_TRACE("call Paytm_BT_device_Is_paired check connect_mac  is %d", is_paired);
        test_mac[3] = 0;
        is_paired = Paytm_BT_device_Is_paired(test_mac);
        Paytm_TRACE("call Paytm_BT_device_Is_paired check test_mac  is %d", is_paired);


        Paytm_BT_Audio_Enable(false);
        int ret = Paytm_PlayFileFromDir(LOC_EXTER_MEM, "data/resources/sounds/hi/","welc.amr", 8);
        Paytm_TRACE("Play file ret %d", ret);
        // audio asynchronous playback delay waiting for playback to complete
        Paytm_delayMilliSeconds(3000);
        Paytm_BT_Audio_Enable(true);
        break;
    case BUTTON_MINUS:
        Paytm_TRACE("BT volume down");
        // Paytm_BT_Audio_Set_Cmd(BT_CMD_VOLUME_DOWN, NULL, 0);
        volume--;
        if(volume < 0){
            volume = 0;
        }
        if(Paytm_BT_Get_State() == 0){
            break;
        }
        Paytm_BT_Set_volume(volume);

        //1. get paired list
        bt_paired_device_t *plist = NULL;
        plist = Paytm_BT_Get_PairedBT_DeviceList();

        if(plist){
            bt_paired_device_t *pcur = plist;
            last_connect = pcur->last_connected;
            memcpy(mac, pcur->mac, MAC_ADDR_LEN);

            //2. get last paired device
            while (pcur)
            {
                Paytm_TRACE("name: %s lastconnect: %ld", pcur->name, pcur->last_connected);
                Paytm_TRACE_HEX_BUFFER("List mac:", pcur->mac, 6);
                if(last_connect > pcur->last_connected){
                    last_connect = pcur->last_connected;
                    memcpy(mac, pcur->mac, MAC_ADDR_LEN);
                }
                pcur = pcur->next;
            }
            Paytm_Free_DeviceList(plist);
        }else{
            Paytm_TRACE("Get paired list empty!!!!!!!");
            break;
        }

        //3. print paired device list
        ret = Paytm_BT_Remove_Paired_device(mac);
        Paytm_TRACE("Paytm_BT_Remove_Paired_device ret: %d", ret);

        //4. print paired device list
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
            Paytm_TRACE("Get paired list empty!!!!!!!");
        }
        Paytm_Free_DeviceList(plist);
        break;
    case BUTTON_PLUS_MINUS:
        Paytm_TRACE("BT SWITCH");
        if(Paytm_BT_Get_State() == 1){
            Paytm_TRACE("BT current state is ON, turn off BT");
            bt_audio_power_down();
            Paytm_TRACE("bt_audio_power_down done");
        }else{
            Paytm_TRACE("BT current state is OFF, turn on BT");
            bt_audio_power_up();
            Paytm_TRACE("bt_audio_power_up done");
        }
        break;
    case BUTTON_FUNCTION:
        if(msg->state == STATE_BUTTON_SINGLE_CLICK){
            if(mac_paire[0]){
                BtErrCode_e ret = Paytm_BT_Accept_Connection(mac_paire);
                Paytm_TRACE("Paytm_BT_Accept_Connection ret: %d", ret);
                memset(mac_paire, 0, MAC_ADDR_LEN);
                break;
            }
            Paytm_TRACE("clean paried device");

            ret = Paytm_BT_Clear_Paired_Device_List();
            Paytm_TRACE("Paytm_BT_Clear_Paired_Device_List ret: %d", ret);
            //print paired device list after clean paird list

            plist = Paytm_BT_Get_PairedBT_DeviceList();
            if(plist){
                bt_paired_device_t *pcur = plist;
                while (pcur)
                {
                    Paytm_TRACE("name: %s lastconnect: %ld", pcur->name, pcur->last_connected);
                    Paytm_TRACE_HEX_BUFFER("List mac:", pcur->mac, 6);
                    pcur = pcur->next;
                }
            } else {
                Paytm_TRACE("Get paired list empty!!!!!!!");
            }
            Paytm_Free_DeviceList(plist);
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
    Paytm_TRACE("State: %d name: %s", msg->state, msg->name);
    switch (msg->state)
    {
    case SB_BT_DEVICE_CONNECTION_CONNECTED:
        // accepte device connect
        Paytm_TRACE("Try connect unpaired device name: %s", msg->name);
        Paytm_TRACE("Press FUN key accept connect");
        memcpy(mac_paire, mac_addr, MAC_ADDR_LEN);
        // BtErrCode_e ret = Paytm_BT_Accept_Connection(mac_paire);
        // Paytm_TRACE("Paytm_BT_Accept_Connection ret: %d", ret);
        break;
    case SB_BT_DEVICE_CONNECTION_PAIRED:
        Paytm_TRACE("Connected paired device name: %s", msg->name);
        memcpy(connect_mac, mac_addr, MAC_ADDR_LEN);
        Paytm_LED_SetColor(LED_BLUE, 0);
        break;
    case SB_BT_DEVICE_CONNECTION_DISCONNECTED:
        Paytm_LED_SetColor(LED_BLUE, 1);
        break;
    case SB_BT_DEVICE_CONNECTION_TIMEOUT_IDLE:
        Paytm_TRACE("Idle connect timeout %s", msg->name);
        break;
    case SB_BT_DEVICE_CONNECTION_TIMEOUT_NO_ACCEPT:
        Paytm_TRACE("Accept connect timeout %s", msg->name);
        break;
    case SB_BT_DEVICE_CONNECTION_TIMEOUT_PLAYBACK:
        Paytm_TRACE("Playback timeout");
        break;
    default:
        break;
    }
}

static void bt_audio_power_up(void){
    uint8_t mac[MAC_ADDR_LEN] = {0};
    uint8 bt_sw_version[32+1] = {0};
    char bt_name[32] = {0x00};
    char bt_read_name[32] = {0x00};
    char bt_imei[32] = {0x00};
    
    int ret = 0;
    uint32 get_len = 0;

    Paytm_TRACE("BT audio POWER up");

    if(Paytm_BT_Get_State() == 1){
        Paytm_TRACE("BT alread power ON  ,return ");
        return;
    }

    Paytm_LED_SetColor(LED_BLUE, 1);
    Paytm_BT_Callback_Register(prvBtEventCB);


    Paytm_BT_Powerup();
    
    Paytm_delayMilliSeconds(1000);


    ret = Paytm_BT_GetSDKVersion(bt_sw_version, sizeof(bt_sw_version));
    Paytm_TRACE("BT firmware version ret: %d : %s",ret, bt_sw_version);

    ret = Paytm_BT_Set_Timeout(60, 10, 10); 
    Paytm_TRACE("Paytm_BT_Set_Timeout ret: %d", ret);
    

    Paytm_TRACE("BT audio init success!");
    // disable BLE
    Paytm_BT_BLE_Enable(false);
    
    Paytm_GetIMEI(bt_imei);        
    sprintf(bt_name, "Paytm_SoundBox_%d%d%d%d", bt_imei[11] - '0', bt_imei[12] - '0', bt_imei[13] - '0', bt_imei[14] - '0');
    bt_name[19] = '\0';
    ret = Paytm_BT_Get_Name(bt_read_name, &get_len);  Paytm_TRACE("Paytm_BT_Get_Name ret: %d, name_len = %d name: %s", ret, get_len, bt_read_name);

    if(Paytm_strncmp(bt_name, bt_read_name, Paytm_strlen((char*)bt_name)) != 0){
        ret = Paytm_BT_Set_Name(bt_name); Paytm_TRACE("Paytm_BT_Set_Name ret: %d", ret);
    }

    // Paytm_BT_Set_volume(Paytm_BT_Get_Volume());
    ret = Paytm_BT_Get_Mac_Address(mac);
    if (ret != 0)
    {
        Paytm_TRACE("Read mac address ERROR");
    } else {
        Paytm_TRACE_HEX_BUFFER("BT Mac address:", mac, MAC_ADDR_LEN);
    }

    Paytm_delayMilliSeconds(2000);
    // Paytm_BT_Powerdown();
    bt_paired_device_t *plist = NULL;
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
    if(Paytm_BT_Get_State() ==  0){
        return;
    }
    Paytm_BT_Callback_Register(NULL);
    Paytm_BT_Powerdown();
    Paytm_LED_SetColor(LED_GREEN, 1);
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
