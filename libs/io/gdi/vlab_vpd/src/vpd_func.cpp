/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/**
* \file gdi_srv.cpp
* \brief VLAB Virtual probe display enablement functions
* \author Tomas Babinec
* \version 0.1
* \date 02-July-2014
****************************************************************************/

#include <INTEGRITY.h>
#include "oal.h"
#include "vpd_driver.h" // has to be before vpd.h (because of vpd_regs_t)                  
#include "vpd.h"

static MemoryRegion gsPhysicalVpdRegsRegion = NULLMemoryRegion;
static MemoryRegion gsPhysicalVpdDataRegion = NULLMemoryRegion;

// maps VPD registers
vpd_regs_t * VPD_RegsMap(void)
{
  Error lError, lErrorMap;
  MemoryRegion virtMemReg;
  Address virtAddr = 0;
  Value virtLength = 0;
  Address physFirst = 0;
  Address physLast = 0;
  vpd_regs_t *pRet = NULL;

  lError = RequestResource((Object *)&gsPhysicalVpdRegsRegion, VPD_REGS_RESOURCE_NAME, "!systempassword");
  
  if (Success == lError)
  {
      // Get physical addresses of the physical memory region
      GetMemoryRegionAddresses(gsPhysicalVpdRegsRegion, &physFirst, &physLast);

      // map directly to the physical memory region
      lErrorMap = MapMemoryWithAttributes(__ghs_VirtualMemoryRegionPool, gsPhysicalVpdRegsRegion, &virtMemReg, &virtAddr, &virtLength, 0, MEMORY_READ|MEMORY_WRITE|MEMORY_VOLATILE );

      if (Success == lErrorMap)
      {
	 printf("VPD register mapping succeeded. Region size %x.\n", virtLength);
	 pRet = (vpd_regs_t*)virtAddr;
      } // if MapMemory succeeded
      else
      {
	  printf("VPD register mapping failed (%d)\n", lErrorMap);
      } // else from if AllocateAnyMemoryRegion succeeded
  } // if Resource request succeeded
  else
  {
      printf("VPD registers resource request failed (%d)\n", lError);
  } // else from if Resource request succeeded

  return pRet;
} // VPD_RegsMap()

// maps VPD registers
uint8_t * VPD_DataMap(void)
{
  Error lError, lErrorMap;
  MemoryRegion virtMemReg;
  Address virtAddr = 0;
  Value virtLength = 0;
  Address physFirst = 0;
  Address physLast = 0;
  uint8_t *pRet = NULL;

  lError = RequestResource((Object *)&gsPhysicalVpdDataRegion, VPD_DATA_RESOURCE_NAME, "!systempassword");
  
  if (Success == lError)
  {
      // Get physical addresses of the physical memory region
      GetMemoryRegionAddresses(gsPhysicalVpdDataRegion, &physFirst, &physLast);

      // map directly to the physical memory region
      lErrorMap = MapMemoryWithAttributes(__ghs_VirtualMemoryRegionPool, gsPhysicalVpdDataRegion, &virtMemReg, &virtAddr, &virtLength, 0, MEMORY_READ|MEMORY_WRITE|MEMORY_VOLATILE );

      if (Success == lErrorMap)
      {
	 printf("VPD data mapping succeeded. Region size %x.\n", virtLength);
	 pRet = (uint8_t*)virtAddr;
      } // if MapMemory succeeded
      else
      {
	  printf("VPD data mapping failed (%d)\n", lErrorMap);
      } // else from if AllocateAnyMemoryRegion succeeded
  } // if Resource request succeeded
  else
  {
      printf("VPD data resource request failed (%d)\n", lError);
  } // else from if Resource request succeeded

  return pRet;
} // VPD_RegsMap()
