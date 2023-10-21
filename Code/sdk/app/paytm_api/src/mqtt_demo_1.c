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

#define DEMO_MQTT_PRODUCT_KEY 	"dev-WAM4GS-864180051458391"
#define DEMO_USER_NAME   		""
#define DEMO_USER_PWD 			""

#define DEMO_SUB_TOPIC			"dev/client/70e99210-be6b-4a2e-af8a-5d0954acc8dd_WAM4GS_864180051458391"
#define DEMO_SUB_TOPIC_2        "$aws/things/dev-WAM4GS-864180051458391/jobs/notify-next"
#define DEMO_SUB_TOPIC_3        "$aws/things/dev-WAM4GS-864180051458391/jobs/start-next/accepted"
#define SHORT_PUB               "short/1"
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
MIIDWjCCAkKgAwIBAgIVAO0nY9o47TNfxDh5OMmdn4CYZFPbMA0GCSqGSIb3DQEB\n\
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n\
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMzEwMTgwODAx\n\
MTFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n\
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDKLZ8XLBy6IXsDEj5E\n\
1mdpyJGV8N1UeQXg8WyG2+y23OnklL6sTT6uvX63CO2LhfPRATRGPvGCJ//QhVdi\n\
i5rDWVD7cxkREXmtOGdojihZEf7HlxXVyc3L4M1AmfFBt11JLQPb0/Csl55lD645\n\
hhT3WgB3XJvJ3eLwtRZykWeVO9QaG+bE/2LhSfuwgg2k3KGtltYNB8bI74bm0v/e\n\
XtRA1fwPGK84kn8LJ3tmAdeWbCUXC12sLV5pBpJYe+/FQUue1a3Mt8ngdv92iy2W\n\
cOEU8sjkNz8HKHxfoB5SH5AgVwbWQfGxhiDfQ5PtTfC4kzZp2O0xbPzjZ48hqsDZ\n\
emqXAgMBAAGjYDBeMB8GA1UdIwQYMBaAFNxLMyQHGXMRiGPyJdQdNRUV/PuyMB0G\n\
A1UdDgQWBBQGQta8LafdluFXucHQj3tZOwvaWDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n\
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEALUn2iqvNFri3BDDyDqqyUceb\n\
uHUqKv1yw4Oj55Nx+wjhSYmBRDsYfx77gOnzRicYYgZKXoe3mbUxK+jmWr9Wzzet\n\
M6WrC7bb66Ns+BFPrClvTOzy5zSyxNGPcGL42ysMpqlyxPAKnqvZWvSywMlgd6ce\n\
P/3D+7xbgrvR5cWk1I12lzX0HvotrJ8W5hIwMFPFZiU44WUx24p/tvEowL2RswYb\n\
wSOcZ2lYuK2ydCz5Bkuk5oQa17NNu8fC+Z3vTMpWRC8YeczjEafxJx1gLijm8/8z\n\
r/OBfwIK8og7t25294SC+ziCVwGHRT3IMQEj+ATn8vcPuahpHD67PqCc2GFKOg==\n\
-----END CERTIFICATE-----\n";

