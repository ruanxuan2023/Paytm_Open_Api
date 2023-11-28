#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "mbedtls/ssl.h"

#include "paytm_mqtt_api.h"
#include "paytm_net_api.h"

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
    Paytm_PlayFile(LOC_INTER_MEM, "and.amr", 6);
	if(NULL != rsp)
	{
		osiFree(rsp);
	}
}

#define DEMO_MQTT_HOST              "pay.chinainfosafe.com"
#define DEMO_MQTT_SSL_PORT			8883
#define DEMO_MQTT_TCP_PORT          6883

#define DEMO_MQTT_PRODUCT_KEY 	"paytm_0"
#define DEMO_USER_NAME   		"china"
#define DEMO_USER_PWD 			"12345611"

#define DEMO_SUB_TOPIC			"publish/0"
#define DEMO_SUB_TOPIC_2        "publish/2"
#define DEMO_SUB_TOPIC_3        "publish/3"

#define DEMO_PUB_TOPIC          "demo/001"
#define DEMO_PUB_TOPIC_2        "demo/002"
#define DEMO_PUB_TOPIC_3        "demo/003"
#define DEMO_PUB_DATA			"The message demo/001"
#define DEMO_PUB_DATA_2			"The message demo/002"

static void reconnect_handler(void* client, void* reconnect_date)
{
    Paytm_TRACE("Mqtt disconnected event comes, call reconnected func ...");
    Paytm_Mqttt_Try_Reconnect();
    /* no need to call reconnected func, when this cb exit, it will try to reconnected automatically */
}

extern const char  mqtt_client_key[1705];
extern const char  mqtt_client_cert[1173];
extern const char  mqtt_server_cert[4789];
extern const char  ali_ca_cert[];
extern char* appIMEIGet(void);
void testMqtt(void* p)
{
    int rc = 0;
    char * host = DEMO_MQTT_HOST;
    char * client_id = DEMO_MQTT_PRODUCT_KEY;
    char * username = DEMO_USER_NAME;
    char * password = DEMO_USER_PWD;

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
    mqtt_packet.keepalive_sec = 10;
    // enable ssl  authentication
    mqtt_packet.enable_ssl = false;

    rc = Paytm_MQTT_Initialise(NULL, CERTIFICATE_NVRAM, &mqtt_packet);
    if(rc < 0)
    {
        Paytm_TRACE("Mqtt init fail %d!", rc);
        return;
    }

    Paytm_Mqtt_Reconnected_Register(reconnect_handler);

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
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt socket connect fail %d!", rc);
        return;
    }

    topic_list.topic[0] = DEMO_SUB_TOPIC;
    topic_list.qos[0] = Paytm_QOS1_AT_LEASET_ONCE;
    topic_list.count = 1;

    rc = Paytm_MQTT_Subscribe(&topic_list, topic_handler);
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

    publish.messageId = 59;
    publish.topic = DEMO_PUB_TOPIC_2;
    publish.message = DEMO_PUB_DATA_2;
    publish.message_length = strlen(publish.message);
    publish.qos = Paytm_QOS0_AT_MOST_ONECE;
    publish.retain = false;

    rc = Paytm_MQTT_Publish(&publish);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt publish fail 0x%x!", rc);
    }

    Paytm_TRACE(">>>>Mqtt start success!");
    while (1)
    {
        /* code */
        osiThreadSleep(1000);
    }
}