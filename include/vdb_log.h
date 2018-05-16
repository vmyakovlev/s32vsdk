/*
 * Copyright (c) 2015-2016 Freescale Semiconductor
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
 * \file    vdb_log.h
 * \brief   Headers for vision debug logging
 * \author  Tomas Babinec
 * \version 0.1
 * \date    Aug 29, 2013
 ****************************************************************************/

#ifndef VDBLOG_H
#define VDBLOG_H

#ifdef __cplusplus 
extern "C" { 
#endif

#ifndef __KERNEL__

#include <stdio.h>  // because of FILE structure definition
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#else // ifndef __KERNEL__

#include <linux/kernel.h>	// We're doing kernel work
#include <linux/module.h>	// Specifically, a module 
#include <linux/types.h>

#endif // ifndef __KERNEL__

/*****************************************************************************
 * defines
 *****************************************************************************/

#define VDB_LOG_BUF_MAX_LEN 256
#define VDB_LOG_NOTE_OFF
#define VDB_LOG_WARNING_OFF
//#define VDB_LOG_ERROR_OFF

// definition of __func__ macro in case of Integrity OS
#ifdef INTEGRITY
#define __func__ __FUNCTION__
#endif

/*****************************************************************************
 * macros
 *****************************************************************************/



/// predefine log message format for notes
/*#ifndef VDB_LOG_NOTE_OFF
#define VDB_LOG_NOTE(_format,...) \
  (void)vdb_Log(__FILE__":%d/%s : [Note]: " _format, __LINE__, __func__, ##__VA_ARGS__)
#else
#define VDB_LOG_NOTE(_format,...)
#endif*/

#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
     
#define __RSEQ_N() \
     2,2,2,2,                   \
     2,2,2,2,2,2,2,2,2,2, \
     2,2,2,2,2,2,2,2,2,2, \
     2,2,2,2,2,2,2,2,2,2, \
     2,2,2,2,2,2,2,2,2,2, \
     2,2,2,2,2,2,2,2,2,2, \
     2,2,2,2,2,2,2,2,1,0
     
// general definition for any function name
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

#ifndef VDB_LOG_NOTE_OFF
#ifdef __STANDALONE__
  #define VDB_LOG_NOTE(_format,...) (void)printf(__FILE__":%d/%s : [Note]: " _format, __LINE__, __func__, ##__VA_ARGS__)
#else // #ifdef __STANDALONE__
  #define VDB_LOG_NOTE2(_format,...) (void)vdb_Log(__FILE__":%d/%s : [Note]: " _format, __LINE__, __func__, ##__VA_ARGS__)
  #define VDB_LOG_NOTE1(_format)     (void)vdb_Log(__FILE__":%d/%s : [Note]: " _format, __LINE__, __func__)
  #define VDB_LOG_NOTE(...) VFUNC(VDB_LOG_NOTE, __VA_ARGS__)
#endif // else from #ifdef __STANDALONE__
#else // #ifndef VDB_LOG_NOTE_OFF
  #define VDB_LOG_NOTE2(_format,...) 
  #define VDB_LOG_NOTE1(_format)     
  #define VDB_LOG_NOTE(...) VFUNC(VDB_LOG_NOTE, __VA_ARGS__)
#endif // else from #ifndef VDB_LOG_NOTE_OFF

/// predefine log message format for warnings
#ifndef VDB_LOG_WARNING_OFF
#ifdef __STANDALONE__
  #define VDB_LOG_WARNING(_format,...) (void)printf(__FILE__":%d/%s : [Warning]: " _format, __LINE__, __func__, ##__VA_ARGS__)
#else // #ifdef __STANDALONE__
  #define VDB_LOG_WARNING2(_format,...) (void)vdb_Log(__FILE__":%d/%s : [Warning]: " _format, __LINE__, __func__, ##__VA_ARGS__)
  #define VDB_LOG_WARNING1(_format)     (void)vdb_Log(__FILE__":%d/%s : [Warning]: " _format, __LINE__, __func__)
  #define VDB_LOG_WARNING(...) VFUNC(VDB_LOG_WARNING, __VA_ARGS__)
#endif // else from #ifdef __STANDALONE__
#else // #else #ifndef VDB_LOG_WARNING_OFF
  #define VDB_LOG_WARNING2(_format,...)
  #define VDB_LOG_WARNING1(_format)
  #define VDB_LOG_WARNING(...)
#endif // #if 0

/// predefine log message format for errors
#ifndef VDB_LOG_ERROR_OFF
#ifdef __STANDALONE__//n
  #define VDB_LOG_ERROR(_format,...) (void)printf(__FILE__":%d/%s : [Error]: " _format, __LINE__, __func__, ##__VA_ARGS__)
#else // #ifdef __STANDALONE__//u
  #define VDB_LOG_ERROR2(_format,...) (void)vdb_Log(__FILE__": %d/%s : [Error]: " _format, __LINE__, __func__, ##__VA_ARGS__)
  #define VDB_LOG_ERROR1(_format)     (void)vdb_Log(__FILE__": %d/%s : [Error]: " _format, __LINE__, __func__)
  #define VDB_LOG_ERROR(...) VFUNC(VDB_LOG_ERROR, __VA_ARGS__)
#endif // else from #ifdef __STANDALONE__ 

#else // #else #ifndef VDB_LOG_ERROR_OFF
  #define VDB_LOG_ERROR2(_format,...)
  #define VDB_LOG_ERROR1(_format)
  #define VDB_LOG_ERROR(...)
#endif // #if 0

/// predefine log message for not implemented functions
#define VDB_LOG_FCN_NOT_IMPLEMENTED() VDB_LOG_WARNING("Function NOT implemented.\n")

/*****************************************************************************
 * functions (prototypes)
 *****************************************************************************/

/****************************************************************************/
/** Basic logging init function.
 * In user space the stdout is set as output.
 * In kernel space the kernel debug messaging will be used.
 * 
 * \return  0 if all ok, 
 *          1 if stream is NULL
 ****************************************************************************/
int32_t vdb_LogInit(void);

#if !defined(__KERNEL__) && !defined(__STANDALONE__) 

/****************************************************************************/
/** Initializes the logging output to stream or file.
 *
 * \param apStream stream to be opened
 * \return  0 if all ok, 
 *          1 if stream is NULL
 ****************************************************************************/
int32_t vdb_LogInitStr(FILE *apStream);

#endif //if !defined(__KERNEL__) && !defined(__STANDALONE__)

/****************************************************************************/
/** Sets up memory buffer logging.
 *
 *  User is responsible for releasing the memory.
 *
 * \param apMemAddr
 * \param aSize
 * \return  0 if all ok, 
 *          1 if mem buffer is bad
 ****************************************************************************/
int32_t vdb_LogInitMem(void *apMemAddr, size_t aSize);

/****************************************************************************/
/** Stops logging.
 *
 * Resets all previous logging settings to default => no log messages will be
 * written anywhere until another call to vdb_LogInit().
 *
 ****************************************************************************/
void vdb_LogStop(void);

/****************************************************************************/
/** Returns number of used bytes in memory buffer.
 *
 * \return number of used bytes in memory buffer
 ****************************************************************************/
size_t vdb_LogUsedMemGet(void);

/****************************************************************************/
/** Calls appropriate logging printf function.
 *
 * \param apcFormat string
 * 
 * \return number of characters/bytes logged
 ****************************************************************************/
size_t vdb_Log(const char *apcFormat, ...);
#if !defined(__KERNEL__) && !defined(__STANDALONE__)
/****************************************************************************/
/** Does simple formated printing into specified file.
 *
 * Internaly calls vdb_LogVsprintf().
 *
 * \param apStream  file to which to write the log data.
 * \param apcFormat  format of the log message
 * \param aArgs     further arguments
 * 
 * \return  number of characters/bytes printed
 ****************************************************************************/
size_t vdb_LogVfprintf(FILE *apStream, const char *apcFormat, va_list aArgs);
#endif // !defined(__KERNEL__) && !defined(__STANDALONE__)

/****************************************************************************/
/** Does simple formated printing into specified memmory buffer.
 *
 * Internally calls vdb_LogVsprintf().
 *
 * \param apMemAddr memory address to which to print the log
 * \param apcFormat format of the log message
 * \param aArgs further arguments
 * 
 * \return number of characters/bytes printed
 ****************************************************************************/
size_t vdb_LogVmprintf(void *apMemAddr, const char *apcFormat, va_list aArgs);


/****************************************************************************/
/** Does simple formated printing into specified string buffer.
 *
 * Supports %d %u %s %x %X %p formating sequences with '0'/' ' left side padding.
 *
 * \param apBuf
 * \param aSize
 * \param apcFormat
 * \param aArgs
 * 
 * \return number of bytes printed.
 ****************************************************************************/
size_t vdb_LogVsprintf( char       *apBuf,
                        size_t      aSize,
                        const char *apcFormat,
                        va_list     aArgs);

/****************************************************************************/
/** Converts unsigned int to character array
 *
 * \param aNum number to be converted
 * \param aBase the base/radix of the number to be converted
 * \param apBuf string buffer to write to
 * \param aReqLen required length of the string (0 = not specified)
 * \param aMaxLen maximum number of characters allowed to be written
 * \param aPad padding character
 * 
 * \return number of printed characters
 ****************************************************************************/
size_t vdb_LogUi2str(unsigned   aNum,
                      uint32_t  aBase,
                      char     *apBuf,
                      size_t    aReqLen,
                      size_t    aMaxLen,
                      char      aPad);

/****************************************************************************/
/** Converts unsigned int to character array
 *
 * \param aPtr  number to be converted
 * \param aBase the base/radix of the number to be converted
 * \param apBuf string buffer to write to
 * \param aReqLen required length of the string (0 = not specified)
 * \param aMaxLen maximum number of characters allowed to be written
 * \param aPad padding character
 * 
 * \return number of printed characters
 ****************************************************************************/
size_t vdb_LogPtr2str(uintptr_t aPtr,
                      uint32_t  aBase,
                      char     *apBuf,
                      size_t    aReqLen,
                      size_t    aMaxLen,
                      char      aPad);

/****************************************************************************/
/** Converts specified string to unsigned integer
 *
 * \param apBuf pointer to string with number
 * \param arResult resulting number
 * \param arNegative set to true if '-' sign detected
 * 
 * \return number of characters read
 ****************************************************************************/
size_t vdb_LogStr2ui(const char *apBuf, uint32_t *apResult, bool *apNegative);
#ifdef __cplusplus 
} 
#endif 

#endif /* VDBLOG_H */


