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

#endif