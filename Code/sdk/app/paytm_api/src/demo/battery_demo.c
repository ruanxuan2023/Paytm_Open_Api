#include "paytm_typedef.h"
#include "paytm_battery_api.h"

#include <stdio.h>
#include <string.h>
#include "osi_api.h"

void b_cb(void* p)
{
    int status =  *(int*)p;

    Paytm_TRACE("Battery %s", status ? "not charging" : "charging");
}

void batteryDemo(void)
{
    battery_charging_report_callback_register(b_cb);

    Paytm_BatteryLevelMonitoring(true);

    uint16 voltage = 0;

    while (1)
    {
        /* code */
        Paytm_GetBatteryVoltage(&voltage);
        Paytm_delayMilliSeconds(5000);
        if(Paytm_GetChargingStatus()){
            Paytm_TRACE("The battery is charging! ");
        }else{
            Paytm_TRACE("The battery is not charging!");
        }
    }
}