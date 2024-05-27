/**
 * @file hi_thread.h
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

#ifndef HI_THREAD_H_
#define HI_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hi_defines.h"
#include "hi_error.h"

#if _HI_PTHREAD
#include <pthread.h>
#include <unistd.h>
#elif _HI_FREERTOS

#else

#endif

typedef struct
{

#if _HI_PTHREAD
    pthread_mutex_t mutex;
#elif _HI_FREERTOS
    SemaphoreHandle_t mutex;
#else
    uint32_t mutex;
#endif

}hi_mutex_t;

extern int hi_mutex_init(hi_mutex_t *mutex);
extern int hi_mutex_lock(hi_mutex_t *mutex);
extern int hi_mutex_unlock(hi_mutex_t *mutex);
extern int hi_mutex_deinit(hi_mutex_t *mutex);


typedef uint8_t hi_priority_t;
typedef enum{
    HI_PRIORITY_BACKGROUND = 0,   //for data storage or something not important.
    HI_PRIORITY_NORMAL = 1,   //usually task.
    HI_PRIORITY_RESPONSE = 2, //for response user input.
    HI_PRIORITY_DISPLAY = 3,  //for display sync.
    HI_PRIORITY_CRITICAL = 4, //for critical safety function.
}hi_priority_enum;

typedef void* (*hi_thread_func_f)(void* args);

typedef struct
{
#if _HI_PTHREAD
    pthread_t handle;
#elif _HI_FREERTOS
    TaskHandle_t handle;
#else
    uint32_t handle;
#endif
    hi_str_t name;
    hi_thread_func_f func;
    uint32_t stack_depth;   //stack depth for runloop task.
    hi_priority_t priority;   //the priority for runloop task.
    void *args;
}hi_thread_t;

#ifndef HI_THREAD_DEFAULT
#define HI_THREAD_DEFAULT(__name__, __func__) { \
    .name = __name__,   \
    .func = __func__,   \
    .stack_depth = 0,    \
    .priority = HI_PRIORITY_NORMAL, \
    .args = NULL,   \
}
#endif

extern int hi_thread_init(hi_thread_t *thread);
extern int hi_thread_join(hi_thread_t *thread);
extern void hi_thread_cancel(hi_thread_t *thread);
extern void hi_thread_deinit(void);
extern void hi_sleep(uint32_t millionseconds);

#ifdef __cplusplus
}
#endif

#endif /* HI_THREAD_H_ */