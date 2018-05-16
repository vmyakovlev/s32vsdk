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
#include <bsp.h>
#include <INTEGRITY.h>
#include <memoryspace.h>
#include <buildmemtable.h>
#include <oal.h>

#ifndef __S32V234
#ifndef VLAB
#include <modules/ghs/bspsrc/support/atags.h>
#else // ifndef VLAB
void ATAGS_KernelMap(ExtendedAddress ExtendedKernelAddress,
	uint32_t PhysicalAddress, Address Length, Value Attributes,
	MemoryType Type)
  {
    PhysicalRange pr;
    pr.Attributes = Attributes;
    pr.First = ExtendedKernelAddress;
    pr.Last = ExtendedKernelAddress + Length - 1;
    pr.Type = Type;
    if (IsKernelAddress(ExtendedKernelAddress)) {
	ASP_KernelMap((Address)ExtendedKernelAddress, PhysicalAddress, Length,
		Attributes);
    }
    
    CheckSuccess(BMT_RegisterAdditionalPhysicalRange(&pr ));
    MemorySpace_RegisterAnonymousMemoryWindow(
	    &ExtendedKernelMemorySpace, ExtendedKernelAddress,
	    &PhysicalMemorySpace, PhysicalAddress,
	    Length, 0);
  }
#endif // else from ifndef VLAB
#else
#include <devtree_bus_utils.h>
#endif // __S32V234

/*======================================================================================*/
/* Memory space for OAL									*/
#ifndef __S32V234
static MemoryReservation oal_memspace;
#else
static MemorySpace OAL_MemorySpace;
#endif

#ifdef __S32V234
/*======================================================================================*/
/* IODevice OAL memory translation							*/
struct IODeviceVectorStruct OAL_IodeviceVector;
static Address oal_BaseKernAddress;
static ExtendedAddress oal_TransAddr;

/*======================================================================================*/
/* Write register IODevice function							*/
static Error oaldriver_writeregister(IODeviceVector io, Value id, Value val)
{
  Error RetVal = Success;

  switch (id)
  {
    case OAL_IODEVICE_GET_PHYSICAL:
      {
	oal_TransAddr = val;
	if ((oal_TransAddr < oal_BaseKernAddress) || (oal_TransAddr > (oal_BaseKernAddress + OAL_DDR_PHYSICAL_POOL_SIZE - 1))) {
	    oal_TransAddr = 0;
	    RetVal = IllegalAddress;
	}
	break;
      }
    default:
      RetVal = IllegalRegisterNumber;
  }
  return RetVal;
}

/*======================================================================================*/
/* Read register IODevice function							*/
static Error oaldriver_readregister(IODeviceVector io, Value id, Value *val)
{
  Error RetVal = Success;

  switch (id)
  {
    case OAL_IODEVICE_GET_PHYSICAL:
      {
	MemorySpaceAddress MemSpaceAddr;
	ExtendedAddress Len;
	Value Flags;

	RetVal = MemorySpace_GetDmaAddress(OAL_MemorySpace, oal_TransAddr,
		&MemSpaceAddr, &Len, &Flags);
	if (RetVal == Success) {
	  *val = MemSpaceAddr;
	}
	break;
      }
    default:
      RetVal = IllegalRegisterNumber;
  }
  return RetVal;
}
#endif // __S32V234

/*======================================================================================*/
/* Init function for OAL								*/
void oaldriver_iodevice_init(void)
{
#ifndef __S32V234
  ExtendedAddress	addr;

#ifndef VLAB  
  ATAGS_KernelMap(OAL_DDR_PHYSICAL_POOL_BASE, OAL_DDR_PHYSICAL_POOL_BASE, OAL_DDR_PHYSICAL_POOL_SIZE, MEMORY_RW | MEMORY_VOLATILE | MEMORY_ARM_STRONGLY_ORDERED, Other_MemoryType);
  
  oal_memspace.Attributes = MEMORY_RW | MEMORY_VOLATILE | MEMORY_ARM_STRONGLY_ORDERED;
#else  // ifndef VLAB
  ATAGS_KernelMap(OAL_DDR_PHYSICAL_POOL_BASE, OAL_DDR_PHYSICAL_POOL_BASE, OAL_DDR_PHYSICAL_POOL_SIZE, MEMORY_RW | MEMORY_VOLATILE, Other_MemoryType);
  
  oal_memspace.Attributes = MEMORY_RW | MEMORY_VOLATILE;
#endif // else from ifndef VLAB
  oal_memspace.PhysicalSpaceNumber = 0;
  oal_memspace.First = OAL_DDR_PHYSICAL_POOL_BASE;
  oal_memspace.Last = OAL_DDR_PHYSICAL_POOL_BASE+OAL_DDR_PHYSICAL_POOL_SIZE-1;
  oal_memspace.Type = Other_MemoryType;
  oal_memspace.Fixed = true;
  oal_memspace.FirstMask = BMT_MASK(1);
  oal_memspace.LastMask = BMT_MASK(1);
  oal_memspace.name = "oal_memory";

  BMT_AllocateFromAnonymousMemoryReservation(&oal_memspace, &addr);

#else // __S32V234

    // Create a contiguous memory region suitable for DMA use
    MemorySpaceAddress MemSpaceAddr;
    CacheCoherencyMode CoherencyMode;

    CheckSuccess(DevTree_Node_GetMemorySpaces(
	    DevTree_GetRootNode(DevTree_GetTree()), NULL, 0,
	    NULL, NULL, NULL, &OAL_MemorySpace));

    CheckSuccess(MemorySpace_AllocateDmaMemory(OAL_MemorySpace,
	    OAL_DDR_PHYSICAL_POOL_SIZE, ASP_PAGESIZE, MEMORY_RW,
	    WriteCacheCoherency, OAL_DDR_RESOURCE_NAME,
	    &MemSpaceAddr, &oal_BaseKernAddress, &CoherencyMode));
    Assert(CoherencyMode == WriteCacheCoherency);

  // IODevice functions
  OAL_IodeviceVector.WriteRegister = oaldriver_writeregister;
  OAL_IodeviceVector.ReadRegister = oaldriver_readregister;

  RegisterIODeviceVector(&OAL_IodeviceVector, OAL_IODEVICE_NAME);
#endif // __S32V234


  consolestring("OAL Memory ready.\n");
}
 
typedef void (*voidfunc)();
voidfunc __ghsentry_bspuserinit_oal_regmap = oaldriver_iodevice_init;

#pragma ghs alias __ghsautoimport_oal_regmap __ghsentry_bspuserinit_oal_regmap


