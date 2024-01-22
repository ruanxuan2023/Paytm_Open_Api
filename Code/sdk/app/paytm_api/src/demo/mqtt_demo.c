#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "mbedtls/ssl.h"

#include "paytm_mqtt_api.h"
#include "paytm_net_api.h"
#include "mqtt_error.h"

static int task_id = 0;

#define PAYTM_MQTT_INIT 0
#define PAYTM_MQTT_OPEN 1
#define PAYTM_MQTT_CONNECT 2
#define PAYTM_MQTT_SUBSCRIBE 3
#define PAYTM_MQTT_PUBLISH 4
#define PAYTM_MQTT_DISCONNECT 5
#define PAYTM_MQTT_CLOSE 6
#define PAYTM_MQTT_RECONNECT 7

static void topic_handler(void* client, message_data_t* msg)
{
	char *rsp = NULL;

	rsp = (char *)osiMalloc(msg->message->payloadlen+100);
	if(NULL == rsp)
	{
		Paytm_TRACE("malloc no memory!");
	}

	memset(rsp, 0, msg->message->payloadlen+100);
	sprintf(rsp, "MQTT RECV: %d,\"%s\",%d,\"%s\"\r\n", msg->message->id,
			msg->topic_name, msg->message->payloadlen, (char*)msg->message->payload);
	Paytm_TRACE("%s", rsp);
	if(NULL != rsp)
	{
		osiFree(rsp);
	}
}

static void other_msg_handler(void* client, message_data_t* msg)
{
    char *rsp = NULL;

	rsp = (char *)osiMalloc(msg->message->payloadlen+100);
	if(NULL == rsp)
	{
		Paytm_TRACE("malloc no memory!");
	}

	memset(rsp, 0, msg->message->payloadlen+100);
	sprintf(rsp, "MQTT RECV OTHER: %d,\"%s\",%d,\"%s\"\r\n", msg->message->id,
			msg->topic_name, msg->message->payloadlen, (char*)msg->message->payload);
	Paytm_TRACE("%s", rsp);
	if(NULL != rsp)
	{
		osiFree(rsp);
	}
}

#define DEMO_MQTT_HOST              "pay.chinainfosafe.com"
#define DEMO_MQTT_SSL_PORT			8883
#define DEMO_MQTT_TCP_PORT          6883

#define DEMO_MQTT_PRODUCT_KEY 	"paytm_2"
#define DEMO_USER_NAME   		"china_2"
#define DEMO_USER_PWD 			"1234567890AC"

#define DEMO_SUB_TOPIC			"publish/0"
#define DEMO_SUB_TOPIC_2        "publish/2"
#define DEMO_SUB_TOPIC_3        "publish/3"

#define DEMO_PUB_TOPIC          "demo/001"
#define DEMO_PUB_TOPIC_2        "demo/002"
#define DEMO_PUB_TOPIC_3        "demo/003"
#define DEMO_PUB_DATA			"The message demo/001"
#define DEMO_PUB_DATA_2			"The message demo/002"

static void event_handler(void* client, int event_id)
{
    Paytm_TRACE("Event Handler: ");
    switch (event_id)
    {
    case KAWAII_MQTT_NOT_CONNECT_ERROR:
        Paytm_TRACE("# Mqtt connect error");
        Paytm_SendMessage_From_ISR(task_id, PAYTM_MQTT_RECONNECT, 58, 58);
        break;
    case KAWAII_MQTT_CONNECT_FAILED_ERROR:
        Paytm_TRACE("# Mqtt connect fail");
        break;
    case CLIENT_STATE_CONNECTED:
        Paytm_TRACE("# Mqtt connected");
        break;
    case CLIENT_STATE_DISCONNECTED:
        Paytm_TRACE("# Mqtt disconnected");
        Paytm_SendMessage_From_ISR(task_id, PAYTM_MQTT_RECONNECT, 58, 58);
        // Paytm_SendMessage_From_ISR(task_id, PAYTM_MQTT_DISCONNECT, 58, 58);
        break;
    case CLIENT_STATE_CLEAN_SESSION:
        Paytm_TRACE("# Mqtt clean session");
        break;
    default:
        break;
    }
}

