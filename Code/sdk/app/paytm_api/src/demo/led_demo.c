#include <stdlib.h>
#include <stdio.h>
#include "osi_api.h"

#include "paytm_led_api.h"
#include "paytm_sys_api.h"

void ledRun(void)
{
    Paytm_LED_SetColor(LED_GREEN, 1);Paytm_TRACE(">>>>Green led, twinkle");
    Paytm_delayMilliSeconds(8000);
    Paytm_LED_SetColor(LED_RED, 1); Paytm_TRACE(">>>>Red led, twinkle");
    Paytm_delayMilliSeconds(8000);
    Paytm_LED_SetColor(LED_BLUE, 0);Paytm_TRACE(">>>>Blue led, not twinkle");
    Paytm_delayMilliSeconds(8000);
    Paytm_LED_SetColor(LED_BLUE, 1);Paytm_TRACE(">>>>Blue led, twinkle");
    Paytm_delayMilliSeconds(8000);

    Paytm_LED_SetColor(LED_CYAN, 1);Paytm_TRACE(">>>>Cyan led, twinkle");
    Paytm_delayMilliSeconds(8000);
    Paytm_LED_SetColor(LED_MAGENTA, 0);Paytm_TRACE(">>>>Magenta led, not twinkle");
    Paytm_delayMilliSeconds(8000);
    Paytm_LED_SetColor(LED_YELLOW, 1);Paytm_TRACE(">>>>Yellow led, twinkle");
    Paytm_delayMilliSeconds(8000);

    Paytm_LED_SetColor(LED_WHITE, 0);Paytm_TRACE(">>>>White led, not twinkle");
    Paytm_delayMilliSeconds(8000);
    Paytm_LED_SetColor(LED_WHITE, 1);Paytm_TRACE(">>>>White led, twinkle");
    Paytm_delayMilliSeconds(8000);
    Paytm_LED_SetColor(LED_WHITE, 0);

    Paytm_Set_Battery_Led(0, 1, 1);Paytm_TRACE(">>>>Battery led 1 twinkle");
    Paytm_delayMilliSeconds(8000);
    Paytm_Set_Battery_Led(1, 1, 1);Paytm_TRACE(">>>>Battery led 2 twinkle");
    Paytm_delayMilliSeconds(8000);
    Paytm_Set_Battery_Led(2, 1, 1);Paytm_TRACE(">>>>Battery led 3 twinkle");
    Paytm_delayMilliSeconds(8000);

    Paytm_Set_Battery_Led(0, 0, 1);
    Paytm_Set_Battery_Led(1, 0, 1);
    Paytm_Set_Battery_Led(2, 0, 1);

    Paytm_LED_off();
}