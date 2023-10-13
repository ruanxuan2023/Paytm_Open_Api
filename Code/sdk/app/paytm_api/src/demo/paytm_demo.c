#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "mbedtls/ssl.h"

#include "paytm_http_api.h"
#include "paytm_mqtt_api.h"
#include "paytm_net_api.h"

#define DEMO_MQTT_HOST  "a3ri26a4bhtu9s-ats.iot.ap-south-1.amazonaws.com"
#define DEMO_MQTT_PORT			8883

#define DEMO_MQTT_PRODUCT_KEY 	"qa3-iot-int-WAM4GS-864180051513708"
#define DEMO_USER_NAME   		""
#define DEMO_USER_PWD 			""

#define AWS_JOB_NOTIFY_NEXT_TOPIC "$aws/things/qa3-iot-int-WAM4GS-864180051513708/jobs/notify-next"
#define AWS_JOB_START_NEXT_TOPIC "$aws/things/qa3-iot-int-WAM4GS-864180051513708/jobs/start-next"
#define AWS_JOB_START_NEXT_ACCEPTED_TOPIC "$aws/things/qa3-iot-int-WAM4GS-864180051513708/jobs/start-next/accepted"
#define AWS_JOB_START_NEXT_REJECTED_TOPIC "$aws/things/qa3-iot-int-WAM4GS-864180051513708/jobs/start-next/rejected"
#define AWS_JOB_UPDATE_REJECTED_TOPIC "$aws/things/qa3-iot-int-WAM4GS-864180051513708/+/update/rejected"
#define AWS_JOB_PLUS_UPDATE_TOPIC "$aws/things/qa3-iot-int-WAM4GS-864180051513708/jobs/+/update"
#define AWS_JOB_STATUS_UPDATE_TOPIC "$aws/things/qa3-iot-int-WAM4GS-864180051513708/jobs/"
#define AUTOMATION_INFO_TOPIC "qa3-iot-int/client/ab7017d2-e7ca-4ef3-a7d0-f8dbefc6b891_WAM4GS_864180051513708"


#define DEMO_PUB_TOPIC          "$aws/rules/qa3_iot_int_command_state"
#define DEMO_PUB_DATA                 "{\"v\":\"V2.0.0_HI\",\"deviceId\":\"864180051513708\",\"deviceType\":\"WAM4GS\",\"firmwareVersion\":\"V2.0.0_HI\",\"firmwareLanguage\":\"HI\",\"resourceVersion\":\"V0.0.0_HI\",\"id\":\"864180051513708\",\"typ\":\"WAM4GS\",\"rId\":\"84\",\"c\":\"state\"}"


const char  mqtt_cacert[] = "-----BEGIN CERTIFICATE-----\n\
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n\
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n\
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n\
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n\
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n\
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n\
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n\
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n\
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n\
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n\
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n\
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n\
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n\
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n\
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n\
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n\
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n\
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n\
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n\
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n\
-----END CERTIFICATE-----\n";


