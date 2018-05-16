/*
 * Copyright (c) 2014 Freescale Semiconductor
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
#include <oal_cma_list.h>
#include <oal_allocator.h>
#include <oal_debug_out.h>

#ifdef __STANDALONE__
  #include <stdlib.h>
#else
  #include <linux/vmalloc.h>
  #include <linux/slab.h>
#endif
#include <oal_allocator_defines.h>

//////////////////////////////////////////////////////////////////////////////////
// Static variables
//////////////////////////////////////////////////////////////////////////////////

// CMA allocations list
t_MemArea **cma_list[OAL_MAX_ALLOCATOR_NUM] = {0};
int32_t    cma_base_addresses[OAL_MAX_ALLOCATOR_NUM] = {0};
int32_t    cma_end_addresses [OAL_MAX_ALLOCATOR_NUM] = {0};
int32_t    cma_alignment     [OAL_MAX_ALLOCATOR_NUM] = {0};
extern uint64_t oal_alloc_allocated_num[OAL_MAX_ALLOCATOR_NUM];

int32_t cma_list_free_pid(int32_t pid)
{
  int i, j;
  for (i=0; i < OAL_MAX_ALLOCATOR_NUM; ++i)
  {
    int32_t size = (cma_end_addresses[i] - cma_base_addresses[i])/cma_alignment[i];
    for (j = 0; j < size; ++j)
    {
      if (cma_list[i][j] != NULL && cma_list[i][j]->pid == pid)
      {
        // Free allocated memory and free the structure 
        --oal_alloc_allocated_num[cma_list[i][j]->chunk_id];
        apex_allocator_free(cma_list[i][j]->chunk_id, (void *)cma_list[i][j]->address[ACCESS_PHY]);
        
        #ifdef __STANDALONE__
          free(cma_list[i][j]);
        #else
          vfree(cma_list[i][j]);
        #endif
        cma_list[i][j] = NULL;
      }
    }
  }
  return 0;
}

int32_t cma_list_free_all()
{
  int i, j;
  for (i=0; i < OAL_MAX_ALLOCATOR_NUM; ++i)
  {
    int32_t size = (cma_end_addresses[i] - cma_base_addresses[i])/cma_alignment[i];
    for (j = 0; j < size; ++j)
    {
      if (cma_list[i][j] != NULL)
      {
        // Free allocated memory and free the structure 
        --oal_alloc_allocated_num[cma_list[i][j]->chunk_id];
        apex_allocator_free(cma_list[i][j]->chunk_id, (void *)cma_list[i][j]->address[ACCESS_PHY]);
        
        #ifdef __STANDALONE__
          free(cma_list[i][j]);
        #else
          vfree(cma_list[i][j]);
        #endif
        cma_list[i][j] = NULL;
      }
    }
  }
  return 0;
}

int32_t cma_list_init(int32_t id, int32_t base_address, int32_t end_address, int32_t alignment)
{
  cma_base_addresses[id] = base_address;
  cma_end_addresses[id] =  end_address;
  cma_alignment[id]     = alignment;
  #ifdef __STANDALONE__
    cma_list[id] = malloc(((end_address - base_address)/alignment) * sizeof(t_MemArea **));
  #else
    cma_list[id] = vmalloc(((end_address - base_address)/alignment) * sizeof(t_MemArea **));
  #endif
  memset(cma_list[id], 0, ((end_address - base_address)/alignment) * sizeof(t_MemArea **));
  return CMA_OP_OK;
}

int32_t cma_list_deinit()
{
  int i;
  for (i = 0; i < OAL_MAX_ALLOCATOR_NUM; ++i)
  {
    if (cma_list[i] != NULL)
    {
      #ifdef __STANDALONE__
        free(cma_list[i]);
      #else
        vfree(cma_list[i]);
      #endif
      cma_list[i] = NULL;
    }
  }
  return CMA_OP_OK;
}
//////////////////////////////////////////////////////////////////////////////////
// Pushes the new area into managed physical allocations. 
// Does not map into user space!
// Returns CMA_OP_OK on success, CMA_OP_ERROR otherwise.
//////////////////////////////////////////////////////////////////////////////////
int32_t cma_list_area_push_back(t_MemArea *area)
{
  int32_t retval = CMA_OP_OK;
  
  if (area)
  {
    cma_list[area->chunk_id][((int32_t)area->address[ACCESS_PHY]-cma_base_addresses[area->chunk_id]) / cma_alignment[area->chunk_id]] = area;
  }
 
  return retval;
}

//////////////////////////////////////////////////////////////////////////////////
// Finds appropriate t_MemArea structure by physical address.
// Returns pointer to this structure or NULL if not found.
//////////////////////////////////////////////////////////////////////////////////
t_MemArea * cma_list_area_find(uint64_t addr)
{
  t_MemArea *retval = NULL;
  int32_t chunk_id = -1;
  int32_t i;
  
  for (i = 0; i < OAL_MAX_ALLOCATOR_NUM; ++i)
  {
    if (cma_base_addresses[i] <= (uint32_t)addr && cma_end_addresses[i] > (uint32_t)addr)
    {
      chunk_id = i;
      break;
    }
  }
  if (chunk_id >= 0 && addr % cma_alignment[chunk_id] == 0)
  {
    retval = cma_list[chunk_id][((int32_t)addr-cma_base_addresses[chunk_id]) / cma_alignment[chunk_id]];
  }
  
  return retval;
}


//////////////////////////////////////////////////////////////////////////////////
// Removes a managed physical allocation from the list.
// Successful only if there is no virtual map.
// Returns CMA_OP_OK on success, CMA_OP_ERROR otherwise.
// Does not free *area pointer!
//////////////////////////////////////////////////////////////////////////////////
int32_t cma_list_area_remove(t_MemArea *area)
{  
  int32_t retval = CMA_OP_OK;
  if (area)
  {
    cma_list[area->chunk_id][((int32_t)area->address[ACCESS_PHY]-cma_base_addresses[area->chunk_id]) / cma_alignment[area->chunk_id]] = NULL;
  }
  
  return retval;
}