/* All rights reserved.
 *
 */

#include <stdio.h>
#include <string.h>
#include "osi_api.h"
#include "drv_keypad.h"
#include "kernel_version.h"

typedef enum {
	PMIC_WDT_1s,
	PMIC_WDT_2s,
	PMIC_WDT_4s,
	PMIC_WDT_8s,
	PMIC_WDT_16s,
	PMIC_WDT_32s,
	PMIC_WDT_48s,
	PMIC_WDT_64s
}PMIC_WD_TIMEOUT;

static void prvPowerkeyCb(void){
	if(drvPwrKeyGetStatus(PWRKEY_EXTON1) == PWRKEY_STATE_PRESS)
	{
        RTI_LOG("Power Key press");
        osiWdtEnable(0);
    }
	else
	{
		RTI_LOG("Power Key release");
        osiWdtEnable(1);
    }
}

int appimg_enter(void *param)
{
    RTI_LOG("application image enter");
    RTI_LOG("kernelVersion: %s", kernelVersion());
    RTI_LOG("welcome, %s,%s", __DATE__, __TIME__);
	drvPwrKeySetCB(PWRKEY_EXTON1, prvPowerkeyCb);

    osiThreadSleep(400);
    osiWdtEnable(1);
    osiWdtSetTimeout(PMIC_WDT_8s);
    RTI_LOG("osiWdtEnable enable");
    while (1)
    {
    }
    return 0;
}

void appimg_exit(void)
{
    RTI_LOG("application image exit");
}