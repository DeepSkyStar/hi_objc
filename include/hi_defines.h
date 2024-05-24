/**
 * @file hi_defines.h
 * @author Cosmade (deepskystar@outlook.com)
 * @brief 
 * @version
 * @date 2024-05-06
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

#ifndef HI_DEFINES_H_
#define HI_DEFINES_H_


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

//Chips & Systems Define Here.
//System and chip's define provide by hi_system.h.in, it will automatic generate the hi_system.h
// #define _HI_NON_SYS (1)
// #define _HI_FREERTOS (1)
// #define _HI_WIN (1)
// #define _HI_LINUX (1)
// #define _HI_ANDROID (1)
// #define _HI_MACOS (1)
// #define _HI_IOS (1)
// #define _HI_TVOS (1)
// #define _HI_WATCHOS (1)

// #define _HI_ESP32 (1)

#if _HI_FREERTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#endif

#if _HI_ESP32
#include "esp_log.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    HI_OS_NON_SYS = 0,      //_HI_NON_SYS
    HI_OS_FREERTOR = 1,     //_HI_FREERTOR
    HI_OS_WIN = 2,          //_HI_WIN
    HI_OS_LINUX = 3,        //_HI_LINUX
    HI_OS_ANDROID = 4,      //_HI_LINUX
    HI_OS_MACOS = 5,        //_HI_MACOS
    HI_OS_IOS = 6,          //_HI_IOS
    HI_OS_WATCHOS = 7,      //_HI_WATCHOS
    HI_OS_TVOS = 8,         //_HI_TVOS
}hi_os_t;
extern hi_os_t hi_get_os(void);

#define HI_MAX_SIZE ((hi_size_t)-1)
typedef size_t hi_size_t;
typedef char* hi_str_t;
typedef void* hi_ptr_t;

#ifdef __cplusplus
}
#endif

#endif