extern const char  mqtt_client_key[1705];
extern const char  mqtt_client_cert[1173];
extern const char  mqtt_server_cert[4789];
extern const char  ali_ca_cert[];
extern char* appIMEIGet(void);
void CisMqtt(void* p)
{
    int rc = 0;
    char * host = DEMO_MQTT_HOST;
    char * client_id = DEMO_MQTT_PRODUCT_KEY;
    char * username = DEMO_USER_NAME;
    char * password = DEMO_USER_PWD;

    char * will_topic = DEMO_PUB_TOPIC_3;
    char * will_msg = "Hello";
    // config MQTT ca_cert&client_cert&client_privatekey
    Paytm_MQTT_WriteCertificates(mqtt_server_cert, mqtt_client_cert, mqtt_client_key);

    Paytm_mqtt_connect_Packet_t mqtt_packet = {0};
    ST_MQTT_topic_info_t topic_list = {0};
    Paytm_mqtt_publish_Packet_t publish = {0};

    mqtt_packet.host = host;
    mqtt_packet.port = DEMO_MQTT_TCP_PORT;
    mqtt_packet.client_id = client_id;
    mqtt_packet.username = username;
    mqtt_packet.password = password;
    mqtt_packet.keepalive_sec = 60;
    mqtt_packet.version_num = 4;    //3 = 3.1 4 = 3.1.1
    // enable ssl  authentication
    mqtt_packet.enable_ssl = false;

    mqtt_packet.will_flag = 0;
    mqtt_packet.will_qos = 1;
    mqtt_packet.will_retain = 1;
    mqtt_packet.will_topic = will_topic;
    mqtt_packet.will_message = (uint8_t*)will_msg;
    mqtt_packet.cleansession = 0;

    rc = Paytm_MQTT_Initialise(NULL, CERTIFICATE_NVRAM, &mqtt_packet);
    if(rc < 0)
    {
        Paytm_TRACE("Mqtt init fail %d!", rc);
        return;
    }

    Paytm_Mqtt_EventHandler_Register(event_handler);
    Paytm_MQTT_Unsubscried_CallBack_Register(other_msg_handler);
    rc = Paytm_MQTT_Open();
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt socket open fail %d!", rc);
        if(rc == -29)
        {
            Paytm_TRACE("Mqtt already opened");
            Paytm_MQTT_Disconnect();
        }
        return;
    }   

    rc = Paytm_MQTT_Connect();
    if(rc < 0)
    {
        Paytm_TRACE("Mqtt socket connect fail %d!", rc);
        return;
    }else if(rc == 1){
        Paytm_TRACE("Mqtt session reuse, no need to subscribe again!");
    }

    topic_list.topic[0] = DEMO_SUB_TOPIC;
    topic_list.qos[0] = Paytm_QOS2_AT_EXACTLY_ONECE;
    topic_list.count = 1;

    if(rc == 1){
        rc = Paytm_MQTT_Add_Subscribe_List_Only(&topic_list, topic_handler);
    }else{
        rc = Paytm_MQTT_Subscribe(&topic_list, topic_handler);
    }
   
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt subscribe fail 0x%x!", rc);
    }

    publish.messageId = 58;
    publish.topic = DEMO_PUB_TOPIC;
    publish.message = DEMO_PUB_DATA;
    publish.message_length = strlen(publish.message);
    publish.qos = Paytm_QOS1_AT_LEASET_ONCE;
    publish.retain = false;

    rc = Paytm_MQTT_Publish(&publish);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt publish fail 0x%x!", rc);
    }

// exit:
    Paytm_TRACE("Mqtt start success");
}

void* cis_mqtt_task(void* p)
{
    ST_MSG msg = {0};

    while (1)
    {
        /* code */
        Paytm_GetMessage(task_id, &msg);
        Paytm_TRACE("Msg comes: %d", msg.message);
        switch (msg.message)
        {
        case PAYTM_MQTT_INIT:
            CisMqtt(NULL);
            break;
        case PAYTM_MQTT_DISCONNECT:
            Paytm_MQTT_Close();
            Paytm_delayMilliSeconds(2 * 1000);
            Paytm_SendMessage(task_id, PAYTM_MQTT_INIT, 0, 0);
            break;
        case PAYTM_MQTT_RECONNECT:
            while(Paytm_Mqttt_Try_Reconnect() != 0){
                Paytm_delayMilliSeconds(3 * 1000);
            }
            break;
        case PAYTM_MQTT_CLOSE:
            break;
        default:
            break;
        }

        Paytm_delayMilliSeconds(100);
    }
    
}

void CisMqttDemo(void)
{
    task_id = Paytm_CreateTask("cis_mqtt_task", cis_mqtt_task, NULL, 110, 40 * 1024);

    Paytm_delayMilliSeconds(400);
    Paytm_TRACE("Cis Mqtt Task id: %d", task_id);
    Paytm_SendMessage_From_ISR(task_id, PAYTM_MQTT_INIT, 58, 58);

    Paytm_delayMilliSeconds(10000);
    // Paytm_MQTT_Disconnect();
}