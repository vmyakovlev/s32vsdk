/*
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
/**
 * \file     sram_func.c
 * \brief    functions of the sram driver
 * \author   Tomas Babinec
 * \version  0.1
 * \date     13.5.2014
 * \note
 ****************************************************************************/

#define VDB_LOG_NOTE_OFF

#if !defined(__STANDALONE__) && defined(__KERNEL__)
#include <linux/module.h>	// required for EXPORT_SYMBOL
#include <linux/slab.h>		// for kmalloc
#include <asm/io.h>		// for ioremap
#include <linux/string.h>

#else // #if !defined(__STANDALONE__) && defined(__KERNEL__)

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#endif // #if !defined(__STANDALONE__) && defined(__KERNEL__)

#include "vdb_log.h"
#include "sram.h"
#include "sram_func.h"
#include "sram_dl_mspace.h"

//*****************************************************************************
// Macros
//*****************************************************************************

//#define SRAM_DEBUG

//*****************************************************************************
// Command line arguments
//*****************************************************************************

//module_param(gsStaticSingleSize, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
//MODULE_PARM_DESC(gsStaticSingleSize, "Defines the size of static segment in the single banked region of SRAM.");

//module_param(gsStaticMultiSize, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
//MODULE_PARM_DESC(gsStaticMultiSize, "Defines the size of static segment in the Multi banked region of SRAM.");

//*****************************************************************************
// local data
//*****************************************************************************

static int32_t gsStaticSingleSize = 0;
static int32_t gsStaticMultiSize = 0;

//*****************************************************************************
// global data
//*****************************************************************************

// base address of the whole SRAM memory block
char *gpSramMemBlock = NULL;

#ifdef __STANDALONE__
  // use linker script symbols for standalone
  extern uint32_t __SRAM_SINGLE_S_START;
  extern uint32_t __SRAM_SINGLE_S_SIZE;
  extern uint32_t __SRAM_SINGLE_D_START;
  extern uint32_t __SRAM_SINGLE_D_SIZE;
  extern uint32_t __SRAM_MULTI_S_START;
  extern uint32_t __SRAM_MULTI_S_SIZE;
  extern uint32_t __SRAM_MULTI_D_START;
  extern uint32_t __SRAM_MULTI_D_SIZE;
#endif //#ifdef __STANDALONE__
  
// start addresses of different SRAM segments:
// 0: multi-banked static region
// 1: multi-banked dynamic mspace
// 2: single-banked static region
// 3: single-banked dynamic mspace
char *gSramSegments[4] = {0, 0, 0, 0};

// SRAM region sizes
uint32_t gSramSegmentSizes[4] = {0, SRAM_SINGLE_SIZE, 0, SRAM_MULTI_SIZE};

// real SRAM offset from the allocator version
ptrdiff_t gSRAM_V2pOffset = 0;

// single and multi-banked mspaces from DL allocator
mspace 	gSramSingleMspace = 0;
mspace 	gSramMultiMspace = 0;

#if !defined(__STANDALONE__) && defined(__KERNEL__)
  mutex_t gSramMutex;        ///< SRAM locking mutex
  // exported to DL allocator
  mutex_t magic_init_mutex;  ///< MAGIC mutex
  mutex_t mspace_mutex;      ///< MAGIC mutex
  mutex_t gMoreCoreMutex;    ///< more core mutex
#else // #if !defined(__STANDALONE__) && defined(__KERNEL__)
  // placeholders
  uint32_t gSramMutex;
  // exported to DL allocator
  uint32_t magic_init_mutex;
  uint32_t mspace_mutex;
  uint32_t gMoreCoreMutex;
#endif // #if !defined(__STANDALONE__) && defined(__KERNEL__)

//*****************************************************************************
// global functions
//*****************************************************************************

//#define SRAM_BASE 0xCA000000

