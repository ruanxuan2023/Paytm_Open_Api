#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cJSON.h"

#include "paytm_sys_api.h"
#include "paytm_typedef.h"

void cJsonCreate(void)
{
    cJSON* usr;
    usr = cJSON_CreateObject();

    char* property_json;

    cJSON_AddStringToObject(usr, "WifiApBssid", "CISE");
    cJSON_AddNumberToObject(usr, "LightTime", 1008); 

    property_json = cJSON_Print(usr); 

    // 释放内存  
    cJSON_Delete(usr);
    Paytm_TRACE("%s", property_json);
}

/**
 * test val json
 {
	"SceneID": 6,
	"Enable": 1,
	"Count": 1,
	"Data": [{
		"BrightValue": 37
	}, {
		"ColorTempValue": 4937
	}]
}
*/
uint8 cJsonPrase(const char*  val)
{
    cJSON *root = cJSON_Parse(val);
    cJSON *code;
    cJSON *arrayItem;
    cJSON *bVal;
    cJSON *tVal;

    int array_num = 0;
    
    if(!root){
        return 1;
    }

    code = cJSON_GetObjectItem(root, "SceneID");
    if(code != NULL){
        Paytm_TRACE("SceneID = %d", code->valueint);
    }

    code = cJSON_GetObjectItem(root, "Enable");
    if(code != NULL){
        Paytm_TRACE("Enable = %d", code->valueint);
    }

    code = cJSON_GetObjectItem(root, "Count");
    if(code != NULL){
        Paytm_TRACE("Count = %d", code->valueint);
    }
    
    code = cJSON_GetObjectItem(root, "Data");
    
    if(code != NULL){

        array_num = cJSON_GetArraySize(code);
        
        for(int i = 0; i < array_num; i++){

            arrayItem = cJSON_GetArrayItem(code, i);
    
            if(arrayItem == NULL)
            {
                Paytm_TRACE("Can`t get array obj\r\n");
                break;
            }

            bVal = cJSON_GetObjectItem(arrayItem, "BrightValue");
            if(bVal)
            {
                Paytm_TRACE("BrightValue = %d", bVal->valueint);
            }

            tVal = cJSON_GetObjectItem(arrayItem, "ColorTempValue");
            if(tVal)
            {
                Paytm_TRACE("ColorTempValue = %d", tVal->valueint);
            }
        }
    }
    
    cJSON_Delete(root);    
    
    return 0;
}

