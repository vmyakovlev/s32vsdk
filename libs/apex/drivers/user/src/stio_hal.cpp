/******************************************************************************
 *  (C) Copyright CogniVue Corporation. 2011 All right reserved.
 *
 *  Confidential Information
 *
 *  All parts of the CogniVue Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from CogniVue.
 ******************************************************************************/
#include <stio_hal.h>
#include <hw_reg_user.h>

typedef unsigned int REG32;
#include "apex_642cl_stream_dma_map.h"

void stio_hal_interruptclear_sti_done(int apexID, int channel)
{
   volatile APEX_642CL_STREAM_DMA_DCR *const stio_reg = (volatile APEX_642CL_STREAM_DMA_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_STIO);
   switch (channel)
   {
      case 0: stio_reg->Icr.as32BitValue = (1 << 8 ); break; //Icr.asBitField.In0
      case 1: stio_reg->Icr.as32BitValue = (1 << 9 ); break; //Icr.asBitField.In1
      case 2: stio_reg->Icr.as32BitValue = (1 << 10); break; //Icr.asBitField.In2
      case 3: stio_reg->Icr.as32BitValue = (1 << 11); break; //Icr.asBitField.In3
   }
}

void stio_hal_interruptclear_sto_done(int apexID, int channel)
{
   volatile APEX_642CL_STREAM_DMA_DCR *const stio_reg = (volatile APEX_642CL_STREAM_DMA_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_STIO);
   switch (channel)
   {
      case 0: stio_reg->Icr.as32BitValue = (1 << 0); break; //Icr.asBitField.Out0
      case 1: stio_reg->Icr.as32BitValue = (1 << 1); break; //Icr.asBitField.Out1
   }
}
