#ifndef PAY_TM_LED_API_H
#define PAY_TM_LED_API_H

#include "osi_api.h"
#include "gpio_api.h"
#include "drv_def.h"

#define PAYTM_MAX_LED_NUM       (3)

#define PAYTM_LED_RED_PIN       (3)
#define PAYTM_LED_GREEN_PIN     (54)
#define PAYTM_LED_BLUE_PIN      (37)

#define PAYTM_LED_RED_PINMUX     (GPIO_03)
#define PAYTM_LED_GREEN_PINMUX   (GPIO_54)
#define PAYTM_LED_BLUE_PINMUX    (GPIO_37)

#define PAYTM_LED_RED_FUN       (MFP_AF0)
#define PAYTM_LED_GREEN_FUN     (MFP_AF0)
#define PAYTM_LED_BLUE_FUN      (MFP_AF0)

#define PAYTM_LED_BLINK_MS      (250)

typedef enum {
    LED_OFF = 0x00,
    LED_RED = 0x01,
    LED_GREEN = 0x02,
    LED_BLUE = 0x04,
    LED_CYAN = 0x06,
    LED_MAGENTA = 0x05,
    LED_YELLOW = 0x03,
    LED_WHITE = 0x07
}led_color_type_t;

typedef struct{
    uint32_t id;
    bool curLvl;
    uint32_t interval;
    osiTimer_t *ledTimer;
}ledPin_t;

led_color_type_t Paytm_LED_GetColor(void);
void Paytm_LED_SetColor(led_color_type_t type, int STABLE_BLINK);
void Paytm_LED_off(void);
void Paytm_LED_Init(void);

#endif