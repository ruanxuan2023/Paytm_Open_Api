#include "paytm_sys_api.h"

#include <stdlib.h>
#include <stdio.h>
#include "osi_api.h"

static  int task_1 = 0, task_2 = 0;

void * runTask_1(void* p)
{
    ST_MSG msg = {0};

    while (1)
    {
        Paytm_delayMilliSeconds(2000);

        if(Paytm_GetMessage(task_1, &msg) == 0)
        {
            Paytm_TRACE("Task1 recv msg: %s, %d, %d", msg.msg, msg.param1, msg.param2);
        }
    }
    
}

void * runTask_2(void* p)
{   
    int rc = 0;

    while (1)
    {
        Paytm_delayMilliSeconds(5000);

        rc = Paytm_SendMessage(task_1, 0, 26, 155545);
    }
}
static int sum = 0;
static int mutex_id = 0;

void * runTask_3(void* p)
{
    while (1)
    {
        Paytm_delayMilliSeconds(300);

        if(!Paytm_TakeMutex(mutex_id, 100))
        {
            Paytm_TRACE("A Take mutex fail!");
            continue;
        }

        sum++;
        Paytm_TRACE("A:SUM = %d", sum);
        Paytm_delayMilliSeconds(600);
        Paytm_GiveMutex(mutex_id);
    }
}

void * runTask_4(void* p)
{
    while (1)
    {
        Paytm_delayMilliSeconds(600);
        if(!Paytm_TakeMutex(mutex_id, 100))
        {
            Paytm_TRACE("B Take mutex fail!");
            continue;
        }

        sum++;
        Paytm_TRACE("B:SUM = %d", sum);
        Paytm_delayMilliSeconds(300);
        Paytm_GiveMutex(mutex_id);
    }
}

void tasksDemo(void)
{
    mutex_id = Paytm_CreateMutex("mutex_1");

    // task_1 = Paytm_CreateTask("it", runTask_1, NULL, 120, 3 * 1024);
    // task_2 = Paytm_CreateTask("he", runTask_2, NULL, 120, 3 * 1024);

    task_1 = Paytm_CreateTask("is", runTask_3, NULL, 120, 3 * 1024);
    task_2 = Paytm_CreateTask("du", runTask_4, NULL, 120, 3 * 1024);

    Paytm_TRACE("Create two task: %d %d",task_1, task_2);
}