#ifndef PAYTM_NTP_API_H
#define PAYTM_NTP_API_H

#include "paytm_typedef.h"

int Paytm_ntp_sync(const char *NTPServer, uint8 time_interval);
void Paytm_NTP_InternalSyncCb_Regisiter(void (*cb)(void));

#endif