#ifndef PAYTM_NTP_API_H
#define PAYTM_NTP_API_H

#include "paytm_typedef.h"
#include "paytm_sys_api.h"

typedef enum {
    PAYTM_TIME_SYNC_NITZ = 0,
    PAYTM_TIME_SYNC_NTP
}paytmTimeSyncType_e;

typedef void (*Paytm_NTP_TimerSync_f)(paytmTimeSyncType_e type, Paytm_Time *ptime);

int Paytm_ntp_sync(const char *NTPServer, uint8 time_interval);
void Paytm_NTP_InternalSyncCb_Regisiter(Paytm_NTP_TimerSync_f cb);

#endif