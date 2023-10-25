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
		RTI_LOG("malloc no memory!");
	}

	memset(rsp, 0, msg->message->payloadlen+100);
	sprintf(rsp, "MQTT RECV: %d,\"%s\",%d,\"%s\"\r\n", msg->message->id,
			msg->topic_name, msg->message->payloadlen, (char*)msg->message->payload);
	RTI_LOG("%s", rsp);

	if(NULL != rsp)
	{
		osiFree(rsp);
	}
}
#define ALIS_PK "gd6jlD70qXQ"
#define ALIS_DN "lock00000001"
#define ALIS_DS "485cc516b6ccbb61e65b31285144c4bc"

#define ALI_MQTT_HOST   "x509.itls.cn-shanghai.aliyuncs.com"
#define ALI_MQTT_PORT   1883

#define ALI_PUB_1   "/gd6jlD70qXQ/lock00000001/user/update"
#define ALI_PUB_2   "/gd6jlD70qXQ/lock00000001/user/update/error"
#define ALI_PUB_DATA_1    "Update Version V1.0.2"
#define ALI_PUB_DATA_2    "Update Error Code 0xFFE9"

#define ALI_SUB_1   "/sys/gd6jlD70qXQ/lock00000001/thing/event/property/post"
#define ALI_SUB_2   "/sys/gd6jlD70qXQ/lock00000001/thing/service/property/set"

extern const char  ali_client_cert[1286];
extern const char  ali_client_key[1676];
extern const char  ali_ca_cert[1262];

void AliMqttDemo(void* p)
{
    int rc = 0;
    char * host = ALI_MQTT_HOST;

    Paytm_TRACE("Ali X.509 test!");

    // config MQTT ca_cert&client_cert&client_privatekey
    Paytm_SSL_Clear_Mqtt();
    Paytm_MQTT_WriteCertificates(ali_ca_cert, ali_client_cert, ali_client_key);

    Paytm_mqtt_connect_Packet_t mqtt_packet = {0};
    ST_MQTT_topic_info_t topic_list = {0};
    Paytm_mqtt_publish_Packet_t publish = {0};
    
    mqtt_packet.host = host;
    mqtt_packet.port = ALI_MQTT_PORT;
    mqtt_packet.client_id = "";
    mqtt_packet.username = "";
    mqtt_packet.password = "";
    // enable ssl  authentication
    mqtt_packet.enable_ssl = true;

    rc = Paytm_MQTT_Initialise(NULL, CERTIFICATE_NVRAM, &mqtt_packet);
    if(rc < 0)
    {
        Paytm_TRACE("Mqtt init fail %d!", rc);
        return;
    }

    rc = Paytm_MQTT_Open();
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt socket open fail %d!", rc);
        if(rc == -29)
        {
            RTI_LOG("Mqtt already opened");
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


    topic_list.topic[0] = ALI_SUB_1;
    topic_list.qos[0] = Paytm_QOS1_AT_LEASET_ONCE;
    topic_list.count = 1;

    rc = Paytm_MQTT_Subscribe(&topic_list, topic_handler);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt subscribe fail 0x%x!", rc);
    }

    topic_list.topic[0] = ALI_SUB_2;
    topic_list.qos[0] = Paytm_QOS2_AT_EXACTLY_ONECE;
    topic_list.count = 1;

    rc = Paytm_MQTT_Subscribe(&topic_list, topic_handler);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt subscribe fail 0x%x!", rc);
    }

    publish.messageId = 58;
    publish.topic = ALI_PUB_1;
    publish.message = ALI_PUB_DATA_1;
    publish.message_length = strlen(publish.message);
    publish.qos = Paytm_QOS1_AT_LEASET_ONCE;
    publish.retain = false;

    rc = Paytm_MQTT_Publish(&publish);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt publish fail 0x%x!", rc);
    }

    publish.messageId = 59;
    publish.topic = ALI_PUB_2;
    publish.message = ALI_PUB_DATA_2;
    publish.message_length = strlen(publish.message);
    publish.qos = Paytm_QOS0_AT_MOST_ONECE;
    publish.retain = false;

    rc = Paytm_MQTT_Publish(&publish);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt publish fail 0x%x!", rc);
    }

    while (1)
    {
        /* code */
        osiThreadSleep(1000);
    }
}