#include "paytm_dev_api.h"
#include "paytm_dev_info_api.h"

#include <stdlib.h>
#include <stdio.h>
#include "osi_api.h"

void devInfoDemo(void)
{
    Paytm_GPRS_Connect(Paytm__IPVERSION_IPV4, NULL);
    osiThreadSleepMs(6000);

    char res[128] = {0};
    Paytm_Time dt = {0};

    Paytm_GetCoreVersion((char*)res, 128);
    RTI_LOG("Core ver = %s", res);
    memset(res, 0, 128);

    Paytm_GetOS((char*)res, 128);
    RTI_LOG("OS = %s", res);
    memset(res, 0, 128);

    Paytm_GetBaseFWReleaseDate(&dt);
    RTI_LOG("%d-%d-%d %d:%d:%d", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second );

    Paytm_GetModelVersion(res, 128);
    RTI_LOG("Model ver = %s", res);
    memset(res, 0, 128);

    Paytm_GetHardwareVersion(res, 128);
    RTI_LOG("Hardware ver = %s", res);
    memset(res, 0, 128);

    Paytm_GetLibraryVersion(res, 128);
    RTI_LOG("Lib ver = %s", res);
    memset(res, 0, 128);

    Paytm_GetLibraryReleaseDate(&dt);
    RTI_LOG("%d-%d-%d %d:%d:%d", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second );

    Paytm_ReadIMEI(res);
    RTI_LOG("IMEI = %s", res);
    memset(res, 0, 128);

    Paytm_GetIMEI(res);
    RTI_LOG("IMEI = %s", res);
    memset(res, 0, 128);
}