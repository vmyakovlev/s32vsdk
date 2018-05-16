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
#ifndef HEADERIOCTLCMD_H
#define HEADERIOCTLCMD_H

#include <linux/fs.h>
#include <linux/types.h>

/*======================================================================*/
/* Driver global functions */
/*======================================================================*/

/**
 * Add new allocation into the list
 */
#define CMD_ALLOC 				_IOW('m',  0, int)
typedef struct
{
  uint64_t   size;
  uint64_t   align;
  uint64_t   ret_phys_pointer;
  uint8_t   chunk_id;
} CMD_ALLOC_TYPE;

/**
 * Remove the allocation from the list
 */
#define CMD_FREE 				_IOW('m',  1, int)
#define CMD_FREE_PHYS 				_IOW('m',  2, int)

/**
 * Get the size of the allocation
 */
#define CMD_SIZE 				_IOW('m',  3, int)

/**
 * Get specific buffer
 */
#define CMD_BUFFER				_IOW('m',  4, int)
typedef struct
{
  uint64_t   handle_pointer;
  uint64_t   type;
  uint64_t   ret_virt_pointer;
} CMD_BUFFER_TYPE;

/**
 * Create a new mapping for the current process
 */
#define CMD_MAP 				_IOW('m',  5, int)
#define CMD_MAP_PHYS 				_IOW('m',  6, int)
typedef struct
{
  uint64_t   handle_pointer;
  uint64_t   type;
  uint64_t   virtual_pointer;
} CMD_MAP_TYPE;
#define CMD_MAP_NEW 				_IOW('m',  7, int)
typedef struct
{
  uint64_t   type;
  uint64_t   virtual_pointer;
  uint64_t   size;
} CMD_MAP_NEW_TYPE;


#define CMD_FLUSH				_IOW('m',  8, int)
#define CMD_FLUSH_SPECIFIC 			_IOW('m',  9, int)
#define CMD_INVALIDATE_SPECIFIC 		_IOW('m', 10, int)
#define CMD_FLUSHINVALIDATE_SPECIFIC 		_IOW('m', 11, int)
typedef struct
{
  uint64_t   virtual_pointer;
  uint64_t   size;
} CMD_FLUSH_SPECIFIC_TYPE;

#define CMD_INFO				_IOW('m',  12, int)
#define CMD_INFO_MAP				_IOW('m', 13, int)

#define CMD_MAPPINGS				_IOW('m', 14, int)
#define CMD_MAPPINGSHW				_IOW('m', 15, int)


#define CMD_UNMAP 				_IOW('m', 16, int)
typedef struct
{
  uint64_t address[6];
} CMD_MAPPINGS_TYPE;

#define CMD_MUTEX                               _IOW('m', 17, int)
#define CMD_UNMUTEX                             _IOW('m', 18, int)
#define CMD_MEMORY_SIZE_FREE_GET                _IOR('m', 19, int64_t)
#define CMD_MEMORY_SIZE_TOTAL_GET               _IOR('m', 20, int64_t)

#define CMD_MEMORY_GET_DEVICES                  _IOR('m', 21, int)
#define CMD_MEMORY_GET_AUTOBALANCE              _IOR('m', 22, int)
#define CMD_MEMORY_GET_BASE                     _IOR('m', 23, int)
#define CMD_MEMORY_GET_SIZE                     _IOR('m', 24, int)

#define CMD_INTERRUPT_SET                       _IOR('m', 25, int)
typedef struct
{
  uint32_t irq_num;
  uint32_t mode;
  int32_t pid;
} CMD_INTERRUPT_SET_TYPE;

#define CMD_INTERRUPT_REMOVE                    _IOR('m', 26, int)

#define CMD_INTERRUPT_ENABLE                    _IOR('m', 27, int)
#define CMD_INTERRUPT_DISABLE                   _IOR('m', 28, int)

#endif /* HEADERIOCTLCMD_H */
