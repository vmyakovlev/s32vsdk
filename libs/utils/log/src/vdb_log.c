/*
 * Copyright (c) 2015 Freescale Semiconductor
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
 * \file      vdb_log.c
 * \brief     Function definitions for vision debug logging
 * \author    Tomas Babinec
 * \version   0.1
 * \date      Aug 29, 2013
 ****************************************************************************/

#ifndef __KERNEL__

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#endif //ifndef __KERNEL__

#include "vdb_log.h"

/*****************************************************************************
 * static global variables
 *****************************************************************************/

#if !defined(__KERNEL__) && !defined(__STANDALONE__)
// log file
static FILE *spLogFile = NULL;      ///< log file

#endif //!defined(__KERNEL__) && !defined(__STANDALONE__)

//log memory + utilities
static void *spLogMemAddr = NULL;   ///< log memory address
static size_t sLogMemSize = 0;      ///< log memory size
static size_t sLogMemUsed = 0;      ///< log memory used
static void *spLogMemPtr = NULL;    ///< log memory pointer to actual position

/*****************************************************************************
 * exported functions
 *****************************************************************************/

int32_t vdb_LogInit()
{
#if !defined(__KERNEL__) && !defined(__STANDALONE__)
  return vdb_LogInitStr(stdout);
#else //__KERNLE__
  return 0;
#endif //__KERNLE__
} //vdb_LogInit()

//*****************************************************************************

#if !defined(__KERNEL__) && !defined(__STANDALONE__)
int32_t vdb_LogInitStr(FILE *apStream)
{
  int32_t ret = 0;
  // if spStream == NULL, logging will stop
  spLogMemAddr = NULL;
  spLogMemPtr = NULL;
  spLogFile = apStream;
  if(!apStream)
  {
    // let user know, the stream is NULL
    ret = 1;
  }
  return ret;
} // vdb_LogInitStr(FILE*)
#endif // !defined(__KERNEL__) && !defined(__STANDALONE__)

//*****************************************************************************

int32_t vdb_LogInitMem(void *apMemAddr, size_t aSize)
{
  int32_t ret = 0;
  spLogMemAddr = apMemAddr;
  spLogMemPtr = apMemAddr;
  sLogMemSize = aSize;
  sLogMemUsed = 0;
#if !defined(__KERNEL__) && !defined(__STANDALONE__)
  spLogFile = NULL;
#endif // !defined(__KERNEL__) && !defined(__STANDALONE__)
  
  if ((aSize < VDB_LOG_BUF_MAX_LEN) || !apMemAddr)
  {
    // let the user know the mem buffer is bad
    ret = 1;
  } // if
  return ret;
} // vdb_LogInitMem(void*, size_t)

//*****************************************************************************

void vdb_LogStop()
{
  spLogMemAddr = NULL;
  spLogMemPtr = NULL;
  sLogMemSize = 0;
  sLogMemUsed = 0;
#if !defined(__KERNEL__) && !defined(__STANDALONE__)
  spLogFile = NULL;
#endif // !defined(__KERNEL__) && !defined(__STANDALONE__)
} // vdb_LogStop()

//*****************************************************************************

size_t vdb_LogUsedMemGet()
{
  return sLogMemUsed;
} // vdb_LogUsedMemGet()

//*****************************************************************************

size_t vdb_Log(const char *apcFormat, ...)
{
  // return variable = number of chars printed (default = 0)
  int32_t ret = 0;
#if !defined(__KERNEL__) && !defined(__STANDALONE__)
  bool pointers = !spLogFile && !spLogMemAddr;
#else
  bool pointers = false;
#endif // !defined(__KERNEL__) && !defined(__STANDALONE__)
  if (pointers)
  {
    ret = -1;
  }else
  {
    // choose print version at compile time
    va_list args;
    va_start(args, apcFormat);
#if defined(__KERNEL__) && !defined(__STANDALONE__)
    if(spLogMemPtr)
    {
      ret = vdb_LogVmprintf(spLogMemPtr, apcFormat, args);
      // shift memory buffer pointer
      spLogMemPtr = (void*)(((char*) spLogMemPtr) + ret);
    } // if(spLogMemAddr)
    else
    {
      ret = vprintk(apcFormat, args);
    } // else from if(spLogMemAddr)
    // todo: maybe add something for integrity
#elif defined(__STANDALONE__)
    ret = printf(apcFormat, args);
#elif defined(ARM)
    // do very simple printing
    if(spLogFile)
    {
      ret = vdb_LogVfprintf(spLogFile,apcFormat, args);
    } // if(spLogFile)

    if(spLogMemPtr)
    {
      ret = vdb_LogVmprintf(spLogMemPtr,apcFormat, args);
      // shift memory buffer pointer
      spLogMemPtr = (void*)(((char*) spLogMemPtr) + ret);
    } // if(spLogMemAddr)

#else
    if(spLogFile)
    {
      ret = vdb_LogVfprintf(spLogFile,apcFormat, args);
    } // if(spLogFile)

    if(spLogMemPtr)
    {
      ret = vdb_LogVmprintf(spLogMemPtr,apcFormat, args);
      // shift memory buffer pointer
      spLogMemPtr = (void*)(((char*) spLogMemPtr) + ret);
    } // if(spLogMemAddr)

    // use normal print utility on Linux
    //ret = vfprintf(spLogFile, apcFormat, args);
#endif
    va_end(args);
  }
  return ret;
} // vdb_Log

