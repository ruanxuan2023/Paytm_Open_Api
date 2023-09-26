#include "paytm_typedef.h"
#include "paytm_http_api.h"

#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "httpclient.h"

#define BUF_SIZE (4 * 1024)
int32 Paytm_HTTP_Request_Clear(http_request_t *http);

int32 Paytm_HTTP_Initialise_GET(Paytm_location_t lc, http_request_t *http, const char *url, uint16 port, const char *file)
{
    httpclient_t client = {0};
    httpclient_data_t client_data = {0};
    HTTPCLIENT_RESULT ret;
    char *buf, *header;
    int val_pos, val_len;

    buf = malloc(BUF_SIZE);    
    header = malloc(BUF_SIZE);
    if (buf == NULL || header == NULL) {        
        return -1;
    }
    memset(buf, 0, BUF_SIZE);
    memset(header, 0, BUF_SIZE);
    
    client_data.header_buf = header;    
    client_data.header_buf_len = BUF_SIZE;
    client_data.response_buf = buf;
    client_data.response_buf_len = BUF_SIZE;    
    client_data.response_buf[0] = '\0';

    client.remote_port = port;
    ret = httpclient_get(&client, url, &client_data);
    if (ret < 0)
        goto fail;   

    // get response header
    if(0 == httpclient_get_response_header_value(client_data.header_buf, "Content-length", &val_pos, &val_len))
        RTI_LOG("Content-length: %.*s", val_len, client_data.header_buf + val_pos);

    //结果拷贝在参数 http 中, 部分参数还不支持
    http->response_head.http_code = client.response_code;
    http->response_head.ContentLength = client_data.response_content_len;
    //http->response_head.percent = 
    http->response_head.chunked_encoding = client_data.is_chunked;

    //resp body 部分
    http->response_buffer_read_length = client_data.header_buf_len;
    memcpy(http->response_buffer, client_data.response_buf, client_data.response_buf_len);

    //整个resp content 由resp_header + resp_body
    memcpy(http->content, client_data.header_buf, client_data.header_buf_len);
    memcpy(http->content + client_data.header_buf_len, client_data.response_buf, client_data.response_buf_len);
    http->content_size = client_data.header_buf_len + client_data.response_buf_len;

fail:
    free(buf);
    free(header);

    return ret;
}

int32 Paytm_HTTP_Initialise_POST(http_request_t *http, const char *url, char *signature, char *content, uint8 enable_ssl)
{
    httpclient_t client = {0};
    httpclient_data_t client_data = {0};
    char *buf, *header;
    HTTPCLIENT_RESULT ret;
    int val_pos, val_len;

    buf = malloc(BUF_SIZE);    
    header = malloc(BUF_SIZE);
    if (buf == NULL || header == NULL) {        
        return -1;
    }
    memset(buf, 0, BUF_SIZE);
    memset(header, 0, BUF_SIZE);

    client_data.header_buf = header;    
    client_data.header_buf_len = BUF_SIZE;
    client_data.response_buf = buf;
    client_data.response_buf_len = BUF_SIZE;    
    client_data.response_buf[0] = '\0';
    client_data.post_buf = content;
    client_data.post_buf_len = strlen(content);

    if(enable_ssl)
    {
        client.is_http = 0;
        client.auth_password = signature;
        //http.ca http.cert http.key 需要文件系统，后续完善
        // client.client_cert = NULL;
        // client.client_cert_len = 0;
        // client.client_pk = NULL;
        // client.client_pk_len = 0;
    }
    ret = httpclient_post(&client, url, &client_data);
    if (ret < 0)
        goto fail;  

    if(0 == httpclient_get_response_header_value(client_data.header_buf, "Content-length", &val_pos, &val_len))
        RTI_LOG("Content-length: %.*s", val_len, client_data.header_buf + val_pos);
    
    //结果拷贝在参数 http 中, 部分参数还不支持
    http->response_head.http_code = client.response_code;
    http->response_head.ContentLength = client_data.response_content_len;
    //http->response_head.percent = 
    http->response_head.chunked_encoding = client_data.is_chunked;

    //resp body 部分
    http->response_buffer_read_length = client_data.header_buf_len;
    memcpy(http->response_buffer, client_data.response_buf, client_data.response_buf_len);

    //整个resp content 由resp_header + resp_body
    memcpy(http->content, client_data.header_buf, client_data.header_buf_len);
    memcpy(http->content + client_data.header_buf_len, client_data.response_buf, client_data.response_buf_len);
    http->content_size = client_data.header_buf_len + client_data.response_buf_len;

    ret = 0;
fail:
    free(buf);
    free(header);

    return ret;
}

int32 Paytm_HTTP_Initialise_PUT(http_request_t *http, const char *url, char *content, Paytm_http_content_type_t content_type)
{
    httpclient_t client = {0};
    httpclient_data_t client_data = {0};
    HTTPCLIENT_RESULT ret;
    int val_pos, val_len;
    char *buf = NULL;
    buf = pvPortMalloc(BUF_SIZE);
    if (buf == NULL) {
        RTI_LOG1("Malloc failed.\r\n");

    memset(buf, 0, sizeof(buf));
    client_data.response_buf = buf;  //Sets a buffer to store the result.
    client_data.response_buf_len = BUF_SIZE;  //Sets the buffer size.
    client_data.post_buf = content;  //Sets the user data to be put.
    client_data.post_buf_len = strlen(content);  //Sets the put data length.
    //client_data.post_content_type = content_type;  //Sets the content type.
    ret = httpclient_put(&client, url, &client_data);
    
    if(ret < 0)
        goto fail;
    printf("Data received: %s\r\n", client_data.response_buf);

    if(0 == httpclient_get_response_header_value(client_data.header_buf, "Content-length", &val_pos, &val_len))
        HTTPC_LOG("Content-length: %.*s", val_len, client_data.header_buf + val_pos);
    
    //结果拷贝在参数 http 中, 部分参数还不支持
    http->response_head.http_code = client.response_code;
    http->response_head.ContentLength = client_data.response_content_len;
    
    //resp body 部分
    http->response_buffer_read_length = client_data.header_buf_len;
    memcpy(http->response_buffer, client_data.response_buf, client_data.response_buf_len);

    //整个resp content 由resp_header + resp_body
    memcpy(http->content, client_data.header_buf, client_data.header_buf_len);
    memcpy(http->content + client_data.header_buf_len, client_data.response_buf, client_data.response_buf_len);
    http->content_size = client_data.header_buf_len + client_data.response_buf_len;

fail:
    free(buf);

    return ret;
}
}