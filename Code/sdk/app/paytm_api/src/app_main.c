#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "mbedtls/ssl.h"
#include "drv_adc.h"

#include "paytm_button_api.h"
#include "paytm_http_api.h"
#include "paytm_mqtt_api.h"
#include "paytm_net_api.h"
#include "paytm_sys_api.h"
#include "paytm_file_api.h"
#include "paytm_sim_api.h"
#include "paytm_debug_uart_api.h"
#include "paytm_led_api.h"
#include "paytm_audio_api.h"
#include "paytm_tls_verify.h"



void LogTest(void)
{
    uint8_t data[10] = {'a', '0', '8', 99, 46, 13};

    Paytm_Logs_Enable(false);

    Paytm_TRACE_BUFFER_WRITE(data, 10, 1);

    Paytm_TRACE_TAG_DATA("Demo2:", data, 10);

    Paytm_TRACE_HEX_BUFFER("DemoHex:", data, 10);

    Paytm_TRACE("This id is %d in %s", 10, "China");

    Paytm_TRACE_FUNC("Demo7", "appSendData:", "%s.%d", "No", 1);
    
    Paytm_TRACE_DATETIME("L1", "Demo8", "appTex:", "get %d", 10010);

    Paytm_TRACE_DATETIME_PAYTM("L7", "2023-09-23", "Demo9", "%d", 456);
}

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
            Paytm_PlayFile(LOC_INTER_MEM, "D:/test/vol_max.wav",vol );
        }else{
            Paytm_PlayFile(LOC_INTER_MEM, "D:/test/vol_up.wav",vol );
        }
        
        // appAudioTipsPushQue(0, "音量加", 0, false);
        break;
    case BUTTON_MINUS:
        vol--;
        if(vol <= PAYTM_VOLUME_MIN){
            vol = PAYTM_VOLUME_MIN;
            Paytm_PlayFile(LOC_INTER_MEM, "D:/test/vol_min.wav",vol );
        }else{
            Paytm_PlayFile(LOC_INTER_MEM, "D:/test/vol_down.wav",vol );
        }
        break;
    case BUTTON_FUNCTION:
        /* code */
        RTI_LOG("mp3 play test");
        Paytm_PlayFile(LOC_INTER_MEM, "D:/test/Freq_sweep_61_0dB.mp3",20 );
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

static void net_connect(void)
{
    int32 stat = 0;

    Paytm_GPRS_Connect(Paytm__IPVERSION_IPV4, NULL);
    
    while (!(Paytm_GetGPRSState(&stat) == 1 || Paytm_GetGPRSState(&stat) == 5))
    {
        Paytm_delayMilliSeconds(1000);
    }
    
    Paytm_TRACE("Network connected!");
}

