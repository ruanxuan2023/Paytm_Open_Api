#include "paytm_typedef.h"
#include "paytm_battery_api.h"

#include <stdio.h>
#include <string.h>
#include "osi_api.h"

message_callback_t b_cb(void* p)
{
    Paytm_TRACE("Cur bat val is %d", *(int*)p);
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
            Paytm_TRACE("The battery is charging! Voltage = %d, Level = %d", voltage, Paytm_GetBatteryLevel());
        }else{
            Paytm_TRACE("The battery is not charging! Voltage = %d, Level = %d", voltage, Paytm_GetBatteryLevel());
        }
    }
}