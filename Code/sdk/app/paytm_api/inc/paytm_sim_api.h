#ifndef PAYTM_SIM_API_H
#define PAYTM_SIM_API_H

#include "paytm_sys_api.h"

typedef enum{
    NW_SIM_EJECTED = 0,
    NW_SIM_INSERTED,
    NW_SIM_EJECTED_FOR_LONG_TIME
}Paytm_sim_state_e;

int32 Paytm_ReadIMSI(char imsi[]);
int32 Paytm_GetIMSI(char imsi[]);
int32 Paytm_ReadSimNumber(char sim_no[]);
int32 Paytm_GetSimNumber(char sim_no[]);
bool Paytm_GetCGPADDR(char addr[16]);
bool PaytmGetAPN(char apn[64]);
int32 Paytm_ReadSubscriberNumber(char subscriber_number[]);
//TODO--这个类型无定义
Paytm_network_provider_t Paytm_GetNetworkProvider(uint16 mcc, uint16 mnc);
int32 Paytm_ReadMSISDN(const char *ussd, char msisdn[]);
int32 Paytm_GetSimState(void);
int32 Paytm_GetModemFunction(uint8 *func);
int32 Paytm_ResetSimModule(void);
int32 Paytm_GetSignalStrength(void);
int32 Paytm_GetGSMState(int32 *stat);
int32 Paytm_SimMonitor(uint8 *func);
void sim_state_callback_register(message_callback_t cb);
#endif