const char  mqtt_clientkey_1[] = "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpAIBAAKCAQEAyi2fFywcuiF7AxI+RNZnaciRlfDdVHkF4PFshtvsttzp5JS+\n\
rE0+rr1+twjti4Xz0QE0Rj7xgif/0IVXYouaw1lQ+3MZERF5rThnaI4oWRH+x5cV\n\
1cnNy+DNQJnxQbddSS0D29PwrJeeZQ+uOYYU91oAd1ybyd3i8LUWcpFnlTvUGhvm\n\
xP9i4Un7sIINpNyhrZbWDQfGyO+G5tL/3l7UQNX8DxivOJJ/Cyd7ZgHXlmwlFwtd\n\
rC1eaQaSWHvvxUFLntWtzLfJ4Hb/dostlnDhFPLI5Dc/Byh8X6AeUh+QIFcG1kHx\n\
sYYg30OT7U3wuJM2adjtMWz842ePIarA2XpqlwIDAQABAoIBAHHaYx6/fqXquMUv\n\
/tTxEGdnxWR0sY8iYAcerYj8TD9mBGjuwrlw/n2UxMj6RBswIe9vCCsOwx4SvGKG\n\
1IDwMQUVRR9034bXGnhLrSAZ/eq60YlAMdGkxTmsrKlWh6AETIoqyWN/5Kas80vi\n\
GrDWKvRoQJ73DdwOEgCPWcbGz1gfymCbaog/uxwfdE1wf+mFLFpJWBTPo67jGKrb\n\
LwBFFGDUkYwruazKO45mYfL8at4cdLkse7J0lPG3LsdFQs1cJdWHkHyzX+AkIWm9\n\
X9FMhiaJ7LJTWxME+k9RkTXWUxWKx2FD7zHg2V4lodK72KyKJDok1RvZFzFzVoOH\n\
ObPzPzECgYEA8GNgqfllssWRyrUZl0A3eb+1HZyqq8icg31/eyoPWNy9FeUhsV8g\n\
VwLZpq80qJMsMX6j2AcfPEUx+Fuwz05hpuN66mz+/xiZvkIb1b6svhjwWhi+w4Td\n\
jd6GDFVzU5SIsqKP2ux9TilhxWU13c4+VHPzWgvoGiCr6elGc52Xvh0CgYEA1075\n\
3dYdQ8KxJ5yNXo8d8yVZMWIlTfQVMq1nCe5snGjZYq7/tHiZPATn0djCoJL3PKdP\n\
QII3zGNsfpXB/3Et1ngWkB/52lX3WmKnkuVYdSW2ItoQk8U3Udx8FHXmJnYKQRWj\n\
L6Gr8rHkv4Eo5/KfPLBCqxnEZ0XJz1/j6WGV/UMCgYEAl/fLSUlQIIdEFqkMh5Di\n\
QUaPoivDvZuyTnINIHjDgQLzSp8zAKXJlRhJLlX+7T81JgkQXWaLOZfo8kBM85Fy\n\
rIWM1ByZy/zqXNwAKs1C+V5cvCh+mK4rPh/aEHwIz2LMVFnupZDx2nLQy3CjcBiN\n\
UOiIhHmYfsP+yn5duoN7RYUCgYAy86CTy7ivMWKlAho+V1X6FUuPMS+sy+c8T+bD\n\
ZeXT+cf/33PprGRu3KZTdZ1RTJYvh1Rs6qPzvU+OgTggkBMN3utP/hEthwx7Gj+X\n\
tqiBSIRaqQolPhWCTNAcM25cibCRiGJOwk6jp09/tjwM79CZFwkf3oApBE3mwkV4\n\
FDzfawKBgQCr4reyuVg77aVExz2NgJnhzFthO+RshQ8uY0TnQ7ZRanqzCXrM01N5\n\
DO9hu69F9P0wNoLj2dPrLeei4HbHL/9gJbgkWyU+OUTCOogSe6crFogChN/KF35+\n\
Sb9us2EF0D4BHhQ3tetf7Ihfhw6IlBVIjqmT9SP6I1M558yD5E+MzA==\n\
-----END RSA PRIVATE KEY-----\n";

void Mqtt_0(void* p)
{
    int rc = 0;
    char * host = DEMO_MQTT_HOST;
    char * client_id = DEMO_MQTT_PRODUCT_KEY;
    char * username = DEMO_USER_NAME;
    char * password = DEMO_USER_PWD;
    
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
    mqtt_packet.keepalive_sec = 30;
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
    publish.topic = DEMO_PUB_TOPIC;
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

    while (1)
    {
        /* code */
        Paytm_delayMilliSeconds(10 * 1000);
        Paytm_TRACE("Free Heap Info: %d", Paytm_GetFreeHeapSize());
    }
}