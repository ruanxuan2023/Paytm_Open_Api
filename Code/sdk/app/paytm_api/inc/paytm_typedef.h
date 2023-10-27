#ifndef QYW_TYPEDEF_H
#define QYW_TYPEDEF_H

#include "osi_api.h"

typedef unsigned int Paytm_size_t;
typedef int32_t int32;
typedef uint32_t uint32;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint64_t uint64;
typedef int64_t int64;
typedef int PFILE;

//--TODO 此结构体未定义
typedef enum 
{
    LOC_INTER_MEM = 0,
    LOC_EXTER_MEM
}Paytm_location_t;

typedef enum {
    URC_PDP_ACTIVE,
    URC_PDP_INACTIVE,
    URC_NET_ACTIVE,
    URC_NET_DISCONNECTED,
    URC_SIM_INSERTED,
    URC_SIM_REMOVED,
    URC_SIM_READY,
    URC_SIM_EJECTED_FOR_LONG_TIME,
    URC_AUDIO_PLAY_STARTED,
    URC_AUDIO_PLAY_COMPLETED,
    URC_AUDIO_PLAY_ERROR,
    URC_AUDIO_PLAY_SYSTEM_ERR_STR,
    URC_AUDIO_PLAY_STOPPED,
    URC_USB_PLUGGED,	
    URC_USB_REMOVED,
}urcEvent_e;

#endif