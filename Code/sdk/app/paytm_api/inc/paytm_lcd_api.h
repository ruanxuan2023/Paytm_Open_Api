#ifndef PAY_TM_LCD_API_H
#define PAY_TM_LCD_API_H

#include "osi_api.h"
#include "gpio_api.h"
#include "drv_def.h"

typedef struct 
{
    bool     lcd_bt_cfg;
    bool     lcd_fm_cfg;
    bool     lcd_rs_cfg;
    bool     lcd_count_cfg;
    bool     lcd_paytm_cfg;
    uint32_t lcd_power_cfg;
    uint32_t lcd_signal_cfg;
    uint32_t lcd_wifi_cfg;
    uint32_t lcd_vol_cfg;
    uint32_t lcd_amount_cfg;
    uint64_t lcd_num_cfg;
}Paytm_lcd_config_t;

/**
 * @description: Enable lcd to display 0
 * @return {*}
 */
int32_t Paytm_Lcd_Enable(void);
/**
 * @description: Clear the screen
 * @return {*}
 */
int32_t Paytm_Lcd_Clear(void);
/**
 * @description: Enable or de-enable selection icons and level settings.
 * @param {Paytm_lcd_config_t} Select the settings for each icon. Bool type: 0 means no icon is displayed,
 *                              means no icon is displayed, and the integer value is the level displayed.
 * @return {*}  0-success, 1-already init
 */
int32_t Paytm_Lcd_Config(Paytm_lcd_config_t config);
/**
 * @description: Used to remove the icons not to display amount and num.
 * @param {bool amount} When the amount parameter is true, the lcd screen does not display the amount icon.
 *        {bool num}    When the num parameter is true, the lcd screen does not display the num icon.
 * @return {*}  0-success, 1-err
 */
int32_t Paytm_Lcd_Num_Clear(bool amount, bool num);
#endif