/**
 * @file hi_memory.c
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

#include "hi_log.h"
#include "hi_memory.h"

inline void* hi_memset(void* src, int value, hi_size_t size)
{
    return memset(src, value, size);
}

inline void* hi_memcpy(void* dst, const void* src, hi_size_t size)
{
    return memcpy(dst, src, size);
}

inline void* hi_memmove(void* dst, const void* src, hi_size_t size)
{
    return memmove(dst, src, size);
}

inline void* hi_malloc(hi_size_t size)
{
    return malloc(size);
}

inline void* hi_calloc(hi_size_t num, hi_size_t size)
{
    return calloc(num, size);
}

inline void* hi_realloc(void* ptr, hi_size_t size)
{
    return realloc(ptr, size);
}

inline void hi_free(void* ptr)
{
    free(ptr);
}

inline hi_mem_block_pool_t* hi_mem_block_pool_new(hi_mem_pool_config_t config)
{
    if (config.block_size < sizeof(hi_size_t)) {
        config.block_size = sizeof(hi_size_t);
    }
    //Make sure the block size is even.
    if ((config.block_size & 1) == 1) {
        config.block_size += 1;
    }
    //The minimun size for loop pool work.
    hi_mem_block_pool_t* pool = (hi_mem_block_pool_t*)hi_malloc(sizeof(hi_mem_block_pool_t) + config.block_size * (config.block_count + (config.block_count >> 3) + 1));
    hi_memset(pool, 0, sizeof(hi_mem_block_pool_t) + config.block_size * (config.block_count + (config.block_count >> 3) + 1));
    pool->reuse = HI_ITER_NULL;
    pool->cur = 0;
    pool->config = config;
    pool->container = (uint8_t *)pool + sizeof(hi_mem_block_pool_t);
    return pool;
}

inline hi_mem_block_pool_t* hi_mem_block_pool_realloc(hi_mem_block_pool_t* pool, hi_size_t block_count)
{

    pool = hi_realloc(pool, sizeof(hi_mem_block_pool_t) + pool->config.block_size * (block_count + (block_count >> 3) + 1));
    //TODO: when realloc, should set memory 0.
    pool->config.block_count = block_count;
    pool->container = (uint8_t *)pool + sizeof(hi_mem_block_pool_t);
    return pool;
}

inline void hi_mem_block_pool_free(hi_mem_block_pool_t* pool)
{
    hi_free(pool);
}

inline hi_iter_t hi_mem_block_pool_take(hi_mem_block_pool_t* pool)
{
    assert(pool->config.block_size >= sizeof(hi_iter_t));
    hi_iter_t block = HI_ITER_NULL;
    if (pool->reuse != HI_ITER_NULL) {
        block = pool->reuse;
        pool->reuse = *(hi_iter_t*)(HI_MEM_POOL_START(pool) + block);  //Get next pointer.
        if (pool->config.use_check) HI_MEM_POOL_SET_USE(pool, block);
        // HI_LOGD("take 1 %zu", block);
        return block;
    }
    if (pool->cur < pool->config.block_size * pool->config.block_count) {
        block = pool->cur;
        pool->cur += pool->config.block_size;
        if (pool->config.use_check) HI_MEM_POOL_SET_USE(pool, block);
        // HI_LOGD("take 2 %zu", block);
    }
    return block;
}

inline uint8_t* hi_mem_block_pool_get(hi_mem_block_pool_t* pool, hi_iter_t iter)
{
    if (pool->config.use_check && HI_MEM_POOL_IS_IN_USE(pool, iter) == 0) return NULL;
    return (HI_MEM_POOL_START(pool) + iter);
}

inline void hi_mem_block_pool_bring(hi_mem_block_pool_t* pool, hi_iter_t block)
{
    if (pool->config.use_check && HI_MEM_POOL_IS_IN_USE(pool, block) == 0) return ;

    if (block + pool->config.block_size == pool->cur)
    {
        pool->cur -= pool->config.block_size;
        if (pool->config.use_check) HI_MEM_POOL_SET_FREE(pool, block);
        // HI_LOGD("bring 1 %zu", block);
        return;
    }

    *((hi_iter_t*)(HI_MEM_POOL_START(pool) + block)) = pool->reuse;
    pool->reuse = block;
    if (pool->config.use_check) HI_MEM_POOL_SET_FREE(pool, block);
    // HI_LOGD("bring 2 %zu", block);
}

inline void* hi_mem_limit_pool_take(hi_mem_limit_pool_t* pool, hi_size_t size)
{
    if (size + pool->usage > pool->max_size) return NULL;
    pool->usage += size;
    return hi_malloc(size);
}

inline void hi_var_mem_pool_bring(hi_mem_limit_pool_t* pool, void* data, hi_size_t size)
{
    if (size > pool->usage) {
        HI_LOGE("hi_var_mem_pool_bring: bring size is bigger than pool usage.");
    }
    return hi_free(data);
}