int32_t SRAM_SetUp()
{
  int32_t aRetVal = SRAM_DRV_SUCCESS;
  size_t i = 0;
  uint64_t *lpSramBase = NULL;
  uint64_t *lpSramTmp  = NULL;

#if !defined(__STANDALONE__) && defined(__KERNEL__)
  VDB_LOG_NOTE("Ioremap SRAM region\n");
  gpSramMemBlock = (char*)ioremap(SRAM_BASE, SRAM_SIZE);
  lpSramBase = (uint64_t*)gpSramMemBlock;
#else
  VDB_LOG_NOTE("SRAM_BASE: 0x%x, SRAM_SIZE: 0x%x\n", (unsigned) SRAM_BASE, (unsigned) SRAM_SIZE);
  gpSramMemBlock = (char*)SRAM_BASE;
  lpSramBase = (uint64_t*)gpSramMemBlock;

#endif
  if(!gpSramMemBlock)
  {
    VDB_LOG_ERROR("SRAM memory %x ioremap failed.\n", (unsigned) SRAM_BASE);
    goto allocation_failed;
  }

  // calculate offset from virtual to physical SRAM address
  gSRAM_V2pOffset = (ptrdiff_t)SRAM_BASE - (uintptr_t)gpSramMemBlock;
  VDB_LOG_NOTE("v2p sram : %lld\n", (long long int)gSRAM_V2pOffset);
  VDB_LOG_NOTE("gpSramMemBlock: 0x%llx, SRAM_BASE: 0x%x\n", (uintptr_t) gpSramMemBlock, (unsigned) SRAM_BASE);
  
#if !defined(__STANDALONE__) && defined(__KERNEL__)
  // initialize mutexes
  mutex_init(&gSramMutex);
  mutex_init(&magic_init_mutex);
  mutex_init(&mspace_mutex);
  mutex_init(&gMoreCoreMutex);
#else
  gSramMutex        = 0;
  magic_init_mutex  = 0;
  mspace_mutex      = 0;
  gMoreCoreMutex    = 0;
#endif // else from #if !defined(__STANDALONE__) && defined(__KERNEL__)

#ifdef __STANDALONE__
  // use linker script symbols for standalone
  // set segment sizes
  gSramSegmentSizes[0] = (uint32_t)(uintptr_t)&__SRAM_SINGLE_S_SIZE;
  gSramSegmentSizes[1] = (uint32_t)(uintptr_t)&__SRAM_SINGLE_D_SIZE;
  gSramSegmentSizes[2] = (uint32_t)(uintptr_t)&__SRAM_MULTI_S_SIZE;
  gSramSegmentSizes[3] = (uint32_t)(uintptr_t)&__SRAM_MULTI_D_SIZE;
  
  // set segment base addresses
  gSramSegments[0] = (char*)&__SRAM_SINGLE_S_START;
  gSramSegments[1] = (char*)&__SRAM_SINGLE_D_START;
  gSramSegments[2] = (char*)&__SRAM_MULTI_S_START;
  gSramSegments[3] = (char*)&__SRAM_MULTI_D_START;
#else // ifdef __STANDALONE__
  // use harcoded values in Linux (will become obsolete with OAL SRAM allocator)
  // compute segment sizes
  gSramSegmentSizes[0] = gsStaticSingleSize;
  gSramSegmentSizes[1] = SRAM_SINGLE_SIZE - gsStaticSingleSize;
  gSramSegmentSizes[2] = gsStaticMultiSize;
  gSramSegmentSizes[3] = SRAM_MULTI_SIZE - gsStaticMultiSize;
  
  // set segment base addresses
  if(gSramSegmentSizes[0])
  {
    gSramSegments[0] = gpSramMemBlock;
  } // single-banked static segment defined
  gSramSegments[1] = gpSramMemBlock + gSramSegmentSizes[0];
  if(gSramSegmentSizes[2])
  {
    gSramSegments[2] = gpSramMemBlock + SRAM_SINGLE_SIZE;
  } // multi-banked static segment defined
  gSramSegments[3] = gpSramMemBlock + SRAM_SINGLE_SIZE + gSramSegmentSizes[2];
#endif // else from ifdef __STANDALONE__
  
#ifdef SRAM_DEBUG
  VDB_LOG_NOTE("SRAM segment sizes: %d, %d, %d, %d.\n",
    (int) gSramSegmentSizes[0],
    (int) gSramSegmentSizes[1],
    (int) gSramSegmentSizes[2],
    (int) gSramSegmentSizes[3]
  ); 
  VDB_LOG_NOTE("SRAM segment base addresses: %lx, %lx, %lx, %lx.\n",
    (uintptr_t) gSramSegments[0],
    (uintptr_t) gSramSegments[1],
    (uintptr_t) gSramSegments[2],
    (uintptr_t) gSramSegments[3]
  ); 
#endif // SRAM_DEBUG
  
  // *** init ECC with 64bit accesses for dynamic regions only ***
  // single-banked region
  lpSramTmp = (uint64_t*)gSramSegments[1];
  for(i = 0; i < (gSramSegmentSizes[1]/8); i ++)
  {
    lpSramTmp[i] = 0x0ULL;
  } // for the single-banked region  
  // multi-banked region
  lpSramTmp = (uint64_t*)gSramSegments[3];
  for(i = 0; i < (gSramSegmentSizes[3]/8); i ++)
  {
    lpSramTmp[i] = 0x0ULL;
  } // for the multi-banked region

  gSramMultiMspace = create_mspace_with_base((void*)gSramSegments[3], gSramSegmentSizes[3], 1);
  if(gSramMultiMspace == NULL)
  {
    VDB_LOG_ERROR("SRAM Mspace initialization failed.\n");
    goto multi_mspace_failed;
  }// if SRAM single-banked mspace creation failed
  
  
  // initialize single- and multi-banked mspaces
  gSramSingleMspace = create_mspace_with_base((void*)gSramSegments[1], gSramSegmentSizes[1], 1);
  if(gSramSingleMspace == NULL)
  {
    aRetVal = SRAM_DRV_FAILURE;
    VDB_LOG_ERROR("SRAM Mspace initialization failed.\n");
    goto single_mspace_failed;
  }// if SRAM single-banked mspace creation failed

#ifdef SRAM_DEBUG	
  // attempt simple allocation sequence
  {
    char *chunk_arr[20];
    int i = 1;
    
    VDB_LOG_NOTE("Multi mspace test\n");
    for(i = 1; i < 10; ++i)
    {
      chunk_arr[i] = NULL;
      chunk_arr[i] = mspace_malloc(gSramMultiMspace, 32768);
      if(chunk_arr[i] == NULL)
      {
        VDB_LOG_ERROR("%dth Failed\n", (int) i);
      } //if failed
      else
      {
        VDB_LOG_NOTE("%dth Succeeded\n", (int) i);
      } // else succeeded
    } // for all chunks
    
    
    // deallocate the test sequence
    for(i = 1; i < 10; ++i)
    {
      if(chunk_arr[i] != NULL)
      {
        VDB_LOG_NOTE("Free chunk %d\n", (int) i);
        mspace_free(gSramMultiMspace, chunk_arr[i]);
      } //if allocated
    } // for all chunks
    
    VDB_LOG_NOTE("Single mspace test\n");
    for(i = 1; i < 10; ++i)
    {
      chunk_arr[i] = NULL;
      chunk_arr[i] = mspace_malloc(gSramSingleMspace, 32768);
      if(chunk_arr[i] == NULL)
      {
        VDB_LOG_ERROR("%dth Failed\n", (int) i);
      } //if failed
      else
      {
        VDB_LOG_NOTE("%dth Succeeded\n", (int) i);
      } // else succeeded
    } // for all chunks
    
    
    // deallocate the test sequence
    for(i = 1; i < 10; ++i)
    {
      if(chunk_arr[i] != NULL)
      {
        VDB_LOG_NOTE("Free chunk %d\n", (int) i);
        mspace_free(gSramSingleMspace, chunk_arr[i]);
      } //if allocated
    } // for all chunks
  } // debug sram allocation sequence	
#endif // SRAM_DEBUG

  // init error processing
  if( aRetVal )
  {
    single_mspace_failed:
      VDB_LOG_NOTE("Destroy single mspace\n");
      destroy_mspace(gSramMultiMspace);  
    multi_mspace_failed:
#if !defined(__STANDALONE__) && defined(__KERNEL__)
      VDB_LOG_NOTE("Io unmap\n");
      iounmap(gpSramMemBlock);
#endif // #if !defined(__STANDALONE__) && defined(__KERNEL__)
      gpSramMemBlock = NULL;
    allocation_failed:
    
    aRetVal = SRAM_DRV_FAILURE;
  } // if some failure
  
  return aRetVal;
} // SRAM_Setup()

