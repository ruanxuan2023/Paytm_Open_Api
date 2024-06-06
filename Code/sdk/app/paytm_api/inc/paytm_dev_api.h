#ifndef PAYTM_DEV_API_H
#define PAYTM_DEV_API_H

#include "paytm_typedef.h"
#include "paytm_sys_api.h"
#include "paytm_audio_api.h"

#define PAYTM_TOKEN_32      "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
#define PAYTM_DEV_INFO_FILE "paytm_dev_info.cfg"
#define PAYTM_TOKEN_FILE    "paytm_token.txt"
#define PAYTM_AUTHID_FILE   "paytm_authid.txt"
#define PAYTM_FOTA_FILE     "FOTA_APP.bin"
#define PAYTM_DFOTA_FILE     "FOTA_KERNEL.bin"

int64 Paytm_GetPhysicalROM(void);
int64 Paytm_GetTotalROM(void);

/**
 * @description: Return free rom size(internel flash) of device, uints is byte
 * @return {*}
 */
int32 Paytm_GetFreeROM(void);

/**
 * @description: Return free size of externel flash, uints is byte
 * @return {*}
 */
int32 Paytm_GetFreeExtFlashSize(void);
uint16 Paytm_GetTotalNVRAM(void);
uint16 Paytm_GetFreeNVRAM(void);
int64 Paytm_GetTotalHeapSize(void);

/**
 * @description: Return free ram size of device, uints is byte
 * @return {*}
 */
int32 Paytm_GetFreeHeapSize(void);
int64 Paytm_GetPhysicalRAM(void);
int64 Paytm_GetTotalRAM(void);

/**
 * @description: token data example
 *  {   
 *     "token":"xxxxxxxxxxxxxxx",
 *     "crc": xxx
 *  }
 * @return {*}
 */
int32 Paytm_readToken32Byte(Paytm_location_t sl,char *token, uint32 token_len);
int32 Paytm_readAuthID(char *authID, uint32 authID_len);
int32 Paytm_setAuthID(char *authID, uint32 authID_len);
uint8 Paytm_ValidateHardwareConfig(void);
int32 Paytm_requestFOTA(Paytm_location_t sl, char *binfilename, char *sigfilename, uint8 *verifyOk);
int32 Paytm_resetForFOTA(void);
bool Paytm_SetFOTAFileReady(const char *fw_path);
bool Paytm_CheckFOTAFileValid(const char *fw_path);
bool Paytm_SetDFOTAFileReady(const char *path);

/**
 * @description: Set the device will reboot or not when crash happens
 * @param {bool} enable true: when crash happens device enter dump mode and stub; false: device reboot immediately
 * @return {*}
 */
void Paytm_Exception_Dump_Enable(bool enable);

/**
 * @description: Set the USB function disable, battery charge only
 * @return {*}
 */
void Paytm_Usb_Function_Disable(void);

/// \brief  This function checks if device was turned off due to critical battery level
    /// \param[out] count           count for device turned off due to critical battery level
    /// \return 1       Powered off due to critical battery level
    ///         0       otherwise
uint8 Paytm_WasLastSwitchOffDueToBatteryCheck(uint8 *count);

typedef enum {
    STATE_POWERKEY_PRESSED = 0,
    STATE_POWERKEY_RELEASED,
    STATE_DEVICE_IDLE
}Paytm_powerkey_userstate_t;

typedef enum{
    Paytm_POWERUP_UNKNOWN = 0, 
    Paytm_POWERUP_PWRKEY, 
    Paytm_POWERUP_PIN_RESET, 
    Paytm_POWERUP_OTA_RESET, 
    Paytm_POWERUP_SOFT_RESET ,
    Paytm_POWERUP_ALARM, 
    Paytm_POWERUP_CHARGE, 
    Paytm_POWERUP_WDG, 
    Paytm_POWERUP_PSM_WAKEUP,
    Paytm_POWERUP_PANIC    
}Paytm_powerUp_reason_e;

int32 Paytm_GetPowerOnReason(void);
int32 Paytm_Reset(uint8 normal);
int32 Paytm_PowerDown(uint8 normal);
void Paytm_AutoSleep_Enable(uint8 enable);
uint8 Paytm_AutoSleep_Enabled(void);
uint8 Paytm_GetAutoSleepState(void);
uint8 Paytm_GetSleepState(void);

#endif