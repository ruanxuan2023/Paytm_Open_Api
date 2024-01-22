
#include "paytm_net_api.h"
#include "paytm_sys_api.h"
#include "paytm_typedef.h"
#include "paytm_dev_api.h"

#include <stdlib.h>
#include <stdio.h>
#include "osi_api.h"

void setApn(void)
{
    apnCfgParam_t apn_cfg = {
        .mcc = "460",
        .mnc = "0",
        .operator = "China Mobile",
        .apn = "cmnet",
        .mtu = 0,
        .auth_mode = PAYTM_APN_AUTH_NONE,
        .ip_type = PAYTM_APN_IPV4V6,
        .user_name = NULL,
        .passworld = NULL
    };

    Paytm_SetApn(&apn_cfg, true);

    Paytm_TRACE("After setting apn, we need reboot the device then it will work");

    Paytm_Reset(0);
}