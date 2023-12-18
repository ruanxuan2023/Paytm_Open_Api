#ifndef PAY_TM_LED_API_H
#define PAY_TM_LED_API_H

#include "osi_api.h"
#include "gpio_api.h"
#include "drv_def.h"

#define PAYTM_MAX_LED_NUM       (3)
#define PAYTM_MAX_BATTERY_LED_NUM (3)

#define PAYTM_LED_RED_PIN       (2)
#define PAYTM_LED_GREEN_PIN     (74)
#define PAYTM_LED_BLUE_PIN      (14)
#define PAYTM_CHARGE_LED_PIN_1  (55)
#define PAYTM_CHARGE_LED_PIN_2  (37)
#define PAYTM_CHARGE_LED_PIN_3  (73)
#define PAYTM_BACKLIT_LED_PIN  (3)

#define PAYTM_LED_RED_PINMUX     (GPIO_02)
#define PAYTM_LED_GREEN_PINMUX   (GPIO_74)
#define PAYTM_LED_BLUE_PINMUX    (GPIO_14)
#define PAYTM_CHARGE_LED_PINMUX_1   (GPIO_55)
#define PAYTM_CHARGE_LED_PINMUX_2   (GPIO_37)
#define PAYTM_CHARGE_LED_PINMUX_3   (GPIO_73)
#define PAYTM_BACKLIT_LED_PINMUX   (GPIO_03)

#define PAYTM_LED_RED_FUN       (MFP_AF0)
#define PAYTM_LED_GREEN_FUN     (MFP_AF0)
#define PAYTM_LED_BLUE_FUN      (MFP_AF0)
#define PAYTM_CHARGE_LED_1_FUN  (MFP_AF0)
#define PAYTM_CHARGE_LED_2_FUN  (MFP_AF0)
#define PAYTM_CHARGE_LED_3_FUN  (MFP_AF0)
#define PAYTM_BACKLIT_LED_FUN  (MFP_AF0)

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

/**
 * @description: 
 * @param {uint8_t} id  0/1/2
 * @param {bool} open   open the led or close
 */
void Paytm_Set_Battery_Led(uint8_t id, bool open, bool blink);
/**
 * @description: open or close the background light
 * @param {bool} open
 */
void Paytm_Set_Backlit_Led(bool open);

#endif