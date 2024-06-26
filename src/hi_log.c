/**
 * @file hi_log.c
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

#include "hi_sys.h"
#include "hi_log.h"

void hi_print_hex(const uint8_t *data, hi_size_t size)
{
#ifndef BOOTLOADER_BUILD
#ifndef _HI_NO_DEBUG_LOG
    printf("{");
    for (int i = 0; i < size; i++)
    {
        if (i != 0)
        {
            printf(",");
        }
        printf("%02x", data[i]);
    }
    printf("}\n");
#endif
#endif
}