#ifndef PAYTM_BT_AUDIO_API_H
#define PAYTM_BT_AUDIO_API_H

#include "paytm_sys_api.h"
#include "paytm_typedef.h"

#define MAC_ADDR_LEN   (6)
#define DEV_NAME_MAX_LEN   (64)

typedef enum {
    SB_BT_DEVICE_CONNECTION_IDLE = 0x00,         // no connect/disconnect urc received for power on
    SB_BT_DEVICE_CONNECTION_WAITING,             // no connect urc received for power on
    SB_BT_DEVICE_CONNECTION_REJECTED,            // connection rejected by calling SB_BT_RejectConnection()
    SB_BT_DEVICE_CONNECTION_TIMEOUT_PLAYBACK,    // timed out for no playback
    SB_BT_DEVICE_CONNECTION_TIMEOUT_NO_ACCEPT,   // timed out for no call for SB_BT_AcceptConnection()
    SB_BT_DEVICE_CONNECTION_TIMEOUT_IDLE,        // timed out for no connection
    SB_BT_DEVICE_CONNECTION_TIMEOUT_NO_BT_STATE, // timed out for incomplete connection packet
    SB_BT_DEVICE_CONNECTION_DISCONNECTED,        // connection remotely disconnected
    SB_BT_DEVICE_CONNECTION_UNPAIRED,            // connection upnaired by calling SB_BT_RemovePairedDevice()
    SB_BT_DEVICE_CONNECTION_INITIATING = 0x80,   // connect urc received for power on
    SB_BT_DEVICE_CONNECTION_CONNECTED,           // new unpaired device connected
    SB_BT_DEVICE_CONNECTION_PAIRED,              // already paired device connected
    SB_BT_PLAY_STOP,                            // media playing stop or pause 
    SB_BT_PLAY_START,                            // media playing start
} bluetooth_device_state_t;

typedef struct {
    bluetooth_device_state_t state;
    uint8 mac[MAC_ADDR_LEN];
    uint8 name[DEV_NAME_MAX_LEN];
    void *param;
} bluetooth_device_msg_t;

typedef enum {
    OAK_BT_SUCCESS,
    OAK_BT_TURNED_OFF_ERR,    // BT is turned off
    OAK_BT_NO_REQ_PNDING_ERR,  // no request pending
    OAK_BT_MAC_MISMTCH_ERR,  // mac address mismatch
    OAK_BT_LMT_RCHED_ERR,   // 25 Devices already paired
    OAK_BT_AL_REJECTED_ERR,   //Connection was already REJECTED / UNPAIRED
    OAK_BT_AL_PAIRED_ERR, // Connection was already PAIRED
    OAK_BT_TIMEOUT_ERR, // Connection was already PAIRED
}BtErrCode_e;

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
int32_t Paytm_BT_Set_Timeout(uint32_t accept_connection_timeout_sec, uint32_t disconnect_timeout_sec, uint32_t paused_timeout_sec);

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
 * @return {*} true: success, <false: fail
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

typedef struct _bt_paired_device_t {
    uint64 last_connected;
    uint8 mac[MAC_ADDR_LEN];
    char *name;
    struct _bt_paired_device_t *next;
} bt_paired_device_t;
	
	
/// \brief This function return linked list of paired BT devices with bluetooth module
bt_paired_device_t *Paytm_BT_Get_PairedBT_DeviceList(void);

/// \brief This function frees the allocated memory space to SB_Get_PairedBT_DeviceList
void Paytm_Free_DeviceList(bt_paired_device_t *deviceList);

/// \brief This function retrieves the firmware version of the bluetooth module.
/// \param[out] version         buffer for firmware version of the bluetooth module
/// \param[in] version_len      length of the buffer version
/// \return QL_RET_OK           success
///         < 0                 failed
int32 Paytm_BT_GetSDKVersion(uint8 *version, uint32 version_len);



/// \brief This function set BLE enable/disable.
/// \param[in] en      true: BLE ENABLE false: BLE DISABLE
/// \return QL_RET_OK           success
///         other                 failed
int32_t Paytm_BT_BLE_Enable(bool en);


int32_t Paytm_BT_Powerup(void);
int32_t Paytm_BT_Powerdown(void);
#endif