const char  mqtt_clientcert[] = "-----BEGIN CERTIFICATE-----\n\
MIIDWjCCAkKgAwIBAgIVAN8V9e9GzjMRaRFywREKRjyw6JxJMA0GCSqGSIb3DQEB\n\
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n\
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMzA2MTMxMTUz\n\
MjZaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n\
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDP+IT30fqpNO9fyceD\n\
v83AFdTLa/pHdWMVYMieJT0KBEKZsYejtF2bphH0a5gSU+p89wpJl8aI/Kg/fwgN\n\
iiazDOafvRW3cE7BEdr6pJ1wxKNAC6uHm0PYBPojbXzFYxJNR3wypbsWQBKKjSPE\n\
YsjlXxFykME9mxmt9f5B8lffmbcyDgiWHO1FkZuERSnd2qkDIuWKfNqSjtMeIacT\n\
KzhxtVQMOtX/mY9tOstOZGM6t/h5lxq1fS0CBGjqVrWYJQf9UBBOGRZbC5ZAj30G\n\
TOiOlOWpcNFsw621gjIPBpO3YOyrbqRnD7qLquI925CIkZFKqXgxtsWEYLvktq+r\n\
XJVLAgMBAAGjYDBeMB8GA1UdIwQYMBaAFGYWsSE2kQc51BHU/fonlXUD8yXoMB0G\n\
A1UdDgQWBBTZ7HZ14BXCwBqtmwhPTO8wyV976TAMBgNVHRMBAf8EAjAAMA4GA1Ud\n\
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEALvZ0Kn+aSygxkvoSoElHO8Yp\n\
tpiLcH7im5epW1jT/TAxE402HGEKif8hk/87DcPNsalQRPhiOgmKCmqRUkDu9HjG\n\
Z/1rNS7eiX+z+GsKXIVJsqQLjCxczzPZPSgmZstUTv2KnokfSB6FhauAbvlSrvbP\n\
7ukNdye5LpvGpTQwevyPJGvWLWqZBUvCV6//T5yi2eceY6aZmLwXGWxFENFKnz+2\n\
rPy9xUPGZK/hzFZXOfKzSxAeebXBIqh9Am2CYWmF9bVX2/hCeNKPZph052Qb9EZ5\n\
tGtHFZ2hIpDf+oNY1v9Rpg67Sy9sjz8r7TAfdTG6vbeAP0l1bM5IWmh6JYEQvg==\n\
-----END CERTIFICATE-----\n";

const char  mqtt_clientkey[] = "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpAIBAAKCAQEAz/iE99H6qTTvX8nHg7/NwBXUy2v6R3VjFWDIniU9CgRCmbGH\n\
o7Rdm6YR9GuYElPqfPcKSZfGiPyoP38IDYomswzmn70Vt3BOwRHa+qSdcMSjQAur\n\
h5tD2AT6I218xWMSTUd8MqW7FkASio0jxGLI5V8RcpDBPZsZrfX+QfJX35m3Mg4I\n\
lhztRZGbhEUp3dqpAyLlinzako7THiGnEys4cbVUDDrV/5mPbTrLTmRjOrf4eZca\n\
tX0tAgRo6la1mCUH/VAQThkWWwuWQI99BkzojpTlqXDRbMOttYIyDwaTt2Dsq26k\n\
Zw+6i6riPduQiJGRSql4MbbFhGC75Lavq1yVSwIDAQABAoIBACFEHGjToqe8Oun0\n\
icBuRsgJ9e7QB8m0YjQCNjTtufTHN6wD9LFf4XVLPCX/q+SKncB9d5zL9UrmNzdo\n\
523bAlOzpZQ5l6avBTnm1EMxX6mxGnfdJgPb9x8vtJ9EHSgpr7zCyYAEeIeXmh+l\n\
ceow7veZnF3Z/78N9y6XYaNE03+FejG3aoEWTaaoZpY/AFSGWgJTh40gpAzPqEMm\n\
INngH3A3ETrmp3b8e7E8ICq/X8qFYApB1tRs2RuBTuGBpNj82VtEE3x5hZXrGKTP\n\
e0Bg6twO+/cIyLer0mYzqI9zC8/4s/or1J0pN96UnWlcOlkJ+pDqkXmbOKEb8Zkx\n\
xaFvtZECgYEA6CYHfEG8aiHN/XyzDhQ9h8FjpLaN6/DyWZ7dSgj4Em2qePrbsQ2u\n\
ceWzXhPxD49JYFomQj3ThEteb3ckEj3aTEOof1Ow+bN590tbrwC7dIeR6ll+Bt8w\n\
wX/4TKCV8fnxiYky1FnC5V8JUHxKspdcbQ88yH0dDHndgrKKhToKbQMCgYEA5VaR\n\
Acele2x8sbia3IaQGcrB3oTz6NtlspjTZO11IAAGbLyiqHVHCruGUWReBN4G9uIf\n\
t/UR32ggVghYaG9fzL5VLXnCZJxqm5DOgc8OuXrXgzx9zHibo5s/o6oVnDhz9IMQ\n\
yf7x6huMpCU11aH6wwRHoYNNjK91gZlwDxxtUBkCgYEA2N/WNF4REDF+iCZ7/FYq\n\
WW3wkcs98MLLFbCVWPniDw1GqfjpTpHSe3M0y1nj5T5eNVZi0ZWCyd/DXGy4VRZV\n\
ntPCURk1h0EFW2KaR3ZuqWGRjpaGZxMRMGRF6ghHrKRs4qk6xCkLK09Km1dSTV+c\n\
KAKJLV+8yI/8wYHcaLQdU1MCgYBvwJoScvWpSfkNJPaZZRzM5KdJxiGQo3BP8fZ4\n\
pNlBCaIX1GeyjMtcaHqQKMdDuelM3m00V8Aoy/WN1q013qeg/lJFQaK/IhU0zwBB\n\
8uKq8TgZJB3syAyXUaxWFShIop2mVUOWGpxJRrlmtxcfjbnC09AP3Qqp9kndKCaZ\n\
xnFh6QKBgQDa4C3V+/s9kE9e2kQTh+KwFezqyC5Tmf4yALFJ9LZKTeYiu6OsZvjH\n\
a88sw/ada5iGFd3oGO7RC6Wnau4NDE9lN3J+bXKr+kYiY4eifGlNFlupWOR7Jvgx\n\
7q8D0LUMljMLZvfurF0UIsZxx6j+XqTDm65BMF1ctYQOCV51hKQ6Og==\n\
-----END RSA PRIVATE KEY-----\n";

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

