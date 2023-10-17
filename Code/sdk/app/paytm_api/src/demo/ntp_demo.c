#include "paytm_ntp_api.h"
#include "paytm_sys_api.h"
#include "paytm_typedef.h"

#include <stdio.h>
#include <string.h>
#include "osi_api.h"

#define NTP_TEST_HOST   "ntp.aliyun.com"
void ntpDemo(void *p)
{
    Paytm_ntp_sync(NTP_TEST_HOST, 60 * 60);

    Paytm_delayMilliSeconds(3 * 1000);

    Paytm_Time gi = {0};

    Paytm_GetTime(&gi);
    RTI_LOG("%d-%d-%d, %d:%d:%d", gi.year, gi.month, gi.day, gi.hour, gi.minute, gi.second);
}