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
    Paytm_TRACE("%d-%d-%d, %d:%d:%d", gi.year, gi.month, gi.day, gi.hour, gi.minute, gi.second);
}

void timeZoneSet(void)
{
    Paytm_ntp_sync(NTP_TEST_HOST, 60 * 60);

    Paytm_delayMilliSeconds(3 * 1000);

    Paytm_Time gi = {0};

    Paytm_SetTimezone(8 * 60);
    Paytm_GetTime(&gi);
    Paytm_TRACE("%d-%d-%d, %d:%d:%d", gi.year, gi.month, gi.day, gi.hour, gi.minute, gi.second);
    Paytm_TRACE("Now timezone = %d", Paytm_GetTimezone());

    Paytm_SetTimezone(-2 * 60);
    Paytm_GetTime(&gi);
    Paytm_TRACE("%d-%d-%d, %d:%d:%d", gi.year, gi.month, gi.day, gi.hour, gi.minute, gi.second);
    Paytm_TRACE("Now timezone = %d", Paytm_GetTimezone());

    Paytm_SetTimezone(330);
    Paytm_GetTime(&gi);
    Paytm_TRACE("%d-%d-%d, %d:%d:%d", gi.year, gi.month, gi.day, gi.hour, gi.minute, gi.second);
    Paytm_TRACE("Now timezone = %d", Paytm_GetTimezone());

}

void rtcTimeSet(void)
{
    Paytm_Time gi = {
        .year = 1998,
        .month = 7,
        .day = 7,
        .hour = 19,
        .minute = 11,
        .second = 39
    };

    Paytm_SetTime(&gi);

    Paytm_delayMilliSeconds(5000);

    memset(&gi, 0x00, sizeof(gi));
    Paytm_GetTime(&gi);
    Paytm_TRACE("%d-%d-%d, %d:%d:%d", gi.year, gi.month, gi.day, gi.hour, gi.minute, gi.second);
}