void PaytmMqtt(void* p)
{
    Paytm_TRACE("testMqtt starts");
    int rc = 0;
    char * host = DEMO_MQTT_HOST;
    char * client_id = DEMO_MQTT_PRODUCT_KEY;
    char * username = DEMO_USER_NAME;
    char * password = DEMO_USER_PWD;

    net_connect();
    
    // config MQTT ca_cert&client_cert&client_privatekey
    Paytm_SSL_Clear_Mqtt();
    Paytm_MQTT_WriteCertificates(mqtt_cacert, mqtt_clientcert, mqtt_clientkey);

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

    Paytm_TRACE("mqtt_packet.host: %s", mqtt_packet.host);
    Paytm_TRACE("mqtt_packet.port: %d", mqtt_packet.port);
    Paytm_TRACE("mqtt_packet.client_id: %s", mqtt_packet.client_id);
    Paytm_TRACE("mqtt_packet.username: %s", mqtt_packet.username);
    Paytm_TRACE("mqtt_packet.password: %s", mqtt_packet.password);
    Paytm_TRACE("mqtt_packet.enable_ssl: %d", mqtt_packet.enable_ssl);

    rc = Paytm_MQTT_Initialise(NULL, CERTIFICATE_NVRAM, &mqtt_packet);
    if(rc < 0)
    {
        Paytm_TRACE("Mqtt init fail %d!", rc);
        return;
    }else{
        Paytm_TRACE("Mqtt init success!");
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
        Paytm_TRACE("Mqtt socket open success!");
    }

    rc = Paytm_MQTT_Connect();
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt socket connect fail %d!", rc);
        return;
    }else{
        Paytm_TRACE("Mqtt socket connect success!");
    }

    topic_list.topic[0] = AUTOMATION_INFO_TOPIC;
    topic_list.qos[0] = Paytm_QOS1_AT_LEASET_ONCE;
    topic_list.count = 1;

    rc = Paytm_MQTT_Subscribe(&topic_list, topic_handler);
    if(rc != 0)
    {
        Paytm_TRACE("Mqtt subscribe fail 0x%x!", rc);
    }else{
        Paytm_TRACE("Mqtt subscribe success!");
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
    }else{
        Paytm_TRACE("Mqtt publish success!");
    }

    while (1)
    {
        /* code */
        osiThreadSleep(1000);
    }
}

