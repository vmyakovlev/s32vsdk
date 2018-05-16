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
#ifndef STRUCTDEFS_H
#define STRUCTDEFS_H

#ifdef __STANDALONE__
  #include <stdint.h>
#else
  #include <linux/module.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
  
typedef struct UserAddress t_UserAddress;
typedef struct MemArea t_MemArea;

/**
 * Structure saves info about user space mappings
 */
typedef struct UserAddress
{
  /**
   * Process id
   */
  uint32_t pid;
  
  /**
   * User virtual memory
   */
  uint64_t user_virtual;
  
  /**
   * Next in double-linked list
   */
  t_UserAddress *next;
  
  /**
   * Previous in double-linked list
   */
  t_UserAddress *prev;
} t_UserAddress;

/**
 * Structure saves info about kernel space contiguous memory allocation
 */
typedef struct MemArea
{
  /**
   * Process id
   */
  uint32_t pid;
  
  uint64_t address[5];
  
  /**
   * Allocation size
   */
  uint64_t size;
  
  /**
   * Memory chunk id
   */
  uint8_t chunk_id;
  
} t_MemArea;

#ifdef __cplusplus
}
#endif

#endif /* STRUCTDEFS_H */
