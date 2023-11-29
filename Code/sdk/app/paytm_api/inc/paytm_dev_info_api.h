#ifndef PAYTM_DEV_INFO_API_H
#define PAYTM_DEV_INFO_API_H

#include "paytm_sys_api.h"
#include "paytm_typedef.h"


//TODO 这几个长度具体为多少
#define IMEI_BUFFER_LEN (18)
#define IMSI_BUFFER_LEN (18)
#define SIM_NUMBER_BUFFER_LEN (32)

typedef enum {
    Paytm__IPVERSION_IPV4 = 0,
    Paytm__IPVERSION_IPV6,
    Paytm__IPVERSION_IPV4_IPV6
}Paytm_ip_version_t;

typedef enum {
    Paytm_NW_PROVIDER_NONE = 0,
    Paytm_NW_PROVIDER_IN_AIRTEL,
    Paytm_NW_PROVIDER_IN_VI,
    Paytm_NW_PROVIDER_IN_BSNL,
    Paytm_NW_PROVIDER_IN_JIO,
    Paytm_NW_PROVIDER_UNKNOWN    
}Paytm_network_provider_t;

typedef struct 
{
    char imei[IMEI_BUFFER_LEN + 1];
    char imsi[IMSI_BUFFER_LEN + 1];
    char sim_no[SIM_NUMBER_BUFFER_LEN + 1];
    Paytm_ip_version_t ip_version;
    Paytm_network_provider_t nw_provider;
    uint32 local_ip;
    uint32 local_ipV6[4];
    int32 sim_state;
    int32 signal_strength;
    uint8 charging_status;
    uint8 battery_level;
    uint8 network_type;
    uint8 gprs_register_state;
}deviceinfo_t;

int32 Paytm_GetCoreVersion(char *version, Paytm_size_t version_len);
int32 Paytm_GetOS(char *version, uint32 version_len);
int32 Paytm_GetBaseFWReleaseDate(Paytm_Time *release);
int32 Paytm_GetModelVersion(char *version, uint32 version_len);
int32 Paytm_GetHardwareVersion(char *version, uint32 version_len);

/**
 * @description:                Return the paytm library version
 * @param {char} *version       Input version data array
 * @param {uint32} version_len  Input version len
 * @return {*}
 */
int32 Paytm_GetLibraryVersion(char *version, uint32 version_len);
int32 Paytm_GetLibraryReleaseDate(Paytm_Time *release);
int32 Paytm_GetCpuUsage(void);
int32 Paytm_ReadIMEI(char imei[]);
int32 Paytm_GetIMEI(char imei[]);

#endif