char paytm_http_cert[1339] = {"-----BEGIN CERTIFICATE-----\n\
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n\
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n\
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n\
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n\
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n\
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n\
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n\
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n\
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n\
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n\
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n\
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n\
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n\
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n\
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n\
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n\
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n\
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n\
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n\
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n\
-----END CERTIFICATE-----\n"};
#define HTTPS_POST_PAYTM_URL "https://qa2-iot-int.paytm.com/iot/iotservice/device/onboard"
#define SIGNATURE       "signature: qN0dpBUZG1QyScjZHydM+5abymkf9ZmhTLMpaibueEVe+W29M2xtzpuobvmzwrZ/aIKKe7FSpArXKPMTDF7HCBqeJ9/3K3pmwavqUzjGWCU="
#define HTTPS_POST_PAYLOAD "{\"id\":\"864180051473234\",\"type\":\"WAM4GS\"}"
void PaytmHttpSSLPost(void * p)
{
    Paytm_TRACE("Inside  testHttpSSLPost");
    int rc = 0;
    // char *post_url = HTTPS_URL;
    char *post_url = HTTPS_POST_PAYTM_URL;
    secure_connection_t http_ssl = {0};

    net_connect();

    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.verify_mode = MBEDTLS_SSL_VERIFY_OPTIONAL;
    http.signature = (char*)Paytm_malloc(100);

    strncpy(http.signature, SIGNATURE, 100);
    http.content_type = HTTP_CONTENT_JSON;

    http.use_ssl = true;
    http_ssl.cacert = (char*)paytm_http_cert;
    // http_ssl.clientcert = (char*)http_client_cert;
    // http_ssl.clientkey = (char*)http_client_key;

    Paytm_SSL_Config_Http(&http_ssl);

    rc = Paytm_HTTP_Initialise_POST(&http, post_url, NULL, HTTPS_POST_PAYLOAD, true);
    if(rc < 0)
    {
        Paytm_TRACE("Post fail! Rc = %d", rc);
    }
    Paytm_TRACE("content: %s", http.content);
    Paytm_TRACE("signature: %s", http.signature);
    Paytm_TRACE("rspBuf: %s", http.response_buffer);
    Paytm_TRACE("authorization: %s", http.authorization);
    Paytm_TRACE("content_size: %s", http.content_size);
    Paytm_TRACE("server_url: %s", http.server_url);
    Paytm_TRACE("custom_headers: %s", http.custom_headers);
    Paytm_TRACE("signature: %s", http.signature);
    Paytm_TRACE("signature: %s", http.error_code);
    Paytm_TRACE("signature: %s", http.response_head);

    if(http.response_buffer != NULL)
    {
        Paytm_free(http.response_buffer);
    }
    
    if(http.content != NULL)
    {
        Paytm_free(http.content);
    }

    if(http.signature != NULL)
    {
        Paytm_free(http.signature);
    }

    while (1)
    {
        osiThreadSleep(1000);
    }
}

#define HTTP_GET_PAYTM_URL "https://qa2-iot-int.paytm.com/iot/iotservice/device/v1/time"
void PaytmHttpsGet(void * p)
{
    Paytm_TRACE("Starting function testHttpsGet");
    int rc = 0;
    char *get_url = HTTP_GET_PAYTM_URL;
    // char *get_url = HTTPS_URL;
    secure_connection_t http_ssl = {0};

    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.verify_mode = MBEDTLS_SSL_VERIFY_OPTIONAL;
    
    http.use_ssl = true;
    http_ssl.cacert = (char*)paytm_http_cert;
    // http_ssl.clientcert = (char*)http_client_cert;
    // http_ssl.clientkey = (char*)http_client_key;

    Paytm_SSL_Clear_Http();
    Paytm_SSL_Config_Http(&http_ssl);

    rc = Paytm_HTTP_Initialise_GET(LOC_EXTER_MEM, &http, get_url, 0, NULL);
    if(rc < 0)
    {
        Paytm_TRACE("Get fail!");
    }

    Paytm_TRACE("Header: %s", http.content);
    Paytm_TRACE("rspBuf: %s", http.response_buffer);
    Paytm_TRACE("authorization: %s", http.authorization);
    Paytm_TRACE("content_size: %s", http.content_size);
    Paytm_TRACE("server_url: %s", http.server_url);
    Paytm_TRACE("custom_headers: %s", http.custom_headers);
    Paytm_TRACE("signature: %s", http.signature);
    Paytm_TRACE("signature: %s", http.error_code);
    Paytm_TRACE("signature: %s", http.response_head);


    if(http.response_buffer != NULL)
    {
        Paytm_free(http.response_buffer);
    }
    
    if(http.content != NULL)
    {
        Paytm_free(http.content);
    }

    while (1)
    {
        osiThreadSleep(1000);
    }
}