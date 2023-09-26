#ifndef PAYTM_MQTT_API_H
#define PAYTM_MQTT_API_H

#include "paytm_typedef.h"
#include "paytm_ssl_api.h"
#include "osi_api.h"

#define MQTT_MAX_TOPIC (10)

typedef enum{
    Paytm_QOS0_AT_MOST_ONECE = 0, //<At most once
    Paytm_QOS1_AT_LEASET_ONCE, //< At least once
    Paytm_QOS2_AT_EXACTLY_ONECE //< Exactly once
}Paytm_mqtt_qos_e;


typedef struct{
    Paytm_mqtt_qos_e qos;
    uint8 retain;
    uint8 messageId;
    char *topic;
    char *message;
    uint32 message_length;
}Paytm_mqtt_publish_Packet_t;


typedef struct 
{
    int connect_id;
    int version_num;
    int keepalive_sec;
    int timeout_sec;
    int cleansession;
    int show_recvlen;
    bool will_flag;
    int will_qos;
    int will_retain;
    char* will_topic;
    uint8_t* will_message;
    char url[128];
    char host[64];
    char ip_addr[20];
    int port;
    int client_id;
    char* username;
    char* password;
}Paytm_mqtt_connect_Packet_t;

typedef struct {
    char *topic[MQTT_MAX_TOPIC];
    Paytm_mqtt_qos_e qos[MQTT_MAX_TOPIC];
    uint8 count;
}Paytm_MQTT_topic_info_t;

typedef Paytm_MQTT_topic_info_t ST_MQTT_topic_info_t;

//--TODO 却上两个结构体定义
int32 Paytm_MQTT_Initialise(const char *url, Enum_Certificate_Location location);
int32 Paytm_MQTT_Open();
int32 Paytm_MQTT_Connect();
int32 Paytm_MQTT_Subscribe(ST_MQTT_topic_info_t *mqtt_topic_info_t);
int32 Paytm_MQTT_Publish(Paytm_mqtt_publish_Packet_t*);
int32 Paytm_MQTT_Close();
uint8 Paytm_MQTT_Check_Certificates();
int32 Paytm_MQTT_WriteCertificates(const char *_cacert, const char *_clientcert, const char *_clientkey);
int32 Paytm_MQTT_IsConnected();
int32 Paytm_MQTT_Unsubscribe(Paytm_MQTT_topic_info_t*);
int32 Paytm_MQTT_Disconnect();
int32 Paytm_Mqtt_Recv_Register(char* topic, void* callback);


#endif
