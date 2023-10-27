#ifndef PAYTM_SYS_API_H
#define PAYTM_SYS_API_H

#include <stdarg.h>
#include "osi_api.h"
#include "paytm_typedef.h"

typedef void (*message_callback_t)(void* param);

/*********************************    MEMORY API    **************************************/

void *Paytm_malloc(Paytm_size_t size);
void *Paytm_realloc(void *old_ptr, Paytm_size_t old_size, Paytm_size_t new_size);
void Paytm_free(void *buff);
void* Paytm_memset(void* dest, uint8_t value, Paytm_size_t size);
void* Paytm_memcpy(void* dest, const void* src, Paytm_size_t size);
int32_t Paytm_memcmp(const void* dest, const void*src, Paytm_size_t size);
void* Paytm_memmove(void* dest, const void* src, Paytm_size_t size);

/*********************************    LIBRARY FUNCTIONS    ***********************************/

int32 Paytm_atoi(const char* s);
double Paytm_atof(const char* s);
char* Paytm_strncpy(char* dest, const char* src, Paytm_size_t size);
char* Paytm_strncat(char* s1, const char* s2, Paytm_size_t size);
int32 Paytm_strncmp(const char* s1, const char* s2, Paytm_size_t size);
char* Paytm_strchr(const char* s1, int32 ch);
Paytm_size_t Paytm_strlen(const char* str);
char* Paytm_strstr(const char* s1, const char* s2);
uint32 Paytm_strcspn(const char *s1, const char *s2);
uint32 Paytm_strspn(const char *s1, const char *s2);
int32 Paytm_strtol(const char *nptr, char **endptr, int base);
uint32 Paytm_strtoul(const char *nptr, char **endptr, int base);
char *Paytm_strrchr(const char *s, char c);
int32 Paytm_strrcasecmp(const char *s1, const char *s2);
int32 Paytm_strncasecmp(const char *s1, const char *s2, Paytm_size_t len);
int32 Paytm_vsnprintf(char *str, Paytm_size_t size, const char *format, ...);
int32 Paytm_snprintf(char *str, Paytm_size_t size, const char *format, ...);
int32 Paytm_sscanf(const char* , const char*, ...);
int32 Paytm_IsEmptyStr(const char *str);
int32 Paytm_isspace(char ch);
int32 Paytm_isalpha(char ch);
int32 Paytm_islower(char ch);
int32 Paytm_isupper(char ch);
int32 Paytm_isdigit(char c);
int32 Paytm_ishexdigit(char ch);
int32 Paytm_toupper(int32 c);
int32 Paytm_tolower(int32 c);
uint8 Paytm_IsEmptyBuffer(const uint8 *buffer, Paytm_size_t buffer_len);
int32 Paytm_bin2hex(const uint8 *bytearrray, uint32 bytearrray_length, char *hex_str, uint32 *hex_len, char separator);
int32 Paytm_hex2bin(const char *hex_str, uint32 hex_len, uint8 *bytearrray, uint32 *bytearrray_length);

/*********************************    TIME FUNCTIONS    ***********************************/

typedef struct {
    int32 month;
    int32 day;
    int32 hour;
    int32 minute;
    int32 second;
    int32 wday;

    int32 year;
    int32 millisecond;
    int32 timezone;
}Paytm_Time;

int32 Paytm_SetTime(Paytm_Time *dt);
int32 Paytm_GetTime(Paytm_Time *dt);
int32 Paytm_GetLocalTime(Paytm_Time *dt);
uint64 Paytm_Mktime(Paytm_Time* dt);
uint64 Paytm_millis(void);
void Paytm_delayMilliSeconds(uint32 msec);
/* UTC+8, zone = 8 * 60; UTC +5.5, zone = 5.5 * 60; UTC-3, zone = -3 * 60 */
void Paytm_SetTimezone(int32_t zone);
int32_t Paytm_GetTimezone(void);

/*********************************    TIMER FUNCTIONS    ***********************************/

#define TIMER_COUNT_MAX (10)
/* The timer_id input limits from 0 to 9 */
int32 Paytm_Timer_Register(uint32 timer_id, void* callback_onTimer, void* param);
int32 Paytm_Timer_RegisterFast(uint32 timer_id, void* callback_onTimer, void* param);
int32 Paytm_Timer_Start(uint32 timer_id, uint32 interval_msec, uint8 autoRepeat);
int32 Paytm_Timer_Stop(uint32 timer_id);

/*********************************    LOGGING FUNCTIONS    ***********************************/

void Paytm_TRACE_BUFFER_WRITE(uint8 *content, Paytm_size_t content_len, uint8 includeCRLF);
void Paytm_TRACE_TAG_DATA(const char *tag, uint8 *content, Paytm_size_t content_len);
void Paytm_TRACE_HEX_BUFFER(const char *tag, const uint8 *content, Paytm_size_t content_len);
int32 Paytm_TRACE_PREFIXED(const char *format, va_list args, const char *prefix_format, ...);
int32 Paytm_TRACE(const char *format, ...);
int32 Paytm_TRACE_FUNC(const char *tag, const char *fn, const char *format, ...);
int32 Paytm_TRACE_DATETIME(const char *level, const char *tag, const char *fn, const char *format, ...);
int32 Paytm_TRACE_DATETIME_PAYTM(const char *level, const char *datetime, const char *tag, const char *format, ...);
int Paytm_Logs_Enable();
int Paytm_Logs_Disable();
int Paytm_SDKLogs_Enable();
int Paytm_SDKLogs_Disable();

/*********************************    RANDOM FUNCTIONS    ***********************************/

void Paytm_random_init(void);
int32 Paytm_random(void);

/********************************    TASKS RELATED FUNCTIONS    *********************************/

typedef struct{
    uint32_t message;
    uint32_t param1;
    uint32_t param2;
    uint32_t srcTaskID;
}ST_MSG;

typedef struct 
{
    int task_id;

}ST_TaskConfig;

typedef struct 
{
    uint8_t task_id;
    uint8_t msg_id; 
    osiThread_t * task;
    osiMessageQueue_t * msg_q;
}complex_task_t;

typedef struct 
{
    uint8_t mutex_id;
    char * name;
    osiMutex_t * mutex;
}cmplex_mutex_t;


void SetTagForTaskID(int32 taskId, const char *label);
int32 Paytm_GetActiveTaskId(void);

//0-sucess  -1-fail or no msg
int32 Paytm_GetMessage(int32 taskId, ST_MSG *msg);
int32 Paytm_GetMessage_From_ISR(int32 taskId, ST_MSG *msg);
//0-success  <0 - fail
int32 Paytm_SendMessage(int32 destTaskId, uint32_t message, uint32 param1, uint32 param2);
int32 Paytm_SendMessage_From_ISR(int32 id, uint32_t message, uint32 param1, uint32 param2);

int64 Paytm_GetTotalAssignedStackSize(const ST_TaskConfig taskConfig[], uint8 count);

int64 Paytm_GetTotalStackSize(void);

int64 Paytm_GetCurrentTaskFreeStackSize(void);

//mutex_id-success  -1-fail
int32 Paytm_CreateMutex(char *mutex_name);

//true-sucesss other-fail
int32 Paytm_TakeMutex(uint32 mutex_id, uint32 timeout_msec);

//0-success -1-fail
int32 Paytm_GiveMutex(uint32 mutex_id);

int32_t Paytm_CreateTask(const char *name, void* entry, void *argument, uint8_t priority, uint32_t stack_size);

#endif