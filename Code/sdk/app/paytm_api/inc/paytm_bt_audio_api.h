#ifndef PAYTM_BT_AUDIO_API_H
#define PAYTM_BT_AUDIO_API_H

#include "paytm_sys_api.h"
#include "paytm_typedef.h"

#define BT_SWITCH_IO    (56)
#define BT_RESET_IO     (70)

typedef enum{
    BT_PREVIOUS_SONG = 0x21,
    BT_NEXT_SONG = 0x22,
    BT_VOLUME_UP = 0x23,
    BT_VOLUME_DOWN = 0x24,
    BT_PAUSE_PLAY = 0x25,
    BT_DISCONNECT = 0x26
}Paytm_BT_AUDIO_CMD;

typedef enum{
    SB_BT_DEVICE_CONNECTION_IDLE = 0x00,            /* BT power on, but no connection/disconnection event */
    SB_BT_DEVICE_CONNECTION_WAITING,                /* BT power on, but no connnect request comes */
    SB_BT_DEVICE_CONNECTION_REJECTED,               /* Event after calling Paytm_BT_Reject_Connection */
    SB_BT_DEVICE_CONNECTION_TIMEOUT_PLAYBACK,       /* timed out for no playback(dont understand this? What playback) */ 
    SB_BT_DEVICE_CONNECTION_TIMEOUT_NO_ACCEPT,      /* timed out for no call for SB_BT_AcceptConnection (Dose it mean when remote device request to pair, but application not give accept cmd so return this urc?) */
    SB_BT_DEVICE_CONNECTION_TIMEOUT_IDLE,           /* BT timeout for no connection(What difference with SB_BT_DEVICE_CONNECTION_WAITING?) */
    SB_BT_DEVICE_CONNECTION_TIMEOUT_NO_BT_STATE,    /* timed out for incomplete connection packet */
    SB_BT_DEVICE_CONNECTION_DISCONNECTED,           /* connection remotely disconnected */
    SB_BT_DEVICE_CONNECTION_UNPAIRED,               /* connection upnaired by calling Paytm_BT_Remove_Paired_device */
    SB_BT_DEVICE_CONNECTION_INITIATING = 0x80,      /* connect urc received for power on(when init BT module, give this event) */
    SB_BT_DEVICE_CONNECTION_CONNECTED,              /* new unpaired device connected */
    SB_BT_DEVICE_CONNECTION_PAIRED,                 /* already paired device connected */
    SB_BT_PLAY_STOP                                 /* media playing or pause */
}Paytm_BT_Device_State;

/**
 * @description: Init BT Audio module
 * @return {*}  0: init success, -1: init fail
 */
int32_t Paytm_BT_Audio_Init(void);

/**
 * @description: Enable BT audio or disable, default is close
 * @param {bool} enable
 * @return {*} ture: switch success; false: switch false
 */
int32_t Paytm_BT_Audio_Enable(bool enable);

/**
 * @description: 
 * @param {message_callback_t} cb
 * @return {*}
 */
int32_t Paytm_BT_Callback_Register(message_callback_t cb);

/**
 * @description: Send BT cmd to control bt audio
 * @param {uint8_t} cmd
 * @return {*}  0: Send success and rsp ok, -1: send fail or bt_uart init fail
 */
int32_t Paytm_BT_Audio_Cmd_Set(uint8_t cmd);

/**
 * @description: 
 * @param {int32_t} accept_connection_timeout_sec: 
 * @param {int32_t} disconnect_timeout_sec
 * @param {int32_t} paused_timeout_sec
 * @return {*}
 */
void Paytm_BT_Set_Timeout(int32_t accept_connection_timeout_sec, int32_t disconnect_timeout_sec, int32_t paused_timeout_sec);

/**
 * @description: 
 * @return {*} 0:BT power off, 1:BT power on, <0: get state fail
 */
int32_t Paytm_BT_Get_State(void);

/**
 * @description: When remote device requests to pair with BT, BT gives a event to application, and call this fuction to accept this pair request
 * @param {uint8} *mac_addr: mac address of remote device
 * @return {*} 0: success, <0: fail
 */
int32_t Paytm_BT_Accept_Connection(uint8 *mac_addr);

/**
 * @description: When remote device requests to pair with BT, BT gives a event to application, and call this fuction to reject this pair request
 * @param {uint8} *mac_addr: mac address of remote device
 * @return {*} 0: success, <0: fail
 */
int32_t Paytm_BT_Reject_Connection(uint8 *mac_addr);

/**
 * @description: Remove one device from the paired device list
 * @param {uint8} *mac_addr: mac address of remote device
 * @return {*} 0: success, <0: fail
 */
int32_t Paytm_BT_Remove_Paired_device(uint8 *mac_addr);

/**
 * @description:  Clears paired remote device list from bluetooth module
 * @return {*} 0: success, <0: fail
 */
int32_t Paytm_BT_Clear_Paired_Device_List(void);

/**
 * @description:  Checks if remote device mac address is already paired
 * @param {uint8} *mac_addr: mac address of remote device
 * @return {*} 0: success, <0: fail
 */
bool Paytm_BT_device_Is_paired(uint8 *mac_addr);

/* It seems not support to set&get specific volume, because different phone may have different volume level. 
    We can use function Paytm_BT_Audio_Cmd_Set to control volume up&down
*/
int32_t Paytm_BT_Set_volume(uint8_t volume);
int32_t Paytm_BT_Get_Volume(void);

/**
 * @description: Set broadcast name for bluetooth connection
 * @param {char*} name
 * @return {*}
 */
int32_t Paytm_BT_Set_Name(char* name);

/**
 * @description: Get broadcast name for bluetooth connection
 * @param {char*} name: Input pointer to save BT moule name
 * @param {uint32*} name_len: Input value to save BT module name length
 * @return {*}
 */
int32_t Paytm_BT_Get_Name(char* name, uint32 *name_len);

/**
 * @description: Get BT Module mac address 
 * @param {uint8} *mac: Input pointer to save mac address
 * @param {uint32} mac_len: Input value to save mac address len
 * @return {*}
 */
int32_t Paytm_BT_Get_Mac_Address(uint8 *mac, uint32 mac_len);

#endif