#ifndef PAYTM_NET_API_H
#define PAYTM_NET_API_H

#include "paytm_typedef.h"
#include "paytm_dev_info_api.h"

int32 Paytm_GetOperator(char* _operator, uint32 operator_len);
int32 Paytm_ReadCellTowerDetails(int32 *mcc, int32 *mnc, int32 *lac, int32 *cellId);
int32 Paytm_GetGPRSState(int32 *stat);
int32 Paytm_GetLocalIPAddress(void);
int32 Paytm_GPRS_Connect(Paytm_ip_version_t ip_version, char *apn);
int32 Paytm_GPRS_Disconnect(void);
int32 Paytm_Reset_GPRS_Service(void);
void network_state_callback_register(message_callback_t cb);
void gprs_state_callback_register(message_callback_t cb);
#endif