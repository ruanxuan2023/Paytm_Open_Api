#ifndef PAYTM_BT_AUDIO_API_H
#define PAYTM_BT_AUDIO_API_H

#include "paytm_typedef.h"

#define BT_SWITCH_IO    (56)
#define BT_RESET_IO     (70)

/*CMD
上一首      0x21
下一首      0x22
加音量      0x23
减音量      0x24
暂停/播放   0x25
断开        0x26
*/
#define BT_PREVIOUS_SONG 0x21
#define BT_NEXT_SONG 0x22
#define BT_VOLUME_UP 0x23
#define BT_VOLUME_DOWN 0x24
#define BT_PAUSE_PLAY 0x25
#define BT_DISCONNECT 0x26

/**
 * @description: Enable BT audio or not, default is close(when enable BT audio, the 4G board will not play)
 * @param {bool} enable
 * @return {*} ture: switch success; false: switch false
 */
int32_t Paytm_BT_Audio_Enable(bool enable);

/**
 * @description: 
 * @return {*}  0: init success, -1: init fail
 */
int32_t Paytm_BT_Audio_Init(void);

/**
 * @description: Send BT cmd to control bt audio
 * @param {uint8_t} cmd
 * @return {*}  0: Send success and rsp ok, -1: send fail or bt_uart init fail
 */
int32_t Paytm_BT_Audio_Cmd_Set(uint8_t cmd);


void Paytm_BT_Powerup(void);
void Paytm_BT_Reset(void);

#endif