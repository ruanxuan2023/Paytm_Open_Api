#ifndef PAYTM_DEV_API_H
#define PAYTM_DEV_API_H

#include "paytm_typedef.h"
#include "paytm_sys_api.h"
#include "paytm_audio_api.h"

#define PAYTM_TOKEN_32 "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"

int64 Paytm_GetPhysicalROM(void);
int64 Paytm_GetTotalROM(void);
int64 Paytm_GetFreeROM(void);
uint16 Paytm_GetTotalNVRAM(void);
uint16 Paytm_GetFreeNVRAM(void);
int64 Paytm_GetTotalHeapSize(void);
int64 Paytm_GetFreeHeapSize(void);
int64 Paytm_GetPhysicalRAM(void);
int64 Paytm_GetTotalRAM(void);

int32 Paytm_readToken32Byte(char *token, uint32 token_len);
int32 Paytm_readAuthID(char *authID, uint32 authID_len);
int32 Paytm_setAuthID(char *authID, uint32 authID_len);
uint8 Paytm_ValidateHardwareConfig(void);
int32 Paytm_requestFOTA(Paytm_location_t sl, char *binfilename, char *sigfilename, uint8 *verifyOk);
int32 Paytm_resetForFOTA(void);

typedef enum {
    STATE_POWERKEY_PRESSED = 0,
    STATE_POWERKEY_RELEASED,
    STATE_DEVICE_IDLE
}Paytm_powerkey_userstate_t;

typedef enum{
    Paytm_POWERUP_UNKNOWN = 0, 
    Paytm_POWERUP_PWRKEY, 
    Paytm_POWERUP_PIN_RESET, 
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