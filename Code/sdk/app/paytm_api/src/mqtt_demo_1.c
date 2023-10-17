#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "mbedtls/ssl.h"

#include "paytm_mqtt_api.h"
#include "paytm_net_api.h"

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

static void topic_handler(void* client, message_data_t* msg)
{
    RTI_LOG("topic handler is called...");
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

#define DEMO_MQTT_HOST  "a3ri26a4bhtu9s-ats.iot.ap-south-1.amazonaws.com"
#define DEMO_MQTT_PORT			8883

#define DEMO_MQTT_PRODUCT_KEY 	"qa3-iot-int-WAM4GS-864180051437643"
#define DEMO_USER_NAME   		""
#define DEMO_USER_PWD 			""

#define DEMO_SUB_TOPIC			"qa3-iot-int/client/ab7017d2-e7ca-4ef3-a7d0-f8dbefc6b891_WAM4GS_864180051437643"
#define DEMO_SUB_TOPIC_2        "qa2-iot-int/client/25f7b4a3-b5af-4801-b157-5d7b4a7011b9_WAM4GS_864180051473234"
#define DEMO_SUB_TOPIC_3        "$aws/things/qa3-iot-int-WAM4GS-864180051437643/jobs/notify-next"
#define SHORT_PUB   "short/1"
#define DEMO_PUB_TOPIC          "$aws/things/qa3-iot-int-WAM4GS-864180051437643/shadow/get"
#define DEMO_PUB_DATA			"{\"v\":\"V2.0.0_HI\",\"deviceId\":\"864180051437643\",\"deviceType\":\"WAM4GS\",\"firmwareVersion\":\"V2.0.0_HI\",\"firmwareLanguage\":\"HI\",\"resourceVersion\":\"V0.0.0_HI\",\"id\":\"864180051437643\",\"typ\":\"WAM4GS\",\"rId\":\"84\",\"c\":\"state\"}"


#define DEMO_PUB_TOPIC_2        "qa2-iot-int/client/25f7b4a3-b5af-4801-b157-5d7b4a7011b9_WAM4GS_864180051473234"
#define DEMO_PUB_TOPIC_3        "qa2-iot-int/client/25f7b4a3-b5af-4801-b157-5d7b4a7011b9_WAM4GS_864180051473234"
#define DEMO_PUB_DATA_2			"The message demo/002"




const char  mqtt_cacert_1[] = "-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n\
rqXRfboQnoZsG4q5WTP468SQvvG5\n\
-----END CERTIFICATE-----\n";


const char  mqtt_clientcert_1[] = "-----BEGIN CERTIFICATE-----\n\
MIIDWTCCAkGgAwIBAgIUVTdXufBYUVLt/VocUHuh4w28uoQwDQYJKoZIhvcNAQEL\n\
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n\
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMDgyNjE0MTUz\n\
MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n\
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAM2v9lUekHfQdm0XoyjI\n\
bohPMGy8hG8fGyL1Go4bnLzsbIj21Ca2xIbKkXU2rNYXMTUfpD215iyKiiP+tWqq\n\
o7U1YrD+mdDnvwuDutcn/h0cCHl23Zua0FDdnRa0tMaoCSmgBqB4K44rqYFUwB9X\n\
FFeZCZndfDeAGSZauLydTaOnIqfFY3n1Wz2jJY4jZFrd9ZSbYsS+cwGM7jx4Uhql\n\
JxRY3xP3+y70LzEIaK5O5p9Sac0jSinGPkipypgNRIYykvqt3pOV5G58j4T+8Hwu\n\
NN9BG1kunWCKCETfMndX1jwEFQmZfVFeZ7ijEYus/lYd/U4b673n9dWaCEl2RiJk\n\
+L8CAwEAAaNgMF4wHwYDVR0jBBgwFoAUlRysbh4Ub/Y7cTdrlFSL5oRfsmswHQYD\n\
VR0OBBYEFItFl1veYnUfs2gALtgmgBRU0Bc4MAwGA1UdEwEB/wQCMAAwDgYDVR0P\n\
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCh2AwjQotj/fbQ4KVqLSgWV5Bb\n\
DhQYrn4NJEN5wb0iaeI01Jom4g/um7oqfMBXyIKkwrvaE9z/VPPRyaMZ+Jzre+kS\n\
Elp52Zcj6QzZZdPoLnkvvBK8gapU4BotyU6aS6xhcOCPX+svxyOe9cdFIxLMnYS4\n\
KzML7+EDz5fJ8oXvwfC8RLjIex8HSWrvVfmT/i5BwEAsRWeG8fPLZOOeeRN+2phJ\n\
dklXnd2w3y1L1ZnZfxN/S8IW5tQM9F8h8buVhe/ABQC+jfhbuLU54dZSLF9NLqpW\n\
92H7mAhH2ItZiop2lSn/awiZ2DROG+XGBrK1LiJSlcIpcC7KpHLZCjriYU99\n\
-----END CERTIFICATE-----\n";

const char  mqtt_clientkey_1[] = "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpAIBAAKCAQEAza/2VR6Qd9B2bRejKMhuiE8wbLyEbx8bIvUajhucvOxsiPbU\n\
JrbEhsqRdTas1hcxNR+kPbXmLIqKI/61aqqjtTVisP6Z0Oe/C4O61yf+HRwIeXbd\n\
m5rQUN2dFrS0xqgJKaAGoHgrjiupgVTAH1cUV5kJmd18N4AZJlq4vJ1No6cip8Vj\n\
efVbPaMljiNkWt31lJtixL5zAYzuPHhSGqUnFFjfE/f7LvQvMQhork7mn1JpzSNK\n\
KcY+SKnKmA1EhjKS+q3ek5XkbnyPhP7wfC4030EbWS6dYIoIRN8yd1fWPAQVCZl9\n\
UV5nuKMRi6z+Vh39Thvrvef11ZoISXZGImT4vwIDAQABAoIBAQCp7Jq2y19SfHe5\n\
vHNgd+VI2xMIYd2O58XFZXQgvpMISNGJfVDbAtbs/GCa0yaKX/KZDqDjEmkrlvB0\n\
uDIsAvjn9UipLEzqlunvm045/aIjbktZ/Wcq7AT5KXxji7pk++FF8BxJab2h4FRS\n\
si9eNNRrYdTatm2udDdSxlQkDlhbv3WoahqrqCcLBmhWndSRzPihZeEBEPdYBZJd\n\
ldQWnBkX5OeB/ZxGh2S+oLvEi5ffb7yb48QFP8CUF1q/JDbNOFbY4iYGaQJmIoA+\n\
gek7WdyW8/NU0QH2sy5H6B5WoSXNoY6oXPvg7wZpc97wGQegsF0UaWZfrFr9kpsY\n\
NoPYAYExAoGBAPkNvUJgksagrXOTPFA4dJy5ex9u2ZTpDSQ+psauuWl3x5amz37E\n\
3Xg3rG6Gw2jH1392ccap3SsUoTxThEnx3z1OQ5wIFt044aZPIEWlTkNh8JRnvNp0\n\
UWBdHFedhL8pltNU5sEUIYgqVPrBZ2D9yEHQrUqPLUw4R+hxsaPo96j1AoGBANNs\n\
lZrE4udJZ1kLlVR/dRI93p0CFIbmqv6VbRHTi6myLQbXLi/kDWACzKkJ/vlIktmQ\n\
eyN47rsbLoePhX/CcxIcEvYuG3SlN5/LQVRVb9q20VMqIC9aiWklZcDcTfx57rT2\n\
pA64XjrWm+VtP1PIjA/NLXED0i/+TN9wfogTBtpjAoGAKWY9RaFkBJyE+KdK6VLZ\n\
H4w9iTYV+QjcZF/9vR0iOQPuK37DEyGKZwdqJF1Gck515fw8YXevChA91RdU8C3C\n\
nCA8YN1/X3qrF8gjZzBZWB4ZrbGo1O9s74XKxk2q2VQCfFKqoQHFqSrQQXaQxrCo\n\
7zfO9oviTb81RNBRZRu+v1kCgYEArXxvcnVWv3TTMhU4LwL7fMF59xoq39tNuuPA\n\
gR02gEPtgKx3gOLAOuVxLNBAAiwUHCTbdmsYcj5GuwQBBqn3pIeW451H+DpOX3f4\n\
nowQ4cacE4MAr/q5wKboTQ45jBA92Q595npREDTj7ansigKJM7ooLJF3GtywFbUI\n\
jv4V5IMCgYBacs38gA/Kn65RKxA6wZq91oXMjDjQDyB/t2a5Sz5P5510EpOKcQSy\n\
+8NH1aAgtZ60kABFiRefx74mk943jXgSbAYliY5Eq99jboRViABdf7F05Ofb27+8\n\
obISYKznQ9hkZtv2Qz2941fNncz7cEDE3DAqnpqpi0Fu+iV+TBr0uw==\n\
-----END RSA PRIVATE KEY-----\n";

void Mqtt_0(void* p)
{
    int rc = 0;
    char * host = DEMO_MQTT_HOST;
    char * client_id = DEMO_MQTT_PRODUCT_KEY;
    char * username = DEMO_USER_NAME;
    char * password = DEMO_USER_PWD;

    net_connect();
    
    // config MQTT ca_cert&client_cert&client_privatekey
    Paytm_MQTT_WriteCertificates(mqtt_cacert_1, mqtt_clientcert_1, mqtt_clientkey_1);

    Paytm_mqtt_connect_Packet_t mqtt_packet = {0};
    ST_MQTT_topic_info_t topic_list = {0};
    Paytm_mqtt_publish_Packet_t publish = {0};
    
    mqtt_packet.host = host;
    mqtt_packet.port = DEMO_MQTT_PORT;
    mqtt_packet.client_id = client_id;
    mqtt_packet.username = username;
    mqtt_packet.password = password;
    // enable ssl  authentication
    mqtt_packet.enable_ssl = true;

    rc = Paytm_MQTT_Initialise(NULL, CERTIFICATE_NVRAM, &mqtt_packet);
    if(rc < 0)
    {
        Paytm_TRACE("Mqtt init fail %d!", rc);
        return;
    }else{
        Paytm_TRACE("Mqtt init success %d!", rc);
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
    }else{
        Paytm_TRACE("Mqtt socket open success %d!", rc);
    }

    rc = Paytm_MQTT_Connect();
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt socket connect fail %d!", rc);
        return;
    }else{
        Paytm_TRACE("Mqtt socket connect success %d!", rc);
    }


    topic_list.topic[0] = DEMO_SUB_TOPIC;
    topic_list.qos[0] = Paytm_QOS1_AT_LEASET_ONCE;
    topic_list.count = 1;

    rc = Paytm_MQTT_Subscribe(&topic_list, topic_handler);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt subscribe 1 fail 0x%x!", rc);
    }else{
        Paytm_TRACE("Mqtt subscribe 1 success %d!", rc);
    }

    publish.messageId = 58;
    publish.topic = SHORT_PUB;
    publish.message = DEMO_PUB_DATA;
    publish.message_length = strlen(publish.message);
    publish.qos = Paytm_QOS1_AT_LEASET_ONCE;
    publish.retain = false;

    rc = Paytm_MQTT_Publish(&publish);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt publish 1 fail 0x%x!", rc);
    }else{
        Paytm_TRACE("Mqtt publish 1 success %d!", rc);
    }

    topic_list.topic[0] = DEMO_SUB_TOPIC_2;
    topic_list.qos[0] = Paytm_QOS2_AT_EXACTLY_ONECE;
    topic_list.count = 1;

    rc = Paytm_MQTT_Subscribe(&topic_list, topic_handler);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt subscribe 2 fail 0x%x!", rc);
    }else{
        Paytm_TRACE("Mqtt subscribe 2 success %d!", rc);
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
        Paytm_TRACE("Mqtt publish 2 fail 0x%x!", rc);
    }else{
        Paytm_TRACE("Mqtt publish 2 success %d!", rc);
    }

    while (1)
    {
        /* code */
        osiThreadSleep(1000);
    }
}