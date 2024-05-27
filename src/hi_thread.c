/**
 * @file hi_thread.c
 * @author Cosmade (deepskystar@outlook.com)
 * @brief 
 * @version
 * @date 2024-05-27
 * 
 * @copyright Copyright 2024 Cosmade
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "hi_thread.h"

inline int hi_mutex_init(hi_mutex_t *mutex)
{
#if _HI_PTHREAD
    return pthread_mutex_init(&mutex->mutex, NULL);
#elif _HI_FREERTOS
    SemaphoreHandle_t mutex;
#else
    return -1;
#endif
}

inline int hi_mutex_lock(hi_mutex_t *mutex)
{
#if _HI_PTHREAD
    return pthread_mutex_lock(&mutex->mutex);
#elif _HI_FREERTOS
    return xSemaphoreTake(mutex->mutex, portMAX_DELAY);
#else
    return -1;
#endif
}

inline int hi_mutex_unlock(hi_mutex_t *mutex)
{
#if _HI_PTHREAD
    return pthread_mutex_init(&mutex->mutex, NULL);
#elif _HI_FREERTOS
    return xSemaphoreGive(mutex->mutex);
#else
    return -1;
#endif
}

inline int hi_mutex_deinit(hi_mutex_t *mutex)
{
#if _HI_PTHREAD
    return pthread_mutex_destroy(&mutex->mutex);
#elif _HI_FREERTOS
    return vSemaphoreDelete(mutex->mutex);
#else
    return -1;
#endif
}

inline int hi_thread_init(hi_thread_t *thread)
{
#if _HI_PTHREAD
    //TODO: fill out thread attr.
    return pthread_create(&thread->handle, NULL, thread->func, thread->args);
#elif _HI_FREERTOS
    //TODO: make translate from hi_priority to freertos.
    xTaskCreate((void)thread->func,
                thread->name,
                (thread->stackDepth == 0)?:thread->stackDepth,
                thread->args,
                thread->priority,
                thread->handle);
#else
    return -1;
#endif
}

inline int hi_thread_join(hi_thread_t *thread)
{
#if _HI_PTHREAD
    //TODO: fill out thread attr.
    return pthread_join(thread->handle, NULL);
#elif _HI_FREERTOS
    return xTaskWaitForEnd(thread->handle, portMAX_DELAY);
#else
    return -1;
#endif
}

inline void hi_thread_cancel(hi_thread_t *thread)
{
    //TODO: to be implementation.
}

inline void hi_thread_deinit(void)
{
#if _HI_PTHREAD
    //TODO: test it.
    pthread_exit(NULL);
#elif _HI_FREERTOS
    xTaskDelete(NULL);
#else
    return -1;
#endif
}


inline void hi_sleep(uint32_t millionseconds)
{
#if _HI_PTHREAD
    usleep(millionseconds*1000);
#elif _HI_FREERTOS
    vTaskDelay(millionseconds / portTICK_PERIOD_MS);
#else
    return;
#endif
}