typedef enum
{
	WM_AUDIO_PLAY			   =  4,  /*AUDIO PLAY by other threads only */
	WM_AUDIO_STOP			   =  5,  /*AUDIO STOP by other threads only*/
	WM_SPEAKER_CHECK		   =  6,  /*Check speaker quality*/
	WM_BATTERY_CHECK		   =  7,  /*Check BATT. CHARGE STATUS*/
	WM_SYSTEM_CHECK		       =  8,  /*Check CPU USAGE , HEAP RAM AND STACK RAM*/
	WM_NETWORK_RECONNECT       =  9,
	WM_NETWORK_DISCONNECT      =  10, /*N/W DISCONNECT*/
	WM_PDP_INACTIVE            =  11, /*PDP NOT ACTIVE*/
    WM_FILE_DOWNLOAD           = 18,  /*UI call to test this Fnc*/
	WM_APP_DOWNLOAD            = 19,  /*UI call to test this Fnc*/
	WM_HTTP_TEST               = 20,  /*UI call to test this Fnc*/
	WM_FILE_TEST               = 21,  /*File system test*/
	WM_UNZIP_TEST              = 22,  /*TEST FILE UNZIP*/
	WM_TEST_SENDMSG_MEMORY     = 23,  /*TEST SEND MESSAGEQ MEMORY AFTER LOOPS*/
	WM_READ_TASK_STATUS        = 24,  /*READ TASK RESOURCE STATUS*/
	WM_MQTT_INIT               = 25,  /*mqtt task init         UI call to test this Fnc*/
	WM_MQTT_CONNECT            = 26,  /*mqtt connect to server UI call to test this Fnc*/
	WM_READ_HIGH_RES_TIMER     = 27,  /*READ HI RES TIMER AND TIME ZONE*/
	WM_READ_SIM_STATE          = 28,  /*READ SIM STATE MANUALLY*/
	WM_READ_CHARGER_STATUS     = 29,  /*READ CHARGER PLUGGED STATE*/
	WM_GET_SIM_INFO            = 30,  /*READ DATA RELATED TO SIM*/
	WM_GET_PDP_INFO            = 31,  /*READ DATA RELATED TO PDP*/
    WM_GET_NW_INFO             = 32,  /*READ DATA RELATED TO NETWORK*/
	WM_PLAY_VOL_SOUND          = 33,  /*PLAY VOL UP DN SOUND*/
    WM_NVR_TESTING             = 34,  /*NV RAM TESTING */
    WM_FILE_TRANSFER           = 35,  /*FILES TRANSFER FORM C TO D */
    WM_READ_APN                = 36,  /*READ CURRENT APN */
	TEST_FILE_EXIST            = 37,  /*check if a file exist*/
	TEST_APP_SIGN_VERIFY       = 38,  /*CALCULATE HASH FORM customer_app.bin file*/
	TEST_FORMAT_DRIVE          = 40,  /*FORMATS THE D DRIVE , ONLY FOR D DRIVE*/
    WM_READ_FIRMWARE_BUILD_TIME= 41,  /*READ FIRMWARE BUILT TIME*/	
    WM_RUN_ADD_FILE            = 42,  /*RUN ADD FILE LONG DUREATION*/
	TEST_APP_UPDATE_FILE       = 43,  /*TEST APP UPDATE FROM FILE,d:/customer_app.bin UNZIPPED FILE IN D DRIVE */
    TEST_MQTT_LOOP_QA          = 44,  /*MQTT LOOP TEST FOR MEM LEAK CHECK*/
    TEST_FS_LOOP_QA            = 45,  /*LED LOOP TEST FOR FS LEAK CHECK*/
    TEST_STATUS_READ_QA        = 46,  /*STATUS LOOP TEST FOR USB, SIM, BATT AND SPEAKER*/
    TEST_AUDIO_LOOP_QA         = 47,  /*STATUS LOOP TEST FOR AUDIO */
    TEST_HTTP_GET_QA           = 48,  /*STATUS LOOP TEST FOR HTTP GET */
    WM_READ_ISDN_COPS          = 49,  /*GET ISDN AND OPERATOR NAME*/
    WM_CLEAR_QUEUE             = 50,  /*CLEAR ALL MESSAGED IN A QUEUE*/
	WM_DFOTA_HTTP_DEMO         = 51,  /*DOWNLOAD DFOTA FROM HTTPS*/
	WM_SSL_DEMO                = 52,  /*SSL TEST*/
	WM_MBED_DEMO               = 53,  /*MBED TLS DEMO*/
	WM_OTA_TEST_QA             = 54,  /*OTA DEMO*/
	WM_ASYNC_INIT              = 55,
	WM_ASYNC_WRITE             = 57,
	WM_ASYNC_READ              = 58,
	WM_ASYNC_CLOSE             = 59,
	WM_SAMPLE_FILE_WRITE       = 60,
	WM_TIMER_CYCLIC_TEST       = 61,
	WM_MQTT_SUBSCRIBE          = 63,
	WM_STOP_WATCH_DOG_TIMER    = 64,
	WM_CALCULATE_SHA256        = 65,
	WM_TASK_TERMINATE          = 67,
	WM_TASK_RESUME             = 68,
	
	WM_DEVICE_CRASH_TEST_A     = 90,
	WM_DEVICE_CRASH_TEST_B     = 91,
	WM_DEVICE_CRASH_TEST_C     = 92,
}TASK_SELECTION;	

