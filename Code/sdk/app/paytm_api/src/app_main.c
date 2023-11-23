#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "mbedtls/ssl.h"
#include "mbedtls/sha256.h"

#include "drv_adc.h"

#include "paytm_audio_api.h"
#include "paytm_battery_api.h"
#include "paytm_button_api.h"
#include "paytm_debug_uart_api.h"
#include "paytm_dev_api.h"
#include "paytm_file_api.h"
#include "paytm_http_api.h"
#include "paytm_led_api.h"
#include "paytm_mqtt_api.h"
#include "paytm_net_api.h"
#include "paytm_sim_api.h"

#include "paytm_sys_api.h"
#include "paytm_debug_uart_api.h"
#include "paytm_led_api.h"
#include "paytm_tls_verify.h"


static void net_connect(void)
{
    Paytm_GPRS_Connect(Paytm__IPVERSION_IPV4, NULL);
    //int32 stat = 0;
    // while (!(Paytm_GetGPRSState(&stat) == 1 || Paytm_GetGPRSState(&stat) == 5))
    // {
    //     //RTI_LOG("Networking connecting");
    //     Paytm_delayMilliSeconds(1000);
    // }
    
    while (!Paytm_Net_IsConnected())
    {
        RTI_LOG("Networking connecting");
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
    WM_CJSON_CREATE            = 69,
    WM_CJSON_PRASE             = 70,
    WM_SAVE_READ_AUTHID        = 71,
    WM_PLAY_COMBINE_AUDIOS     = 72,
    WM_CERT_READ_WRITE         = 73,
    WM_PWK_DEMO                = 74,
    WM_BUTTON_DEMO             = 75,
    WM_TIMEZONE_CHANGE         = 76,
    WM_POWER_ON_OFF_FLOW       = 78,
	WM_RTC_TIME_SET            = 79,
    WM_SET_APN                 = 80,

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
extern void httpDownload2(void* p);
extern void ButtonTest(void);
extern void fileUnzip(void);
extern void fileLite(void);
extern void cJsonCreate(void);
extern uint8 cJsonPrase(const char*  val);
extern void ledRun(void);
extern void devInfoDemo(void);
extern void pwkDemo(void);
extern void timeZoneSet(void);
extern void fota_download(void* p);
extern void dfota_download(void* p);
extern void rtcTimeSet(void);
extern void setApn(void);
extern void fileFormatDemo(void);
extern void runTimer(void);
extern void fileCreateFolderDemo(void);
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
        net_connect();
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
        net_connect();
        Paytm_CreateTask("http", httpDownload2, NULL, 100, 80 * 1024);
        break;
    case WM_HTTP_TEST:
        testHttpGet(NULL);
        break;
    case WM_FILE_TEST:
        // fileSystemDemo();
        // fileFormatDemo();
        fileCreateFolderDemo();
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
        ret = Paytm_Cal_File_Hash(LOC_EXTER_MEM, "resources/sounds/en/welc.amr", digest);
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
        net_connect();
        Paytm_CreateTask("mqtt", testMqtt, NULL, 110, 40 * 1024);
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
        net_connect();
        Paytm_CreateTask("Dfota", dfota_download, NULL, 100, 60 * 1024);
        break;
    case WM_SSL_DEMO:
        break;
    case WM_MBED_DEMO:
        break;
    case WM_OTA_TEST_QA:
        net_connect();
        Paytm_CreateTask("fota", fota_download, NULL, 100, 60 * 1024);
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
        runTimer();
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
    case WM_CJSON_CREATE:
        cJsonCreate();
        break;
    case WM_CJSON_PRASE :
    {
        char val[] = "{ \
        \"SceneID\": 6,   \
        \"Enable\": 1,    \
        \"Count\": 1,     \
        \"Data\": [       \
            {           \
            \"BrightValue\": 37   \
            },                  \
            {                   \
            \"ColorTempValue\": 4937      \
            }           \
        ]               \
        }";
        cJsonPrase(val);
        break;
    }
    case WM_SAVE_READ_AUTHID:
    {
        char token[32] = {0};
        char auth_id[120 + 1] = {0};
        // char auth_id_set[120] = {0};
        char *authid = "qa3-iot-int/client/74cbc5e6-355d-4334-b4d1-7702fcbf124b_WAM4GS_864180051437643";

        Paytm_readAuthID(auth_id, 120);
        Paytm_readToken32Byte(token, 32);
        Paytm_TRACE("auth_id: %s", auth_id);
        Paytm_TRACE("token: %s", token);

        // memset(auth_id_set, 0x41, 120);
        Paytm_setAuthID(authid, strlen(authid));

        memset(auth_id, 0x00, 120);
        Paytm_readAuthID(auth_id, strlen(authid));
        auth_id[120] = '\0';
        Paytm_TRACE("auth_id: %s", auth_id);
        break;
    }
    case WM_PLAY_COMBINE_AUDIOS:
    {
        Paytm_PlayFileFromDir(LOC_EXTER_MEM, "resources/sounds/en", "Received.amr,10000.amr,and.amr,num90.amr", 7);
        break;
    }
    case WM_CERT_READ_WRITE:
    {
       
        int http_ca_len = 0, http_cert_len = 0, http_key_len = 0;
        char *http_ca, *http_cert, *http_key;
        int rc = 0;
     #if 1
        http_ca_len = Paytm_filesize(LOC_INTER_MEM, HTTP_CA_FILE);
        http_cert_len = Paytm_filesize(LOC_INTER_MEM, HTTP_CERT_FILE);
        http_key_len = Paytm_filesize(LOC_INTER_MEM,  HTTP_KEY_FILE);

        Paytm_TRACE("Size %d, %d, %d", http_ca_len, http_cert_len, http_key_len);

        http_ca = (char*)Paytm_malloc(http_ca_len + 1);
        http_cert = (char*)Paytm_malloc(http_cert_len + 1);
        http_key = (char*)Paytm_malloc(http_key_len + 1);
        http_ca[http_ca_len] = '\0';
        http_cert[http_cert_len] = '\0';
        http_key[http_key_len] = '\0';

        Paytm_Read_HTTP_Certs(LOC_INTER_MEM, http_ca , http_cert, http_key);
        
        Paytm_TRACE("http_ca: %s", http_ca);
        Paytm_TRACE("http_cert: %s", http_cert);
        Paytm_TRACE("http_key: %s", http_key);
        Paytm_free(http_key);
        Paytm_free(http_ca);
        Paytm_free(http_cert);
        #endif
        char *ca_f = "AAAAACCCCCEEEEEBBBBBDDDDDFFFFF";
        char *cert_f = "012345678901234567890123456789";
        char *key_f = "+++++-----@@@@@*****((((()))))";
        
        rc = Paytm_Write_HTTP_Certs(LOC_INTER_MEM, ca_f, strlen(ca_f), cert_f, strlen(cert_f), key_f, strlen(key_f));
        Paytm_TRACE("Paytm_Write_HTTP_Certs rc = %d", rc);

        http_ca_len = Paytm_filesize(LOC_INTER_MEM, HTTP_CA_FILE);
        http_cert_len = Paytm_filesize(LOC_INTER_MEM, HTTP_CERT_FILE);
        http_key_len = Paytm_filesize(LOC_INTER_MEM,  HTTP_KEY_FILE);

        Paytm_TRACE("Size %d, %d, %d", http_ca_len, http_cert_len, http_key_len);

        http_ca = (char*)Paytm_malloc(http_ca_len + 1);
        http_cert = (char*)Paytm_malloc(http_cert_len + 1);
        http_key = (char*)Paytm_malloc(http_key_len + 1);
        http_ca[http_ca_len] = '\0';
        http_cert[http_cert_len] = '\0';
        http_key[http_key_len] = '\0';

        Paytm_Read_HTTP_Certs(LOC_INTER_MEM, http_ca , http_cert, http_key);
        
        Paytm_TRACE("http_ca: %s", http_ca);
        Paytm_TRACE("http_cert: %s", http_cert);
        Paytm_TRACE("http_key: %s", http_key);
        Paytm_free(http_key);
        Paytm_free(http_ca);
        Paytm_free(http_cert);

        break;
    }
    case WM_PWK_DEMO:
        Paytm_Battery_Initialise(20);
        pwkDemo();
        break;
    case WM_BUTTON_DEMO:
        ButtonTest();
        break;
    case WM_TIMEZONE_CHANGE:
        net_connect();
        timeZoneSet();
        break;
    case WM_POWER_ON_OFF_FLOW:
        //add code to do power down flow

        //then call function to shut down the device
        Paytm_PowerDown(0);
        break;
    case WM_RTC_TIME_SET:
        rtcTimeSet();
        break;
    case WM_SET_APN:
        setApn();
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

void print_net_status(void* p)
{
    while (1)
    {
        if(Paytm_Net_IsConnected())
        {
            RTI_LOG("Networking connected");
        }else{
            RTI_LOG("Networking not connected");
        }
        
        Paytm_delayMilliSeconds(1000);
    }
}

void app_main(void)
{
    sys_initialize();

    Paytm_TRACE("************************************************\n");
    // WM_APP_DOWNLOAD WM_GET_SIM_INFO WM_FILE_TEST WM_CERT_READ_WRITE WM_OTA_TEST_QA WM_PWK_DEMO
    OpenDemoViaId(WM_PWK_DEMO);
    OpenDemoViaId(WM_GET_SIM_INFO);
    // Paytm_TRACE("***************TEST FOTA BIN********************\n");
    while (1)
    {
        osiThreadSleep(1000);
    }

    return;
}