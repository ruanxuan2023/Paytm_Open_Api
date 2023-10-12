#ifndef PAYTM_HTTP_API_H
#define PAYTM_HTTP_API_H

#include "paytm_ssl_api.h"
#include "paytm_typedef.h"
#include "osi_api.h"

#define HTTP_URL_LEN (128)
#define FS_FILE_PATH_MAX (32)
#define FS_DIR_NAME_MAX (32)
#define HTTP_BUF_SIZE (2 * 1024)

typedef enum{
    HTTP_GET_REQUEST = 0,
    HTTP_POST_REQUEST,
    HTTP_PUT_REQUEST, 
    HTTP_HEAD_REQUEST
}Paytm_http_action_type_t;

typedef enum{
    HTTP_CONTENT_ANY = 0,
    HTTP_CONTENT_JSON,
    HTTP_CONTENT_ZIP,
    HTTP_CONTENT_XML,
    HTTP_CONTENT_HTML,
    HTTP_CONTENT_TEXT,
    HTTP_CONTENT_OCTET_STREAM,
    HTTP_CONTENT_URL_ENCODED,
    HTTP_CONTENT_TOTAL
}Paytm_http_content_type_t;

typedef enum{
    STATE_HTTP_SOCKET_SEND = 0,
    STATE_HTTP_SOCKET_READ_HEAD,
    STATE_HTTP_SOCKET_READ_BODY,
    STATE_HTTP_CREATE,
    STATE_HTTP_INTERRUPT,
    STATE_HTTP_DOWNLOAD_COMPLETE,
    STATE_FILE_UNZIP_COMPLETE,
    STATE_HTTP_READ_COMPLETE,
    STATE_HTTP_READ_ERROR_COMPLETE,
    STATE_HTTP_ERROR,
    STATE_UNZIP_FILE,
    STATE_FIRMWARE_URGARDE_START
}http_subtask_userstate_t;

typedef struct 
{
    /* data */
    int32 http_code; 
    uint32 ContentLength;
    Paytm_http_content_type_t ContentType;
    uint8 content_length_exist : 1,
    chunked_encoding : 1,
    keep_alive : 1;
    uint8 percent;

}Paytm_http_header_type_t;

//--TODO 缺少两个变量定义
typedef struct 
{
    /* data */
    Paytm_http_action_type_t action;
    uint16 timeout_sec;
    char server_url[HTTP_URL_LEN + 1];
    uint16 port;
    Paytm_http_content_type_t content_type;
    Paytm_http_content_type_t accept_type;
    uint32 content_size;
    char *content;
    char *signature;
    char *authorization;
    char *cache_control;
    char *custom_headers;
    int verify_mode;                /*0-VERIFY_NONE 1-VERIFY_OPTIONAL 2-VERIFY_REQUIRED 3-UNSET*/
    bool use_ssl;
    int32 error_code;
    //sb_location_t storage_location;
    char filepath[FS_FILE_PATH_MAX];
    char unzip_path[FS_DIR_NAME_MAX];
    Paytm_http_header_type_t response_head;
    char *response_buffer;
    size_t response_buffer_size;
    size_t response_buffer_read_length;
    secure_connection_t ssl;
}Paytm_http_request_Packet_t;

typedef struct
{
    uint16_t status_code;   //200 400 403 404 etc
    uint8_t * body;
}Paytm_http_response_t;

typedef Paytm_http_request_Packet_t http_request_t;

int32 Paytm_HTTP_Request_Clear(http_request_t *http);
int32 Paytm_HTTP_Initialise_GET(Paytm_location_t lc, http_request_t *http, const char *url, uint16 port, const char *file);
int32 Paytm_HTTP_Initialise_POST(http_request_t *http, const char *url, char *signature, char *content, uint8 enable_ssl);
int32 Paytm_HTTP_Initialise_PUT(http_request_t *http, const char *url, char *content, Paytm_http_content_type_t content_type);
#endif  