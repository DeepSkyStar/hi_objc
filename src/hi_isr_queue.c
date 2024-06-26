/**
 * @file hi_isr_queue.c
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
#include "hi_isr_queue.h"

void hi_isr_queue_init(hi_isr_queue_t *queue)
{
#if _HI_FOR_FREERTOS
    if (queue->len > 0) {
        queue->queue = xQueueCreate(queue->len, queue->item_size);
    }
#endif
}

hi_result_t hi_isr_queue_send(hi_isr_queue_t *queue, void *item, hi_time_t time_to_wait)
{
#if _HI_FOR_FREERTOS
    if (queue->len > 0) {
        if (xQueueSend(queue->queue, item, time_to_wait * portTICK_PERIOD_MS)) {
            return HI_RESULT_MAKE_OK;
        }
        else {
            return HI_RESULT_MAKE_FAILED(HI_FAILED_REASON_OUT_OF_INDEX);
        }
    }
#endif

    return HI_RESULT_MAKE_FAILED(HI_FAILED_REASON_NOT_SUPPORT);
}

hi_result_t hi_isr_queue_send_fromISR(hi_isr_queue_t *queue, void *item, hi_priority_t priority)
{
#if _HI_FOR_FREERTOS
    if (queue->len > 0) {
        //TODO: Define the priority map.
        if (xQueueSendFromISR(queue->queue, item, NULL)) {
            return HI_RESULT_MAKE_OK;
        }
        else {
            return HI_RESULT_MAKE_FAILED(HI_FAILED_REASON_SYSTEM);
        }
    }
    else {
        return HI_RESULT_MAKE_FAILED(HI_FAILED_REASON_OUT_OF_INDEX);
    }
#else
    return HI_RESULT_MAKE_FAILED(HI_FAILED_REASON_NOT_SUPPORT);
#endif
}

hi_result_t hi_isr_queue_recv(hi_isr_queue_t *queue, void *item, hi_time_t time_to_wait)
{
#if _HI_FOR_FREERTOS
    if (queue->len > 0) {
        if (xQueueReceive(queue->queue, item, time_to_wait * portTICK_PERIOD_MS)){
            return HI_RESULT_MAKE_OK;
        }
        else {
            return HI_RESULT_MAKE_FAILED(HI_FAILED_REASON_SYSTEM);
        }
    }
    else {
        return HI_RESULT_MAKE_FAILED(HI_FAILED_REASON_OUT_OF_INDEX);
    }
#else
    return HI_RESULT_MAKE_FAILED(HI_FAILED_REASON_NOT_SUPPORT);
#endif
}

void hi_isr_queue_deinit(hi_isr_queue_t *queue)
{
#if _HI_FOR_FREERTOS
    if (queue->len > 0 && queue->queue)
    {
        xQueueReset(queue->queue);
        queue->queue = NULL;
    }
#endif
}