extern void sys_initialize(void);
extern void getTaobaoSuggest(void* p);
extern void testMqtt_india(void* p);
extern void testMqtt(void* p);
extern void PaytmHttpSSLPost(void * p);
extern void Mqtt_0(void* p);
extern void ntpDemo(void *p);
extern void batteryDemo(void);
extern void audioPlayDemo(void);
extern void AliMqttDemo(void* p);
extern void testNetWorkReconected(void);
extern void testNetWorkDisconected(void);
extern void testHttpGet(void * p);
extern void fileSystemDemo(void);
extern void tasksDemo(void);
extern void testSim(void);
extern void readSimState(void);
extern void readPDPInfo(void);
extern void readAPN(void);
extern void fileHeapLeakDemo(void* p);
extern void httpDownload(void* p);
void OpenDemoViaId(TASK_SELECTION id)
{
    switch (id)
    {
    case WM_AUDIO_PLAY:
        audioPlayDemo();
        break;
    case WM_AUDIO_STOP:
        break;
    case WM_SPEAKER_CHECK:
        break;
    case WM_BATTERY_CHECK:
        batteryDemo();
        break;
    case WM_SYSTEM_CHECK:
        devInfoDemo();
        break;
    case WM_NETWORK_RECONNECT:
        testNetWorkReconected();
        break;
    case WM_NETWORK_DISCONNECT:
        testNetWorkDisconected();
        break;
    case WM_PDP_INACTIVE:
        break;
    case WM_FILE_DOWNLOAD:
        break;
    case WM_APP_DOWNLOAD:
        break;
    case WM_HTTP_TEST:
        testHttpGet(NULL);
        break;
    case WM_FILE_TEST:
        fileSystemDemo();
        break;
    case WM_UNZIP_TEST:
        break;
    case WM_TEST_SENDMSG_MEMORY:
        tasksDemo();
        break;
    case WM_READ_TASK_STATUS:
        break;
    case WM_MQTT_INIT:
        break;
    case WM_MQTT_CONNECT:
        AliMqttDemo(NULL);
        break;
    case WM_READ_HIGH_RES_TIMER:
        break;
    case WM_READ_SIM_STATE:
        readSimState();
        break;
    case WM_READ_CHARGER_STATUS:
        batteryDemo();
        break;
    case WM_GET_SIM_INFO:
        testSim();
        break;
    case WM_GET_PDP_INFO:
        readPDPInfo();
        break;
    case WM_GET_NW_INFO:
        break;
    case WM_PLAY_VOL_SOUND:
        audioPlayDemo();
        break;
    case WM_NVR_TESTING:
        break;
    case WM_FILE_TRANSFER:
        break;
    case WM_READ_APN:
        readAPN();
        break;
    case TEST_FILE_EXIST:
        Paytm_fexists(LOC_INTER_MEM, "test.text");
        break;
    case TEST_APP_SIGN_VERIFY:
    {
        uint8_t digest[32];
        int ret = 0;
        ret = Paytm_Cal_File_Hash(LOC_INTER_MEM, "customer_app.bin", digest);
        if(ret == 0)
        {
            Paytm_TRACE_HEX_BUFFER("Digest", digest, sizeof(digest));
        }
    }
        break;
    case TEST_FORMAT_DRIVE:
        break;
    case WM_READ_FIRMWARE_BUILD_TIME:
        break;
    case WM_RUN_ADD_FILE:
        break;
    case TEST_APP_UPDATE_FILE:
        break;
    case TEST_MQTT_LOOP_QA:
        Paytm_CreateTask("mqtt", Mqtt_0, NULL, 110, 30 * 1024);
        break;
    case TEST_FS_LOOP_QA:
        Paytm_CreateTask("mqtt", fileHeapLeakDemo, NULL, 110, 30 * 1024);
        break;
    case TEST_STATUS_READ_QA:
        break;
    case TEST_AUDIO_LOOP_QA:
        break;
    case TEST_HTTP_GET_QA:
        break;
    case WM_READ_ISDN_COPS:
        break;
    case WM_CLEAR_QUEUE:
        break;
    case WM_DFOTA_HTTP_DEMO:
        break;
    case WM_SSL_DEMO:
        break;
    case WM_MBED_DEMO:
        break;
    case WM_OTA_TEST_QA:
        break;
    case WM_ASYNC_INIT:
        break;;
    case WM_ASYNC_WRITE:
        break;
    case WM_ASYNC_READ:
        break;
    case WM_ASYNC_CLOSE:
        break;
    case WM_SAMPLE_FILE_WRITE:
        break;
    case WM_TIMER_CYCLIC_TEST:
        break;
    case WM_MQTT_SUBSCRIBE:
        break;
    case WM_STOP_WATCH_DOG_TIMER:
        break;
    case WM_CALCULATE_SHA256:
        break;
    case WM_TASK_TERMINATE:
        break;
    case WM_TASK_RESUME:
        break;
    case WM_DEVICE_CRASH_TEST_A:
        break;
    case WM_DEVICE_CRASH_TEST_B:
        break;
    case WM_DEVICE_CRASH_TEST_C:
        break;
    
    default:
        break;
    }
}  


void app_main(void)
{
    sys_initialize();


    while (1)
    {
        osiThreadSleep(1000);
    }

    return;
}