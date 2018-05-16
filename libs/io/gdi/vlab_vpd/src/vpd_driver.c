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
* \brief VLAB Virtual probe display driver
* \author Tomas Babinec
* \version 0.1
* \date 02-July-2014
****************************************************************************/

#include <bsp.h>
#include <INTEGRITY.h>
#include <memoryspace.h>
#include <buildmemtable.h>
#include "vpd_driver.h"

void KernelMemMap(ExtendedAddress ExtendedKernelAddress,
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
} // KernelMemMap()

/*======================================================================================*/
/* Memory spaces for VLAB VPD registers and data					*/
MemoryReservation vpd_regs_memspace;
MemoryReservation vpd_data_memspace;

/*======================================================================================*/
/* Init function for OAL								*/
void vpddriver_iodevice_init(void)
{
  // map VLAB VPD register region
  ExtendedAddress	addr;
  KernelMemMap(VPD_REGS_PHYSICAL_POOL_BASE, VPD_REGS_PHYSICAL_POOL_BASE, VPD_REGS_PHYSICAL_POOL_SIZE, MEMORY_RW | MEMORY_VOLATILE /*| MEMORY_ARM_STRONGLY_ORDERED*/, Other_MemoryType);
  
  vpd_regs_memspace.Attributes = MEMORY_RW | MEMORY_VOLATILE /*| MEMORY_ARM_STRONGLY_ORDERED*/;
  vpd_regs_memspace.PhysicalSpaceNumber = 0;
  vpd_regs_memspace.First = VPD_REGS_PHYSICAL_POOL_BASE;
  vpd_regs_memspace.Last = VPD_REGS_PHYSICAL_POOL_BASE+VPD_REGS_PHYSICAL_POOL_SIZE-1;
  vpd_regs_memspace.Type = Other_MemoryType;
  vpd_regs_memspace.Fixed = true;
  vpd_regs_memspace.FirstMask = BMT_MASK(1);
  vpd_regs_memspace.LastMask = BMT_MASK(1);
  vpd_regs_memspace.name = VPD_REGS_RESOURCE_NAME;

  BMT_AllocateFromAnonymousMemoryReservation(&vpd_regs_memspace, &addr);	
  consolestring("VPD registers memory ready.\n");

  // map VLAB VPD data region
  KernelMemMap(VPD_DATA_PHYSICAL_POOL_BASE, VPD_DATA_PHYSICAL_POOL_BASE, VPD_DATA_PHYSICAL_POOL_SIZE, MEMORY_RW | MEMORY_VOLATILE /*| MEMORY_ARM_STRONGLY_ORDERED*/, Other_MemoryType);
  
  vpd_data_memspace.Attributes = MEMORY_RW | MEMORY_VOLATILE /*| MEMORY_ARM_STRONGLY_ORDERED*/;
  vpd_data_memspace.PhysicalSpaceNumber = 0;
  vpd_data_memspace.First = VPD_DATA_PHYSICAL_POOL_BASE;
  vpd_data_memspace.Last = VPD_DATA_PHYSICAL_POOL_BASE+VPD_DATA_PHYSICAL_POOL_SIZE-1;
  vpd_data_memspace.Type = Other_MemoryType;
  vpd_data_memspace.Fixed = true;
  vpd_data_memspace.FirstMask = BMT_MASK(1);
  vpd_data_memspace.LastMask = BMT_MASK(1);
  vpd_data_memspace.name = VPD_DATA_RESOURCE_NAME;

  BMT_AllocateFromAnonymousMemoryReservation(&vpd_data_memspace, &addr);	
  consolestring("VPD data memory ready.\n");
}
 
typedef void (*voidfunc)();
voidfunc __ghsentry_bspuserinit_vpd_regmap = vpddriver_iodevice_init;

#pragma ghs alias __ghsautoimport_vpd_regmap __ghsentry_bspuserinit_vpd_regmap


