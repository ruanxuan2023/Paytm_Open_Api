#ifndef PAYTM_BUTTON_API_H
#define PAYTM_BUTTON_API_H

#include "paytm_sys_api.h"

#define KEY_BUTTON_PIN      (51)     
#define KEY_BUTTON_MUX      (GPIO_51)     
#define KEY_BUTTON_FUN      (MFP_AF0)

typedef enum {
    STATE_BUTTON_DOUBLE_CLICK = 0,
    STATE_BUTTON_SINGLE_CLICK,
    STATE_BUTTON_LONG_PRESS
}button_subtask_userstate_e;

typedef enum {
    BUTTON_PLUS = 0,
    BUTTON_MINUS,
    BUTTON_FUNCTION,
    BUTTON_COUNT
}buttons_enum_t;

int32 Paytm_Button_events(uint8 enable);
int32_t Paytm_Button_init(void);
void button_action_callback_register(message_callback_t cb);

#endif 