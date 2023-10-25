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
    Paytm_Battery_Initialise(20);

    battery_charging_report_callback_register(b_cb);

    Paytm_BatteryLevelMonitoring(true);

    int voltage = 0;

    while (1)
    {
        /* code */
        Paytm_GetBatteryVoltage(&voltage);
        Paytm_delayMilliSeconds(5000);
        Paytm_TRACE("The battery %s charging! Voltage = %d", voltage, Paytm_GetChargingStatus() ? "is" : "is not");
    }
    
}