//*****************************************************************************

#if !defined(__KERNEL__) && !defined(__STANDALONE__)

size_t vdb_LogVfprintf(FILE *apStream, const char *apcFormat, va_list aArgs)
{
  char buf[VDB_LOG_BUF_MAX_LEN];
  int32_t byte_cnt;

  byte_cnt = vdb_LogVsprintf(buf, (int32_t) VDB_LOG_BUF_MAX_LEN, apcFormat, aArgs);

  // ignore ret value
  (void)fwrite(buf, sizeof(char), (size_t) byte_cnt, apStream);
  return byte_cnt;
} // vdb_LogVfprintf()
#endif // !defined(__KERNEL__) && !defined(__STANDALONE__)

//*****************************************************************************

size_t vdb_LogVmprintf(void *apMemAddr, const char *apcFormat, va_list aArgs)
{
  char buf[VDB_LOG_BUF_MAX_LEN];
  int32_t byte_cnt;

  byte_cnt = vdb_LogVsprintf(buf, (int32_t) VDB_LOG_BUF_MAX_LEN, apcFormat, aArgs);
  // check for memory buffer overflow
  sLogMemUsed += byte_cnt;
  if(sLogMemUsed > sLogMemSize)
  {
    byte_cnt -= sLogMemUsed - sLogMemSize;
    sLogMemUsed = sLogMemSize;
  } // if mem buffer overflow

  (void)memcpy(apMemAddr, buf, byte_cnt);
  return byte_cnt;
} // vdb_LogVmprintf()

//*****************************************************************************

size_t vdb_LogVsprintf(char *apBuf, size_t aSize, const char *apcFormat, va_list aArgs)
{
  uint32_t byte_cnt = 0;

  char c = *apcFormat;
  while ((c != '\0') && (byte_cnt < aSize))
  {
    // do the apcFormat string parsing
    if (c != '%')
    {
      apBuf[byte_cnt] = c;
    } else
    {
      char pad = ' ';
      uint32_t pad_num = 0;
      uint32_t cnt;
      bool neg = false;
      c = *(++apcFormat);
      if ((c >= '0') && (c <= '9'))
      {
        // c is a number > probably padding required
        if (c == '0')
        {
          pad = '0';
        } else
        {
          pad = ' ';
          // return one character
          --apcFormat;
        } // if c == '0'
          // get the required number of characters
        cnt = vdb_LogStr2ui(++apcFormat, &pad_num, &neg);
        // shift buffer pointer
        apcFormat += cnt;
        c = *apcFormat;
      } // if c is a number

      switch (c)
      {
        // % symbol
        case '%':
        {
          apBuf[byte_cnt] = c;
        } // case '%'
          break;
          // signed int
        case 'd':
        {
          int32_t num = va_arg(aArgs, int32_t);
          if (num < 0)
          {
            apBuf[byte_cnt] = '-';
            num *= -1;
            ++byte_cnt;

            // check if buffer size ok
            if (byte_cnt >= aSize)
            {
              break;
            }

            // set padding to one less
            if (pad_num > 0)
            {
              --pad_num;
            }
          } // if
          byte_cnt += vdb_LogUi2str((unsigned)num,
                                    10,
                                    &(apBuf[byte_cnt]),
                                    pad_num,
                                    (size_t)(aSize - (byte_cnt + 1)),
                                    pad) - 1;
        } // case 'd'
          break;
          // unsigned int
        case 'u':
        {
          byte_cnt += vdb_LogUi2str(va_arg(aArgs, unsigned),
                                    10,
                                    &(apBuf[byte_cnt]),
                                    pad_num,
                                    (size_t)(aSize - (byte_cnt + 1)),
                                    pad) - 1;
        } // case 'u'
          break;
          // hexa
        case 'X':
        case 'x':
        {
          byte_cnt += vdb_LogUi2str(va_arg(aArgs, unsigned),
                                    16,
                                    &(apBuf[byte_cnt]),
                                    pad_num,
                                    (size_t)(aSize - (byte_cnt + 1)),
                                    pad) - 1;
        } // case 'x'/'X'
          break;
          // pointer
        case 'p':
        {
          byte_cnt += vdb_LogPtr2str((uintptr_t)va_arg(aArgs, void*),
                                    16,
                                    &(apBuf[byte_cnt]),
                                    pad_num,
                                    (size_t)(aSize - (byte_cnt + 1)),
                                    pad) - 1;
        } // case 'x'/'X'
          break;
          // string
        case 's':
        {
          // copy characters from string
          const char *str = va_arg(aArgs, char*);
          while ((*str != 0) && (byte_cnt < aSize))
          {
            apBuf[byte_cnt] = *str;
            ++str;
            ++byte_cnt;
          } // while str printing
          --byte_cnt;
        } // case 's'
          break;
        default:
        {
          // print it out as nothing happend
          apBuf[byte_cnt] = '%';
          ++byte_cnt;
          apBuf[byte_cnt] = c;
        } // default
          break;
      } // switch
    } // if (c != '%')
    c = *(++apcFormat);
    ++byte_cnt;
  } // while ((c != '\0') && (byte_cnt < aSize))

  return byte_cnt;
} // vdb_LogVsprintf()

