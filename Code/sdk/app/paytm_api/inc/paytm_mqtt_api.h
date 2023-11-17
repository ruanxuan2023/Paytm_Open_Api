#ifndef PAYTM_MQTT_API_H
#define PAYTM_MQTT_API_H

#include "paytm_typedef.h"
#include "paytm_ssl_api.h"
#include "osi_api.h"
#include "mqttclient.h"

#define MQTT_MAX_TOPIC (10)

#define MQTT_CA_FILE    "mqtt.ca"
#define MQTT_CERT_FILE  "mqtt.cert"
#define MQTT_KEY_FILE   "mqtt.key"

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
    bool enable_ssl;
    bool will_flag;
    int will_qos;
    int will_retain;
    char* will_topic;
    uint8_t* will_message;
    char *url;
    char *host;
    char *ip_addr;
    int port;
    char *client_id;
    char *username;
    char *password;
}Paytm_mqtt_connect_Packet_t;

typedef struct {
    char *topic[MQTT_MAX_TOPIC];
    Paytm_mqtt_qos_e qos[MQTT_MAX_TOPIC];
    uint8 count;
}Paytm_MQTT_topic_info_t;

typedef Paytm_MQTT_topic_info_t ST_MQTT_topic_info_t;

//--TODO 却上两个结构体定义
int32 Paytm_MQTT_Initialise(const char *url, Enum_Certificate_Location location, Paytm_mqtt_connect_Packet_t *mqtt_handle);
int32 Paytm_MQTT_Open();
int32 Paytm_MQTT_Connect();
int32 Paytm_MQTT_Subscribe(ST_MQTT_topic_info_t *mqtt_topic_info_t, message_handler_t global_cb);
int32 Paytm_MQTT_Publish(Paytm_mqtt_publish_Packet_t*);
int32 Paytm_MQTT_Close();

/**
 * @description: Check mqtt relevant cert file exists or not, return false or true
 * @return {*}
 */
uint8 Paytm_MQTT_Check_Certificates();

/**
 * @description: Copy mqtt certs pointer to mqtt module, and the pointer should be global pointer
 * @return {*}
 */
int32 Paytm_MQTT_WriteCertificates(const char *_cacert, const char *_clientcert, const char *_clientkey);

int32 Paytm_MQTT_IsConnected();
int32 Paytm_MQTT_Unsubscribe(Paytm_MQTT_topic_info_t*);
int32 Paytm_MQTT_Disconnect();
int32 Paytm_Mqtt_Recv_Register(char* topic, void* callback);

/**
 * @description: Clear mqtt certs configuration in mqtt module
 * @return {*}
 */
int32 Paytm_SSL_Clear_Mqtt(void);

/**
 * @description:                    Read mqtt certs data from flash memory
 * @param {Paytm_location_t} lc     LOC_INTER_MEM or LOC_EXTER_MEM
 * @param {char*} mqtt_ca           Pointer to save mqtt_ca data
 * @param {char*} mqtt_cert         Poniter to save mqtt_cert data
 * @param {char*} mqtt_key          Poniter to save mqtt_key data
 * @return {*}
 */
int32 Paytm_Read_MQTT_Certs(Paytm_location_t lc, char* mqtt_ca, char* mqtt_cert, char* mqtt_key);

/**
 * @description: Writer mqtt certs to flash memory                    
 * @param {Paytm_location_t} lc
 * @param {char*} mqtt_ca
 * @param {int32} ca_len
 * @param {char*} mqtt_cert
 * @param {int32} cert_len
 * @param {char*} mqtt_key
 * @param {int32} key_len
 * @return {*}
 */
int32 Paytm_Write_MQTT_Certs(Paytm_location_t lc, char* mqtt_ca, int32 ca_len, char* mqtt_cert, int32 cert_len, char* mqtt_key, int32 key_len);

#endif
