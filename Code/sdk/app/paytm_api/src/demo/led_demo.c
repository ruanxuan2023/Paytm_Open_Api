#include <stdlib.h>
#include <stdio.h>
#include "osi_api.h"

#include "paytm_led_api.h"
#include "paytm_sys_api.h"

void ledRun(void)
{
    Paytm_LED_SetColor(LED_WHITE, 0);
    Paytm_delayMilliSeconds(2000);
    Paytm_LED_SetColor(LED_GREEN, 1);
    Paytm_delayMilliSeconds(2000);
    Paytm_LED_SetColor(LED_RED, 1);
    Paytm_delayMilliSeconds(2000);
    Paytm_LED_SetColor(LED_BLUE, 0);
    Paytm_delayMilliSeconds(2000);
    Paytm_LED_SetColor(LED_RED, 1);
    Paytm_delayMilliSeconds(2000);
    Paytm_LED_off();
}