//*****************************************************************************

size_t vdb_LogUi2str(unsigned aNum,
                      uint32_t aBase,
                      char *apcBuf,
                      size_t aReqLen,
                      size_t aMaxLen,
                      char aPad)
{
  uint32_t d = 1;
  uint32_t cnt = 1;
  uint32_t dgt = 0;

  // ceil num of digits
  if (aReqLen > aMaxLen)
  {
    aReqLen = aMaxLen;
  } // if

  // count digits
  while ((aNum / d) >= aBase)
  {
    d *= aBase;
    ++cnt;
  } // while

  // apply padding if any
  if (aReqLen > cnt)
  {
    uint32_t pad_len = aReqLen - cnt;
    for (cnt = 0; cnt < pad_len; ++cnt)
    {
      apcBuf[cnt] = aPad;
    } // for - padding
  } else
  {
    cnt = 0;
  } //else from if padding needed

  // convert digits to characters
  while ((d != 0) && (cnt < aMaxLen))
  {
    dgt = aNum / d;
    aNum %= d;
    d /= aBase;
    if (dgt < 10)
    {
      apcBuf[cnt] = (int8_t)('0' + dgt);
    } else
    {
      apcBuf[cnt] = (int8_t)('a' + dgt - 10);
    } // if (dgt < 10)
    ++cnt;
  } // while
  return cnt;
} //vdb_LogUi2str()

//*****************************************************************************

size_t vdb_LogPtr2str(uintptr_t aPtr,
                      uint32_t aBase,
                      char *apcBuf,
                      size_t aReqLen,
                      size_t aMaxLen,
                      char aPad)
{
  uint32_t lD = 1;
  uint32_t lCnt = 1;
  uint32_t lDgt = 0;

  // ceil num of digits
  if (aReqLen > aMaxLen)
  {
    aReqLen = aMaxLen;
  } // if

  // count digits
  while ((aPtr / lD) >= aBase)
  {
    lD *= aBase;
    ++lCnt;
  } // while

  // apply padding if any
  if (aReqLen > lCnt)
  {
    uint32_t lPadLen = aReqLen - lCnt;
    for (lCnt = 0; lCnt < lPadLen; ++lCnt)
    {
      apcBuf[lCnt] = aPad;
    } // for - padding
  } else
  {
    lCnt = 0;
  } //else from if padding needed

  // convert digits to characters
  while ((lD != 0) && (lCnt < aMaxLen))
  {
    lDgt = aPtr / lD;
    aPtr %= lD;
    lD /= aBase;
    if (lDgt < 10)
    {
      apcBuf[lCnt] = (int8_t)('0' + lDgt);
    } else
    {
      apcBuf[lCnt] = (int8_t)('a' + lDgt - 10);
    } // if (dgt < 10)
    ++lCnt;
  } // while
  return lCnt;
} //vdb_LogPtr2str()

//*****************************************************************************

size_t vdb_LogStr2ui(const char *apcBuf, uint32_t *apResult, bool *apNegative)
{
  uint32_t base = 10;
  uint32_t result = 0;
  uint32_t cnt = 0;
  char c;
  
  if(!apcBuf || !apResult || !apNegative)
  {
    cnt = (uint32_t)-1;
  }
  else
  {
    c = *(apcBuf);

    if (c == '-')
    {
      *apNegative = true;
      c = *(++apcBuf);
      ++cnt;
    } else
    {
      *apNegative = false;
    }

    while ((c >= '0') && (c <= '9'))
    {
      result = result * base + c - '0';
      c = *(++apcBuf);
      ++cnt;
    }
    *apResult = result;
  }
  return cnt;
} // vdb_LogStr2ui()

//*****************************************************************************

/*EOF*/
