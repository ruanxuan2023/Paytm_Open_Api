#ifndef PAYTM_BT_AUDIO_API_H
#define PAYTM_BT_AUDIO_API_H

#include "paytm_sys_api.h"
#include "paytm_typedef.h"

#define BT_SWITCH_IO    (56)
#define BT_RESET_IO     (70)

typedef enum{
    BT_CMD_RESET    = 0x30,   //>
    BT_CMD_REQUIRE_CONNECT,     //>
    BT_CMD_REJECT_CONNECT,      //>
    BT_CMD_CLEAR_PAIRED_MAC,
    BT_CMD_CLEAR_ALL_PAIRED_MAC,
    BT_CMD_IS_PAIRED_MAC,
    BT_CMD_SET_BT_NAME,
    BT_CMD_SET_TIMEOUT,
    BT_CMD_QUERY_STATUS,
    BT_CMD_GET_BT_NAME,
    BT_CMD_GET_ADDRESS,
    BT_CMD_PAUSE_PLAY,
    BT_CMD_DISCONNECT,
    BT_CMD_PREVIOUS_SONG,
    BT_CMD_NEXT_SONG,
    BT_CMD_VOLUME_UP,
    BT_CMD_VOLUME_DOWN,
    BT_CMD_STATUS_REPORT,
    BT_CMD_POWER_DOWN

}Paytm_BT_AUDIO_CMD;

typedef enum{
    BT_EVENT_NONE = 0x00,
    BT_EVENT_START,
    BT_EVENT_INIT_SUCCESS,
    BT_EVENT_INIT_FAILED,
    BT_EVENT_IDLE,                                  /* BT power on, but no connection/disconnection event */
    BT_EVENT_IDLE_TIMEOUT,
    BT_EVENT_PAIRED_REQUEST,
    BT_EVENT_CONNECTION_REJECT,                     /* Event after calling Paytm_BT_Reject_Connection */
    BT_EVENT_CONNECTION_TIMEOUT_NO_ACCEPT,          /* timed out for no call for SB_BT_AcceptConnection (Dose it mean when remote device request to pair, but application not give accept cmd so return this urc?) */
    BT_EVENT_CONNECTION_ERROR,
    BT_EVENT_DISCONNECTED,                          /* connection remotely disconnected */
    BT_EVENT_UNPAIRED,                              /* connection upnaired by calling Paytm_BT_Remove_Paired_device */
    BT_EVENT_PAIRED,                                /* already paired device connected */
    BT_EVENT_PLAY_STOP,                             /* media playing or pause */      
    BT_EVENT_PLAY_RECOVER                       
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
 * @param {uint8_t} accept_connection_timeout_sec: 
 * @param {uint8_t} disconnect_timeout_sec
 * @param {uint8_t} paused_timeout_sec
 * @return {int32_t}
 */
int32_t Paytm_BT_Set_Timeout(uint8_t accept_connection_timeout_sec, uint8_t disconnect_timeout_sec, uint8_t paused_timeout_sec);

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
int32_t Paytm_BT_Remove_Paired_device(const uint8 *mac_addr);

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
bool Paytm_BT_device_Is_paired(const uint8 *mac_addr);

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
int32_t Paytm_BT_Set_Name(const char* name);

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
int32_t Paytm_BT_Get_Mac_Address(uint8 *mac);

int32_t Paytm_BT_Audio_Set_Cmd(uint8_t cmd, uint8_t * data, uint8_t data_len);

void Paytm_BT_Powerup(void);
void Paytm_BT_Powerdown(void);
#endif