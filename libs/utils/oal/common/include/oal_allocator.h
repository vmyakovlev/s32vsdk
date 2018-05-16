/*
 * (C) Copyright CogniVue Corporation. 2016 All right reserved.
 * Copyright (c) 2016 Freescale Semiconductor
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef APEXALLOCATOR_H
#define APEXALLOCATOR_H

#ifdef __STANDALONE__
  #define bool int8_t
  #define true 1
  #define false -1
  #include <stdint.h>
#else
  #include <linux/types.h>
#endif



// SASHBY: hard coded value bad!!!
// #define MAX_ADDRSLIST   0x8000
// #define MAX_CHUNKLIST   0x400
#define MAX_CHUNKLIST   0x8000
#define MAX_ADDRSLIST   ((MAX_CHUNKLIST -2) / 2)
#define OAL_MAX_ALLOCATOR_NUM 8

#ifdef __cplusplus
extern "C" {
#endif

#define EXPORT extern

   /* Functions */
   EXPORT void         apex_allocator_mmap_init(uint8_t space_id, uint64_t base, uint64_t size);
   EXPORT void         apex_allocator_mmap_deinit(uint8_t space_id);
   EXPORT void*        apex_allocator_malloc(uint8_t space_id, uint64_t size, const uint64_t align);
   EXPORT void         apex_allocator_free(uint8_t space_id, void *addr);
   
   EXPORT uint64_t     apex_allocator_get_physical_base(uint8_t space_id);
   EXPORT int64_t      apex_allocator_get_total_size(uint8_t space_id);
   EXPORT int64_t      apex_allocator_get_free_size(uint8_t space_id);


#ifdef __cplusplus
}
#endif

#endif /* APEXALLOCATOR_H */

