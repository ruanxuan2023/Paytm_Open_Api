#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "mbedtls/ssl.h"

#include "paytm_button_api.h"
#include "paytm_http_api.h"
#include "paytm_mqtt_api.h"
#include "paytm_net_api.h"
#include "paytm_sys_api.h"
#include "paytm_file_api.h"
#include "paytm_sim_api.h"
#include "paytm_debug_uart_api.h"
#include "paytm_led_api.h"

#include "drv_adc.h"

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
    RTI_LOG1("This is buttoncb, action is ");
    if(*(int*)p == STATE_BUTTON_DOUBLE_CLICK)
    {
        RTI_LOG("Double click");
    }else if(*(int*)p == STATE_BUTTON_SINGLE_CLICK)
    {
        RTI_LOG("Single click");
    }else if(*(int*)p == STATE_BUTTON_LONG_PRESS)
    {
        RTI_LOG("Long press");
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

extern void sys_initialize(void);
extern void getTaobaoSuggest(void* p);
extern void testMqtt_india(void* p);
extern void testMqtt(void* p);
extern void PaytmHttpSSLPost(void * p);
extern void Mqtt_0(void* p);
extern void ntpDemo(void *p);
extern void batteryDemo(void);
extern void audioPlayDemo(void);
void app_main(void)
{
    sys_initialize();

    audioPlayDemo();

    while (1)
    {
        osiThreadSleep(1000);
    }

    return;
}