//*****************************************************************************

void SRAM_Close()
{
  //
  // free resources
  //

  // free single-banked SRAM mspace
  if( gSramSingleMspace != NULL)
  {
    VDB_LOG_NOTE("Freeing single-banked SRAM Mspace.\n");
    destroy_mspace(gSramSingleMspace);
  } // if Mspace exists
  
  // free multi-banked SRAM mspace
  if( gSramMultiMspace != NULL)
  {
    VDB_LOG_NOTE("Freeing multi-banked SRAM Mspace.\n");
    destroy_mspace(gSramMultiMspace);
  } // if Mspace exists

  // free SRAM memory model
  if( gpSramMemBlock != NULL )
  {
    VDB_LOG_NOTE("Freeing SRAM memory region model.\n");
#if !defined(__STANDALONE__) && defined(__KERNEL__)
    iounmap(gpSramMemBlock);
#endif // #if !defined(__STANDALONE__) && defined(__KERNEL__)
    gpSramMemBlock = NULL;
  } // if allocated
} // SRAM_Close()

//*****************************************************************************

uintptr_t SRAM_v2p(void* apVirtAddr)
{
  return (uintptr_t)(((uintptr_t)apVirtAddr) + gSRAM_V2pOffset);
} // SRAM_v2p()

//*****************************************************************************

