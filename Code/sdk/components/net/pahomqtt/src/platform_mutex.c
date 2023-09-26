/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-15 18:27:19
 * @LastEditTime : 2020-01-08 20:23:13
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "platform_mutex.h"
#include "platform_timer.h"
#include "osi_api.h"

static int sys_sem_new(osiSemaphore_t *sem, uint8_t count)
{
	sem = osiSemaphoreCreate(1, count);
    if (NULL == sem)
	{
        return 0;
    }
	else
	{
        return -1;
    }
}

static int sys_sem_free(osiSemaphore_t *sem)
{
	osiSemaphoreDelete(sem);
	sem = MQTT_RTOS_SEM_NULL;

    return 0;
}

static int sys_sem_signal(osiSemaphore_t *sem)
{
	osiSemaphoreRelease(sem);

	return 0;
}

static int sys_sem_wait(osiSemaphore_t *sem, uint32_t timeout)
{
    bool status = true;
    int count;

    if (timeout == 0)
	{
        osiSemaphoreAcquire(sem);
    }
	else
	{
        count =  (timeout + (MQTT_RTOS_MS_ONE_TICK - 1)) / MQTT_RTOS_MS_ONE_TICK;
        status = osiSemaphoreTryAcquire(sem, count*5);
    }

    if (true == status)
	{
        return 0;
    }
	else
	{
		return -1;
	}
}


int platform_mutex_init(platform_mutex_t* m)
{
    return sys_sem_new(m->mutex, 1);
}

int platform_mutex_lock(platform_mutex_t* m)
{
    return sys_sem_wait(m->mutex, 0);
}

int platform_mutex_unlock(platform_mutex_t* m)
{
    return sys_sem_signal(m->mutex);
}

int platform_mutex_destroy(platform_mutex_t* m)
{
    return sys_sem_free(m->mutex);
}
