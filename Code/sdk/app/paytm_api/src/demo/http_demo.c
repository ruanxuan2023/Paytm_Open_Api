#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "mbedtls/ssl.h"

#include "paytm_dev_api.h"
#include "paytm_file_api.h"
#include "paytm_http_api.h"
#include "paytm_net_api.h"

#define CUSTOM_HEADER_LEN  (128)

#define HTTP_GET_URL    "http://www.kuaidi100.com/query?type=shunfeng&postid=SF1420349064432"
#define HTTP_POST_URL   "http://pay.chinainfosafe.com:6033"
#define HTTPS_URL       "https://www.chinainfosafe.com:1883"
static char *post_data = "Hello chinainfosafe";

extern const char http_client_key[1705];
extern const char http_client_cert[1173];
extern const char http_server_cert[4789];


OSI_UNUSED static void net_connect(void)
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

#define UPLOAD_FILE_NAME  "upload_file_test.bin"
#define UPLOAD_FILE_SIZE  (3*1024*1024)
static uint32_t crc32Update(uint32_t crc, const void *p, size_t len)
{
    uint32_t vds_crc32_table[256] = {
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
    0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
    0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
    0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
    0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
    0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
    0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
    0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
    0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
    0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
    0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
    0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
    0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
    0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
    0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
    0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
    0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
    0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
    0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
    0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
    0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
    0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
    0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
    0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
    0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
    0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
    0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
    0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
    0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
    0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
    0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
    0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
    0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
    0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
    0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
    0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
    0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
    0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
    0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
    0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
    0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
    0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
    0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
    0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
    0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
    0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
    0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
    0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
    0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
    0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
    0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
    0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL};
    if (p == NULL || len == 0)
        return crc;
    crc ^= 0xffffffff;
    const uint8_t *pc = (const uint8_t *)p;
    while (len-- > 0)
        crc = vds_crc32_table[(crc ^ *pc++) & 0xff] ^ (crc >> 8);
    return crc ^ 0xffffffff;
}
uint32_t cal_sum = 0;
static int prv_http_post_segment_cb(uint32 pos, char *content, uint32 contetn_len){
    // Also can send post data from file
    // 1.open file
    // 2.seek pos
    // 3.read data
    // 4.close file

    // write test data
    for(uint32_t i = 0; i < contetn_len; i++){
        content[i] = i%16+'a';
    }
    // calculate post data crc
    cal_sum = crc32Update(cal_sum, content, contetn_len);
    // return write data len
    return contetn_len;
}


