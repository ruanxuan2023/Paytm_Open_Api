#ifndef PAYTM_NET_API_H
#define PAYTM_NET_API_H

#include "paytm_typedef.h"
#include "paytm_dev_info_api.h"

typedef struct {
    char *mcc;
    char *mnc;
    char *operator;
    char *apn;
    int mtu;
    int auth_mode;
    int ip_type;
    char *user_name;
    char *passworld;
}apnCfgParam_t;

typedef enum {
    PAYTM_APN_IPV4V6 = 0,
    PAYTM_APN_IPV4 = 1,
    PAYTM_APN_IPV6 = 2,
    PAYTM_APN_IP_NONE,
}paytmApnIpType_e;

typedef enum {
    PAYTM_APN_AUTH_NONE = 0,
    PAYTM_APN_AUTH_PAP = 1,
    PAYTM_APN_AUTH_CHAP = 2,
}paytmApnAuthType_e;

int32 Paytm_GetOperator(char* _operator, uint32 operator_len);
int32 Paytm_ReadCellTowerDetails(int32 *mcc, int32 *mnc, int32 *lac, int32 *cellId);
int32 Paytm_GetGPRSState(int32 *stat);
int32 Paytm_GetLocalIPAddress(void);
int32 Paytm_GPRS_Connect(Paytm_ip_version_t ip_version, char *apn);
int32 Paytm_GPRS_Disconnect(void);
int32 Paytm_Reset_GPRS_Service(void);
bool Paytm_Net_IsConnected(void);
/**
 * append = true, add this apn cfg to table
 * append = false, clear previous apn settings and add this cfg
*/
bool Paytm_SetApn(apnCfgParam_t *cfg, bool append);
void network_state_callback_register(message_callback_t cb);
void gprs_state_callback_register(message_callback_t cb);
#endif