void* SRAM_p2v(uintptr_t aPhysAddr)
{
  return (void*)((uintptr_t)(aPhysAddr - gSRAM_V2pOffset));
} // SRAM_p2v()

//*****************************************************************************

uintptr_t SRAM_SingleMalloc(uintptr_t aSize)
{
  uintptr_t lPhysAddr = 0;
  void* lpVirtAddr = NULL;
  
  lpVirtAddr = mspace_malloc(gSramSingleMspace, aSize);
  
  if(lpVirtAddr != 0)
  {
    lPhysAddr = SRAM_v2p(lpVirtAddr);
#ifdef SRAM_DEBUG
    VDB_LOG_NOTE("Allocated %u bytes at physical address: %lx with virtual \
    address %lx from single-banked region\n", (unsigned) aSize, lPhysAddr, (uintptr_t) lpVirtAddr);
#endif
  } // if allocation ok
  else
  {
    VDB_LOG_ERROR("Failed to allocate %u bytes from single-banked region.\n", (unsigned) aSize);
  } // else from if allocation ok
  
  return lPhysAddr;
} // SRAM_SingleMalloc()

//*****************************************************************************

uintptr_t SRAM_MultiMalloc(uintptr_t aSize)
{
  uintptr_t lPhysAddr = 0;
  void* lpVirtAddr = NULL;
  
  lpVirtAddr = mspace_malloc(gSramMultiMspace, aSize);

  if(lpVirtAddr != 0)
  {
    lPhysAddr = SRAM_v2p(lpVirtAddr);
#ifdef SRAM_DEBUG
    VDB_LOG_NOTE("Allocated %u bytes at physical address: %x with virtual \
      address %p from multi-banked region\n", 
      (unsigned) aSize, 
      (unsigned) lPhysAddr, 
      (void*) lpVirtAddr
    );
#endif
  } // if allocation ok
  else
  {
    VDB_LOG_WARNING("Failed to allocate %u bytes from multi-banked region.\n", 
                    (unsigned) aSize
                   );
  } // else from if allocation ok
  
  return lPhysAddr;
} // SRAM_MultiMalloc()

//*****************************************************************************

void SRAM_Free(uintptr_t aPhysAddr)
{
  void *lpVirtAddr = SRAM_p2v(aPhysAddr);
  if(((uintptr_t)lpVirtAddr) > (uintptr_t) (gSramSegments[3]))
  {
    mspace_free(gSramMultiMspace, lpVirtAddr);
  } // if single-banked segment
  else
  {
    mspace_free(gSramSingleMspace, lpVirtAddr);
  } // else from if single-banked segment
	    
#ifdef SRAM_DEBUG
  VDB_LOG_NOTE("Freed physical address: %x with virtual address %p\n", (unsigned) aPhysAddr, (void*) lpVirtAddr);
#endif
} // SRAM_Free()

//*****************************************************************************

#if !defined(__STANDALONE__) && defined(__KERNEL__)
EXPORT_SYMBOL(gSRAM_V2pOffset);
EXPORT_SYMBOL(SRAM_v2p);
EXPORT_SYMBOL(SRAM_p2v);
EXPORT_SYMBOL(SRAM_SingleMalloc);
EXPORT_SYMBOL(SRAM_MultiMalloc);
EXPORT_SYMBOL(SRAM_Free);
#endif // #if !defined(__STANDALONE__) && defined(__KERNEL__)
