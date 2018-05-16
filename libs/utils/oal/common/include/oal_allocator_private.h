/*
 * (C) Copyright CogniVue Corporation. 2015-2016 All right reserved.
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

#ifndef APEXALLOCATORPRIVATE_H
#define APEXALLOCATORPRIVATE_H

#include "oal_allocator.h"

#ifdef __cplusplus
extern "C"
{
#endif
  
typedef struct AddrNode  AddrNode_t;
typedef struct ChunkNode ChunkNode_t;

typedef struct AddrNode
{
   uint32_t    addr;
   uint32_t    size;
   uint32_t    allocated_addr;
   AddrNode_t* pPrev;
   AddrNode_t* pNext;
} AddrNode_t;

typedef struct ChunkNode
{
   uint32_t     addr;
   uint32_t     size;
   bool         is_free;
   AddrNode_t   addr_sentinel;
   ChunkNode_t* pPrev;
   ChunkNode_t* pNext;
} ChunkNode_t;

typedef struct Allocator
{
   ChunkNode_t chunk_sentinel;
} Allocator_t;


/* Chunk helpers*/
static void        helper_list_init(ChunkNode_t* pSentinel);
static void        helper_list_addHead(ChunkNode_t* pSentinel, ChunkNode_t* pNodeToBeAdded);
static void        helper_list_addTail(ChunkNode_t* pSentinel, ChunkNode_t* pNodeToBeAdded);
static void        helper_list_remove(ChunkNode_t* pSentinel, ChunkNode_t* pNodeToBeRemoved);
static ChunkNode_t*  helper_list_getHead(ChunkNode_t* pSentinel, int32_t isRemove);
static ChunkNode_t*  helper_chunk_node_alloc(uint8_t space_id);
static void        helper_chunk_node_free(uint8_t space_id, ChunkNode_t* pChunkNode);


/* Address helpers */
static void        helper_list_init_addr(AddrNode_t* pSentinel);
static void        helper_list_addHead_addr(AddrNode_t* pSentinel, AddrNode_t* pNodeToBeAdded);
static void        helper_list_addTail_addr(AddrNode_t* pSentinel, AddrNode_t* pNodeToBeAdded);
static void        helper_list_remove_addr(AddrNode_t* pSentinel, AddrNode_t* pNodeToBeRemoved);
static AddrNode_t* helper_list_find(AddrNode_t* pSentinel, void* pAddress);
static void        helper_list_split(AddrNode_t* pSentinel, AddrNode_t* pSentinelNew, AddrNode_t* pAddrNode);
static void        helper_list_append(AddrNode_t* pSentinel0, AddrNode_t* pSentinel1);
static AddrNode_t* helper_list_getHead_addr(AddrNode_t* pSentinel, int32_t isRemove);
static AddrNode_t* helper_addr_node_alloc_addr(uint8_t space_id);
static void        helper_addr_node_free_addr(uint8_t space_id, AddrNode_t* pAddrNode);
static int32_t     helper_addr_compare(void* key, AddrNode_t* pAddrNode);

#ifdef __cplusplus
}
#endif

#endif /* APEXALLOCATORPRIVATE_H */

