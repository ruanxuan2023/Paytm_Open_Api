#include <stdlib.h>
#include <stdio.h>
#include "osi_api.h"

#include "paytm_sys_api.h"
#include "paytm_typedef.h"

int g_val = 89;

void* timerCb(void * p)
{
    RTI_LOG("This is usercb, val = %d", *(int*)p);
}

void runTimer(void)
{
    int t_id = 0;
    int rc = 0;

    t_id = Paytm_Timer_Register(0, NULL, NULL);

    RTI_LOG("Timer reg res = %d", t_id);

    rc = Paytm_Timer_Start(t_id, 1000, false);
    RTI_LOG("Timer start rc = %d", rc);

    Paytm_delayMilliSeconds(5000);

    Paytm_Timer_Stop(t_id);

    t_id = Paytm_Timer_Register(1, timerCb, &g_val);
    Paytm_Timer_Start(t_id, 1000, true);

    Paytm_delayMilliSeconds(5000);
    Paytm_Timer_Stop(t_id);
}