void testHttpPostExt(void * p)
{
    int rc = 0;
    char *post_url = HTTP_POST_URL;

    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content_type = HTTP_CONTENT_JSON;
    cal_sum = 0;
    rc = Paytm_HTTP_Initialise_POST_EXT(&http, post_url, NULL, UPLOAD_FILE_SIZE, prv_http_post_segment_cb,  false);
    if(rc < 0)
    {
        Paytm_TRACE("Post fail!");
    }

    Paytm_TRACE("Header: %s", http.content);

    // print crc from test server , server calculate crc from all content {"crc": "0xeb793c4c"}
    Paytm_TRACE("rspBuf: %s", http.response_buffer);
    Paytm_TRACE("cal_sum: 0x%x", cal_sum);

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

void http_1MB_speed_test(void* p)
{
    int rc = 0;
    
    char *cis_url = "https://cisfs.oss-cn-shenzhen.aliyuncs.com/random.bin";

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

    Paytm_TRACE("Header: ");
    for(int i = 0; i < strlen(http.content); i++)
    {
        RTI_LOG1("%c", http.content[i]);
    }

    int start, end;
    char *sStr, *pStr, *subStr;

    sStr = strstr(http.content, "Content-Length:");
    if(sStr == NULL)
    {
        Paytm_TRACE("Can not find Content-Length in http.content");
        goto exit;
    }

    http.custom_headers = (char*)Paytm_malloc(CUSTOM_HEADER_LEN);
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
    int get_len_this_time = 0, get_len_sum = 0;
    
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
    Paytm_TRACE("Start download ticks: %d", osiGetTicks());
    while (get_len_sum < file_size)
    {
        get_len_this_time = (file_size - get_len_sum) > HTTP_BUF_SIZE ? HTTP_BUF_SIZE : (file_size - get_len_sum);
        start = get_len_sum;
        end = start + get_len_this_time;

        memset(http.custom_headers, 0, CUSTOM_HEADER_LEN);
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
    Paytm_TRACE("End download ticks: %d", osiGetTicks());
    Paytm_fclose(fd);
        Paytm_TRACE("Ext flash size after download: %d", Paytm_GetFreeExtFlashSize());
    // fileUnzip();
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

void httpDownload2(void* p)
{
    int rc = 0;
    char *cis_url = "https://payments-dev-iot.s3.amazonaws.com/lit/data.czip?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAQ67U57MGW2SN4WJD%2F20231108%2Fap-south-1%2Fs3%2Faws4_request&X-Amz-Date=20231108T130500Z&X-Amz-Expires=604800&X-Amz-SignedHeaders=host&X-Amz-Signature=fa60bf099e991b15e08ac863b0483658a77c9fa7f2bdb0f3ffc8345f79c3b291";
    // char *cis_url = "https://payments-dev-iot.s3.ap-south-1.amazonaws.com/lit/data.czip?response-content-disposition=inline&X-Amz-Security-Token=IQoJb3JpZ2luX2VjEIX%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEaCmFwLXNvdXRoLTEiRzBFAiBQaHRs3A9WBsfGLV4qxG8BlZXwNX7NKUHgsBo7UJ4vkgIhAKlltbUqoWU8UKhvfxiJJgQ2iFzN0nUVzRrPSpymg7XuKrIDCL7%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEQAhoMMDY2NTQ4NzkyMDc3IgwYJXh7708GxnwKhU0qhgO957zhBqfYAYPu3hvKxXQanB4WgVPWuYnT26FjY5jTUBvmRJii071oelWsYxWOBAk9vgiyNyysbWN4mwo6RZKDbnyX8u8pSAXSsqBnRUxmtriPwbI5tH%2FsBea8oGN6%2B%2FSKtZbIm7su5JzyWNZE81kEHWD7k0K75bOogq37VJkvXv%2BQqQV%2BqyyGjE87aY7dxKArEpK7W7oh%2BXVDlJ6gsCZrh5ReWEISP5YKm9cc1Vzo3Y5gExNn9urdNflAQvUilx5QxSghxXrwZjaVTX4hzYyxLTBs6qi5nLM2yv%2BsJ6OLMj1Ri%2FStbcyT8fQ4VHI4cdjFWezV9Zq9fFzfbRrpkrhYF3fc6iSgqaACCphAFRvbFvmU19DWgLgO%2FYQ0yRMTuqYF9wUlhJ7LviPRylioI36ZMv34Rda%2BQXPuMG5%2BX1f0VdoP%2BFVvRP3qtGdFzRzC6I2CBMRjKDg6jGJC%2B600y%2FB3hk5sLdZeLCaUJSmaWEWNLRH%2FVHJqpzMs%2Bnmcv1whjD%2F3ovkUGJswz4CuqgY6iwLIgfjFg4hTn9Q%2FPuk%2FesZpw3U%2Fnq8P%2Bg3CW%2FWW9obuEuUWdTIOvf3B9iaPKTMqPurcnxx7FljRDEMnmq2LeHW3llKrUMR9V2CXftQQnVyZCVbQlOO41eKV1jI1wgDhZFfJ7etjTB1uERWA5shf4Y3IWj1Em4x0uZqzwpYn8ac3e%2FF55QA2cD8tZQ8X9La0OzSTzgy89k1j2%2Fv2Ba0%2FCIA%2FZLznCyaxAHp%2BgUnv9OhXdhxCC2uATfwDdGDbSwHSfqZPqlXJlebZeUkHJmolvCONGFZ9gof%2Fw2hE7t%2Fo1jDQ6ehEhuGRtmn0gWuqaINnXrxwr%2Bq24d06eJAN5NsYohxQdyQ%2FpnzrM2ENRB4%3D&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20231108T123900Z&X-Amz-SignedHeaders=host&X-Amz-Expires=7200&X-Amz-Credential=ASIAQ67U57MGYIKNVAFD%2F20231108%2Fap-south-1%2Fs3%2Faws4_request&X-Amz-Signature=ed141e88f5128f9d7cb50a9d118697c748c08b44fbaa740b2da7e5fac09376e4";
    // char *cis_url = "https://payments-dev-iot.s3.ap-south-1.amazonaws.com/lit/data.czip?response-content-disposition=inline&X-Amz-Security-Token=IQoJb3JpZ2luX2VjEJn%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEaCmFwLXNvdXRoLTEiRzBFAiADBb2UbR99pToX5ypuvfT0dBb8MrLfEnhsKiKczsU2OQIhANH%2BLjJPouKKlPkbAcRh3ZNIWKTeGqQRHn7mYMjZohLDKrQDCNL%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEQAhoMMDY2NTQ4NzkyMDc3Igy92ZmoVZV%2BQi2iWCYqiAN3BYOP0rtvox%2BzcMo7QfFk9BFKc6mudU59gLfrxBNhWW5sICVfabuOUTSWLhXNwisYvQ4DmofNucJHNpAEJ%2BLpiqKqVFRrSFk6VTeftV9oxGlImh%2Bvo7jPwfnjYEGMBH8p7tNob15iKcdIMd874Y%2FbLhmhMyH49mAQ2ONaQC177tAJxevlh7IWviVQWcsrGtPCX4EZ1bHlTurQpKZ%2BuxMCsu8fqF5lCPAzLz8bhaXCl2OBcJFZ5kAZ2jpx3XOV4jiBk9HHaaGVuUtuqQRRqXBAXfsKUwubAcnE6B7mcVh%2FbLDA6uyZx0yGX6uBXMpN9d7P8fyhSPhlWmlmzZjIpE0tz75OasfXEPfP28yK4UwLNaC4furpv3Jz3DDK8qhKtfUJjIUirfzCPlDhoMeG3iPvvjLxXe4DuiHy06fw2%2F8COkCH3puwpDsipJ%2FobLJjNAiILuvez8kikkveh1rwCtajYjiqOq02D3XzQBZiRd%2FCCnBGr2xiNHOdVNtWJW3rjlMoE4GaAr99mTDuv7KqBjqLAqWoiDBzjYdxqP2TyTJ1daSrVI8450tlZiWj3sKeB%2ByXP9CfI6zKi19yYsm8w7pBd2Rn7Rr7cGVYzkTKczYbNak0BD8xQ8GLv2OoEFP%2BzadzwzFFYzqqISK2W2dlVsfA9IWmYEN9Jyv3HT2qPC6rlcVidTqiCIajdkBNDpV%2FC5q9a0%2FSBYidnf4AZUo2%2FMBt1SndlsIHxpF7ieJwp%2BcA2g%2BDybGzNKG0BR4kZLXdXp4ytvKT0%2BKz87ElWHQZ%2BnES9GpwTm%2Fang5Bpn%2FsZe%2BDW4iK4%2FCQuENJFAPDQM0pL0t%2BmX%2F%2FE5I5XD0Cs2rjI1y9d%2Bp26WsvcHqVMGslOBEcoDrcNEHJxw72s0vpvA%3D%3D&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20231109T090225Z&X-Amz-SignedHeaders=host&X-Amz-Expires=43200&X-Amz-Credential=ASIAQ67U57MG3XQC2MNU%2F20231109%2Fap-south-1%2Fs3%2Faws4_request&X-Amz-Signature=0c0a2b29b4e2248b7aa590e8fc5029b9f15d39114671a1d7c080924b11af9f10";
    // char *cis_url = HEAD_URL;
    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);

    http.verify_mode = MBEDTLS_SSL_VERIFY_NONE;
    http.use_ssl = true;
    http.is_chunked = false;

    http.custom_headers = (char*)Paytm_malloc(CUSTOM_HEADER_LEN);
    sprintf(http.custom_headers,
        "Range: bytes=%d-%d\r\n\r\n", 0, 128 - 1);
    rc = Paytm_HTTP_Initialise_GET(LOC_EXTER_MEM, &http, cis_url, 0, NULL);
    if(rc < 0)
    {
        Paytm_TRACE(">>>>Get fail!");
    }

    Paytm_TRACE(">>>>Header: ");
    for(int i = 0; i < strlen(http.content); i++)
    {
        RTI_LOG1("%c", http.content[i]);
    }Paytm_TRACE("\r\n");

    int start, end;
    char *sStr, *pStr, *subStr;

    sStr = strstr(http.content, "Content-Range:");
    if(sStr == NULL)
    {
        Paytm_TRACE("Can not find Content-Range in http.content");
        goto exit;
    }

    sStr = strstr(sStr, "/");
    if(sStr == NULL)
    {
        Paytm_TRACE("Can not find / in http.content");
        goto exit;
    }
    sStr++;

    start = sStr - http.content;
    pStr = strstr(sStr, "\n");
    end = pStr - http.content;

    subStr = (char*)Paytm_malloc(end - start + 1);
    memcpy(subStr, sStr, end - start);
    subStr[end - start] = '\0';

    size_t file_size = 0;

    sscanf(subStr, "%d", &file_size);
    Paytm_TRACE("Prase Content %s, size %d", subStr, file_size);

    //start to recv data
    int get_len_this_time = 0, get_len_sum = 0;
    
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

        memset(http.custom_headers, 0x00, CUSTOM_HEADER_LEN);
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

    Paytm_TRACE("Ext flash size after download: %d", Paytm_GetFreeExtFlashSize());
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

    char *fota_url = "https://cisfs.oss-cn-shenzhen.aliyuncs.com/india_app.bin";

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

    http.custom_headers = (char*)Paytm_malloc(CUSTOM_HEADER_LEN);
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
    int get_len_this_time = 0, get_len_sum = 0;
    
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

void dfota_download(void* p)
{
    int rc = 0;

    char *fota_url_to_dirty = "https://cisfs.oss-cn-shenzhen.aliyuncs.com/DFOTA_3.bin";
    char *fota_url_to_clean = "https://cisfs.oss-cn-shenzhen.aliyuncs.com/DFOTA_3.bin";
    char *fota_url = NULL;

    http_request_t http = {0};
    http.response_buffer = (char*)Paytm_malloc(HTTP_BUF_SIZE);
    http.content = (char*)Paytm_malloc(HTTP_BUF_SIZE);

    http.verify_mode = MBEDTLS_SSL_VERIFY_NONE;
    http.use_ssl = true;
    http.is_chunked = false;

    if(strstr(kernelGitVersion(), ".dirty")){
        // current kernel version is dirty, dfota to clean version 
        fota_url = fota_url_to_clean;
    } else {
        // current kernel version is clean, dfota to dirty version 
        fota_url = fota_url_to_dirty;
    }
    Paytm_TRACE("Git version: %s!", kernelGitVersion());
    Paytm_TRACE("HTTP URL: %s", fota_url);
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

    http.custom_headers = (char*)Paytm_malloc(CUSTOM_HEADER_LEN);
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
    int get_len_this_time = 0, get_len_sum = 0;
    
    start = 0;
    end = 0;
    //create file
    PFILE fd;
    if(Paytm_fexists(LOC_INTER_MEM, PAYTM_DFOTA_FILE) != 0)
    {
        fd = Paytm_fcreate(LOC_INTER_MEM, PAYTM_DFOTA_FILE, "wb+");
        if(fd <= 0)
        {
            Paytm_TRACE("Create file failed");
            goto exit;
        }
    }else{
        fd = Paytm_fopen(LOC_INTER_MEM, PAYTM_DFOTA_FILE, "rb+");
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
    if(!Paytm_SetDFOTAFileReady(PAYTM_DFOTA_FILE))
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