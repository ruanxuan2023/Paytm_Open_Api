#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "mbedtls/ssl.h"

#include "paytm_dev_api.h"
#include "paytm_file_api.h"
#include "paytm_http_api.h"
#include "paytm_net_api.h"

#define HTTP_GET_URL    "http://www.kuaidi100.com/query?type=shunfeng&postid=SF1420349064432"
#define HTTP_POST_URL   "http://pay.chinainfosafe.com:6003"
#define HTTPS_URL       "https://www.chinainfosafe.com:1883"
static char *post_data = "Hello chinainfosafe";

extern const char http_client_key[1705];
extern const char http_client_cert[1173];
extern const char http_server_cert[4789];


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

void testHttpGet(void * p)
{
    int rc = 0;
    char *get_url = HTTPS_URL;
    secure_connection_t http_ssl = {0};

    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.verify_mode = MBEDTLS_SSL_VERIFY_REQUIRED;
    
    http.use_ssl = true;
    http_ssl.cacert = (char*)http_server_cert;
    http_ssl.clientcert = (char*)http_client_cert;
    http_ssl.clientkey = (char*)http_client_key;

    Paytm_SSL_Clear_Http();
    Paytm_SSL_Config_Http(&http_ssl);

    rc = Paytm_HTTP_Initialise_GET(LOC_EXTER_MEM, &http, get_url, 0, NULL);
    if(rc < 0)
    {
        Paytm_TRACE("Get fail!");
    }

    Paytm_TRACE("Header: %s", http.content);
    Paytm_TRACE("rspBuf: %s", http.response_buffer);

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

void testHttpPost(void * p)
{
    int rc = 0;
    char *post_url = HTTP_POST_URL;

    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);

    rc = Paytm_HTTP_Initialise_POST(&http, post_url, NULL, post_data, false);
    if(rc < 0)
    {
        Paytm_TRACE("Post fail!");
    }

    Paytm_TRACE("Header: %s", http.content);
    Paytm_TRACE("rspBuf: %s", http.response_buffer);

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

void testHttpSSLPost(void * p)
{
    int rc = 0;
    char *post_url = HTTPS_URL;
    secure_connection_t http_ssl = {0};

    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.verify_mode = MBEDTLS_SSL_VERIFY_REQUIRED;

    http.use_ssl = true;
    http_ssl.cacert = (char*)http_server_cert;
    http_ssl.clientcert = (char*)http_client_cert;
    http_ssl.clientkey = (char*)http_client_key;

    Paytm_SSL_Config_Http(&http_ssl);

    rc = Paytm_HTTP_Initialise_POST(&http, post_url, NULL, post_data, true);
    if(rc < 0)
    {
        Paytm_TRACE("Post fail! Rc = %d", rc);
    }

    Paytm_TRACE("Header: %s", http.content);
    Paytm_TRACE("rspBuf: %s", http.response_buffer);

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

#define HEAD_URL    "https://cisfs.oss-cn-shenzhen.aliyuncs.com/600RGBFFD20G_0253_0255.bin_2"
#define TAO_BAO_URL "https://suggest.taobao.com/sug?q=number"
//HTTPS Without Certificate
void getTaobaoSuggest(void* p)
{
    int rc = 0;
    char *get_url = "https://suggest.taobao.com/sug?q=number";
    char *cis_url = "https://cisfs.oss-cn-shenzhen.aliyuncs.com/600RGBFFD20G_0253_0255.bin_2";

    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.verify_mode = MBEDTLS_SSL_VERIFY_NONE;
    
    http.use_ssl = true;

    rc = Paytm_HTTP_Initialise_GET(LOC_EXTER_MEM, &http, cis_url, 0, NULL);
    if(rc < 0)
    {
        Paytm_TRACE("Get fail!");
    }

    Paytm_TRACE("Header: %s", http.content);
    Paytm_TRACE("rspBuf: %s", http.response_buffer);
    Paytm_TRACE("RspCode: %d", http.response_head.http_code);

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

extern void fileUnzip(void);
#define SINGLE_DOWNLOAD_LEN     (5 * 1024)

void httpDownload(void* p)
{
    int rc = 0;
    char *cis_url = "https://cisfs.oss-cn-shenzhen.aliyuncs.com/600RGBFFD20G_0253_0255.bin_2";


    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);

    http.verify_mode = MBEDTLS_SSL_VERIFY_NONE;
    http.use_ssl = true;
    http.is_chunked = false;

    rc = Paytm_HTTP_Initialise_HEAD(LOC_EXTER_MEM, &http, cis_url, 0);
    if(rc < 0)
    {
        Paytm_TRACE("Get fail!");
    }

    Paytm_TRACE("Header: %s", http.content);

    int start, end;
    char *sStr, *pStr, *subStr;

    sStr = strstr(http.content, "Content-Length:");
    if(sStr == NULL)
    {
        goto exit;
    }

    http.custom_headers = (char*)Paytm_malloc(128);
    start = sStr - http.content;
    pStr = strstr(sStr, "\n");
    end = pStr - http.content;

    subStr = (char*)Paytm_malloc(end - start + 1);
    memcpy(subStr, sStr, end - start);
    subStr[end - start] = '\0';

    size_t file_size = 0;

    sscanf(subStr, "Content-Length: %d", &file_size);
    Paytm_TRACE("Content %s, size %d", subStr, file_size);

    //start to recv data
    int get_len_this_time = 0, get_len_sum = 0, real_get_len = 0;
    
    start = 0;
    end = 0;
    //create file
    PFILE fd;
    Paytm_fremove(LOC_EXTER_MEM, "paytm_res_en.czip");
    if(Paytm_fexists(LOC_EXTER_MEM, "paytm_res_en.czip") != 0)
    {
        fd = Paytm_fcreate(LOC_EXTER_MEM, "paytm_res_en.czip", "wb+");
        if(fd <= 0)
        {
            Paytm_TRACE("Create file failed");
            goto exit;
        }
    }else{
        fd = Paytm_fopen(LOC_EXTER_MEM, "paytm_res_en.czip", "wb+");
        if(fd <= 0)
        {
            Paytm_TRACE("Create open failed");
            goto exit;
        }
    }

    while (get_len_sum < file_size)
    {
        get_len_this_time = (file_size - get_len_sum) > HTTP_BUF_SIZE ? HTTP_BUF_SIZE : (file_size - get_len_sum);
        start = get_len_sum;
        end = start + get_len_this_time;

        sprintf(http.custom_headers,
        "Range: bytes=%d-%d\r\n\r\n", start, end - 1);

        rc = Paytm_HTTP_Initialise_GET(LOC_EXTER_MEM, &http, cis_url, 0, NULL);
        if(rc < 0)
        {
            Paytm_TRACE("Get range buffer failed!");
            break;
        }
        
        Paytm_TRACE("http->response_buffer_read_length: %d lastByte: 0x%x", http.response_buffer_read_length,http.response_buffer[http.response_buffer_read_length-1]);

        Paytm_fseek(fd, get_len_sum, Paytm_File_Begin);
        Paytm_fwrite(http.response_buffer, 1, http.response_buffer_read_length, fd);

        get_len_sum += http.response_buffer_read_length;
        memset(http.response_buffer, 0x00, HTTP_BUF_SIZE);
        Paytm_TRACE("Range: %d/%d, To get: %d, Real get %d/%d\n\n", start, end - 1, get_len_this_time, get_len_sum, file_size);
    }

    Paytm_fclose(fd);

    fileUnzip();
exit:
    if(http.custom_headers != NULL)
    {
        Paytm_free(http.custom_headers);
    }

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

void fota_download(void* p)
{
    int rc = 0;

    char *fota_url = "https://cisfs.oss-cn-shenzhen.aliyuncs.com/fota_test_file.bin";

    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);

    http.verify_mode = MBEDTLS_SSL_VERIFY_NONE;
    http.use_ssl = true;
    http.is_chunked = false;

    rc = Paytm_HTTP_Initialise_HEAD(LOC_EXTER_MEM, &http, fota_url, 0);
    if(rc < 0)
    {
        Paytm_TRACE("Get fail!");
    }

    Paytm_TRACE("Header: %s", http.content);

    int start, end;
    char *sStr, *pStr, *subStr;

    sStr = strstr(http.content, "Content-Length:");
    if(sStr == NULL)
    {
        goto exit;
    }

    http.custom_headers = (char*)Paytm_malloc(128);
    start = sStr - http.content;
    pStr = strstr(sStr, "\n");
    end = pStr - http.content;

    subStr = (char*)Paytm_malloc(end - start + 1);
    memcpy(subStr, sStr, end - start);
    subStr[end - start] = '\0';

    size_t file_size = 0;

    sscanf(subStr, "Content-Length: %d", &file_size);
    Paytm_TRACE("Fota file size =  %d", file_size);

    //start to recv data
    int get_len_this_time = 0, get_len_sum = 0, real_get_len = 0;
    
    start = 0;
    end = 0;
    //create file
    PFILE fd;
    if(Paytm_fexists(LOC_INTER_MEM, PAYTM_FOTA_FILE) != 0)
    {
        fd = Paytm_fcreate(LOC_INTER_MEM, PAYTM_FOTA_FILE, "wb+");
        if(fd <= 0)
        {
            Paytm_TRACE("Create file failed");
            goto exit;
        }
    }else{
        fd = Paytm_fopen(LOC_INTER_MEM, PAYTM_FOTA_FILE, "rb+");
        if(fd <= 0)
        {
            Paytm_TRACE("Create open failed");
            goto exit;
        }
    }

    while (get_len_sum < file_size)
    {
        get_len_this_time = (file_size - get_len_sum) > HTTP_BUF_SIZE ? HTTP_BUF_SIZE : (file_size - get_len_sum);
        start = get_len_sum;
        end = start + get_len_this_time;

        sprintf(http.custom_headers,
        "Range: bytes=%d-%d\r\n\r\n", start, end - 1);

        rc = Paytm_HTTP_Initialise_GET(LOC_EXTER_MEM, &http, fota_url, 0, NULL);
        if(rc < 0)
        {
            Paytm_TRACE("Get range buffer failed!");
            break;
        }
        
        Paytm_TRACE("http->response_buffer_read_length: %d lastByte: 0x%x", http.response_buffer_read_length,http.response_buffer[http.response_buffer_read_length-1]);

        Paytm_fseek(fd, get_len_sum, Paytm_File_Begin);
        Paytm_fwrite(http.response_buffer, 1, http.response_buffer_read_length, fd);

        get_len_sum += http.response_buffer_read_length;
        memset(http.response_buffer, 0x00, HTTP_BUF_SIZE);
        Paytm_TRACE("Range: %d/%d, To get: %d, Real get %d/%d\n\n", start, end - 1, get_len_this_time, get_len_sum, file_size);
    }

    Paytm_fclose(fd);

    /* Then we check the fota file validation, Set fota file ready  */
    if(!Paytm_SetFOTAFileReady(PAYTM_FOTA_FILE))
    {
        Paytm_TRACE("The fota file is invlid or not ready");
        goto exit;
    }

    /* reset the device */
    Paytm_resetForFOTA();

exit:
    if(http.custom_headers != NULL)
    {
        Paytm_free(http.custom_headers);
    }

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
