/*
 * Copyright (c) 2014-2016 Freescale Semiconductor
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
#ifndef HEADERIOCTLFUNC_H
#define HEADERIOCTLFUNC_H

#include <oal_allocator.h>

#ifdef __cplusplus
extern "C"
{
#endif
  
/**
 * Function allocates a contiguous memory block and add an item into management list.
 * Returns allocated HW address if success, 0 otherwise.
 */
uint64_t oal_alloc(uint64_t size, uint64_t align, uint8_t chunk_id);

/**
 * Function removes a contiguous memory block and removes an entry from the management list.
 * Returns 0 if success and ULONG_MAX if fail.
 * Does not free virtual mappings.
 */
uint64_t oal_free (uint64_t handle_pointer);
uint64_t oal_free_phys (uint64_t physical_pointer);
uint64_t oal_unmap (uint64_t handle_pointer);

/**
 * Function returns number of allocations.
 */
uint64_t oal_get_num_allocations (void);

/**
 * Function returns number of mappings.
 */
uint64_t oal_get_num_mappings (void);

/**
 * Function returns a size of allocated memory block.
 */
uint64_t oal_getsize(uint64_t handle_pointer);

void oal_getmappings(uint64_t handle_pointer, uint64_t mappings[6]);
void oal_getmappingshw(uint64_t handle_pointer, uint64_t mappings[6]);

/** 
 * Function returns a specified buffer based on handle pointer.
 */
uint64_t oal_getbuffer(uint64_t handle_pointer, uint64_t type);

/**
 * Function adds a new memory mapping into the management list.
 */
uint64_t oal_map(uint64_t handle_pointer, uint64_t type, uint64_t virtual_pointer);
uint64_t oal_map_phys(uint64_t physical_pointer, uint64_t type, uint64_t virtual_pointer);
uint64_t oal_map_new(uint64_t type, uint64_t virtual_pointer, uint64_t size);

int64_t oal_memorysizetotal(void);
int64_t oal_memorysizefree(void);

#ifdef __cplusplus
}
#endif

#endif /* HEADERIOCTLFUNC_H */
