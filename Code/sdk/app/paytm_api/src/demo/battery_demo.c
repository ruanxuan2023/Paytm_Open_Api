#include "paytm_typedef.h"
#include "paytm_battery_api.h"

#include <stdio.h>
#include <string.h>
#include "osi_api.h"

void b_cb(void* p)
{
    int status =  *(int*)p;

    Paytm_TRACE("Battery status %d", status );
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
        // if(Paytm_GetChargingStatus()){
        //     Paytm_TRACE("The battery is charging! ");
        // }else{
        //     Paytm_TRACE("The battery is not charging!");
        // }

        // if(Paytm_GetChargingFullStatus()){
        //     Paytm_TRACE("The battery charge full");
        //     Paytm_Set_Battery_Led(0, 1, 0);
        // }else{
        //     Paytm_Set_Battery_Led(0, 1, 1);
        // }
    }
}