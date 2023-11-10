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

    http.custom_headers = (char*)Paytm_malloc(128);
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

        memset(http.custom_headers, 0x00, sizeof(http.custom_headers));
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


void dfota_download(void* p)
{
    int rc = 0;

    char *fota_url_to_dirty = "https://cisfs.oss-cn-shenzhen.aliyuncs.com/dfota_test_to_dirty.bin";
    char *fota_url_to_clean = "https://cisfs.oss-cn-shenzhen.aliyuncs.com/dfota_test_to_clean.bin";
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