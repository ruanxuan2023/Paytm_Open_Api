#include "paytm_dev_api.h"
#include "paytm_dev_info_api.h"
#include "paytm_net_api.h"

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
    Paytm_TRACE("Core ver = %s", res);
    Paytm_memset(res, 0, 128);

    Paytm_GetOS((char*)res, 128);
    Paytm_TRACE("OS = %s", res);
    Paytm_memset(res, 0, 128);

    Paytm_GetBaseFWReleaseDate(&dt);
    Paytm_TRACE("%d-%d-%d %d:%d:%d", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second );

    Paytm_GetModelVersion(res, 128);
    Paytm_TRACE("Model ver = %s", res);
    Paytm_memset(res, 0, 128);

    Paytm_GetHardwareVersion(res, 128);
    Paytm_TRACE("Hardware ver = %s", res);
    Paytm_memset(res, 0, 128);

    Paytm_GetLibraryVersion(res, 128);
    Paytm_TRACE("Lib ver = %s", res);
    Paytm_memset(res, 0, 128);

    Paytm_GetLibraryReleaseDate(&dt);
    Paytm_TRACE("%d-%d-%d %d:%d:%d", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second );

    Paytm_ReadIMEI(res);
    Paytm_TRACE("IMEI = %s", res);
    Paytm_memset(res, 0, 128);

    Paytm_GetIMEI(res);
    Paytm_TRACE("IMEI = %s", res);
    Paytm_memset(res, 0, 128);
}