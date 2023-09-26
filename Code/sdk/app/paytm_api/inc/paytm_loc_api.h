#ifndef PAYTM_LOC_API_H
#define PAYTM_LOC_API_H

#include "paytm_typedef.h"

typedef struct 
{
    float longitude;
    float latitude;
}CellLocationInfo_t;

int32 Paytm_GetCellLocationEx(CellLocationInfo_t *location);
#endif