#ifndef PAYTM_AUDIO_API_H
#define PAYTM_AUDIO_API_H

#include "paytm_sys_api.h"

#define PAYTM_VOLUME_MIN    (1)
#define PAYTM_VOLUME_MAX    (11)

int32 Paytm_SetAudioVolume(uint8 volume);
int32 Paytm_GetVolume(uint8 *volume);
int32 Paytm_PlayFile(Paytm_location_t lc, const char *filePath, uint8 volume);
int32 Paytm_PlayFileFromDir(Paytm_location_t sl, const char *path, const char *filename, uint8 volume);
int32 Paytm_StopPlay(void);
void playback_state_callback_register(message_callback_t cb);
int32 paytm_AudioInit(void);
#endif