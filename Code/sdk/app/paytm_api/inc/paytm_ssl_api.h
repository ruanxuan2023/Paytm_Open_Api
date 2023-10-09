#ifndef PAYTM_SSL_API_H
#define PAYTM_SSL_API_H

#include "paytm_typedef.h"
#include "osi_api.h"

typedef enum{
    CERTIFICATE_NVRAM
}Enum_Certificate_Location;

#define MQTT_CA_LOC     "mqtt_ca.bin"
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

typedef struct 
{
    char * cert_name;
    char * cert_path;
}Paytm_certificate_t;

//clear mqtt & http global cert array data
int32 secure_connection_clear(secure_connection_t *ssl);

//copy the data in ssl to mqtt cert arrray
int32 secure_connection_mqtt_location(secure_connection_t *ssl, Enum_Certificate_Location location);

//copy the data in ssl to http cert arrray
int32 secure_connection_http_location(secure_connection_t *ssl, Enum_Certificate_Location location);

//calculate checksum of certs in ssl 
int32 Paytm_SSL_Certificate_ReadChecksum(secure_connection_t *ssl);

//found if the ssl file exists in flash
uint8 Paytm_SSL_Certificates_Found(const secure_connection_t *ssl);

//delete cert data in ssl and remove file which name is ssl->cert_location
int32 Paytm_SSL_Certificate_Delete(secure_connection_t *ssl);

//write the cert data to the file which file name is cert->cert_path + cert->name
int32 Paytm_SSL_Certificate_WriteContent(Paytm_certificate_t *cert, const char *certificate, uint32 certificate_len, uint16 timeout);

//write cert data to ssl
int32 Paytm_SSL_WriteCertificates(secure_connection_t *ssl, const char *_cacert, const char *_clientcert, const char *_clientkey);

//copy ssl data to http global ssl array
int32 Paytm_SSL_Config_Http(secure_connection_t *ssl);

#endif