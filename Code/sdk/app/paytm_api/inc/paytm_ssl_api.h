#ifndef PAYTM_SSL_API_H
#define PAYTM_SSL_API_H

#include "paytm_typedef.h"
#include "osi_api.h"

typedef enum{
    CERTIFICATE_NVRAM
}Enum_Certificate_Location;

#define MQTT_CA_LOC  "mqtt_ca.bin"
#define MQTT_CERT_LOC   "mqtt_ca.crt"
#define MQTT_KEY_LOC    "mqtt_ca.key"

#define HTTP_CA_LOC     "http_ca.bin"
#define HTTP_CERT_LOC   "http_ca.cert"
#define HTTP_KEY_LOC    "http_ca.key"

typedef struct 
{
    char* cert_location;
    uint8_t *cacert;
    uint8_t *clientcert;
    uint8_t *clientkey;
}secure_connection_t;

static secure_connection_t self_secure_cert = {0};
//--TODO 未定义
typedef struct 
{
    /* data */
    int id;
}Paytm_certificate_t;

//清除预设的证书信息
int32 secure_connection_clear(secure_connection_t *ssl);

//设置mqtt 的 ssl证书，是设置证书文件地址还是填充证书文件内容
int32 secure_connection_mqtt_location(secure_connection_t *ssl, Enum_Certificate_Location location);

//设置http 的 ssl证书，是设置证书文件地址还是填充证书文件内容
int32 secure_connection_http_location(secure_connection_t *ssl, Enum_Certificate_Location location);

//计算这个证书文件的check_sum 256
int32 Paytm_SSL_Certificate_ReadChecksum(secure_connection_t *ssl);

//检查这个证书文件是否存在？--flash里查找此文件
uint8 Paytm_SSL_Certificates_Found(const secure_connection_t *ssl);

//删除这个证书文件--删除flash内文件名为ssl.location的文件
int32 Paytm_SSL_Certificate_Delete(secure_connection_t *ssl);

//将入参的证书信息写入cert--数据写入flash
int32 Paytm_SSL_Certificate_WriteContent(Paytm_certificate_t *cert, const char *certificate, uint32 certificate_len, uint16 timeout);

//将入参的证书、私钥等写入ssl_cert--数据写入flash
int32 Paytm_SSL_WriteCertificates(secure_connection_t *ssl, const char *_cacert, const char *_clientcert, const char *_clientkey);

//配置SSL可能用到的证书私钥、公钥、CA证书等等--从flash中读出来
int32 Paytm_SSL_Config_Http(secure_connection_t *ssl);

#endif