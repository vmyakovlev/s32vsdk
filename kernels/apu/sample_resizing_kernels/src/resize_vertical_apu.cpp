/******************************************************************************
 *  (C) Copyright CogniVue Corporation. 2013 All right reserved.
 *  Copyright (c) 2017 NXP
 *  
 *  Confidential Information
 *
 *  All parts of the CogniVue Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from CogniVue.
 *
 *  Created by: Ramin Azarmehr
 *  Based on code from: Daniel Laroche
 ******************************************************************************/
/**
* \file resize_vertical_apu.cpp
* \brief vertical resizing implementation for APEX
* \author CGV
* \version
* \date
****************************************************************************/

//! Define as preprocessor in simulation project for debugging
#ifdef __APU_DEBUG__ 
  #ifndef ACF_KERNEL_IMPLEMENTATION 
    #define ACF_KERNEL_IMPLEMENTATION 
  #endif
#endif

#ifdef ACF_KERNEL_IMPLEMENTATION


#ifdef APEX2_EMULATE
#include "apu_cond.hpp"
#include "apu_config.hpp"
using namespace APEX2;
#endif

#include "resize_vertical_apu.h"


inline void vecsat(vec32s* val, int32_t min, int32_t max)
{
    vif (*val < min)
    {
       *val = min;
    }
    vendif    
    vif (*val > max)
    {
       *val = max;
    }
    vendif
}

void apu_input_format(vec16s* pdst, const vec16s* psrc, 
                      int16s bh, int16s bw, int16s ioff, int16s iscl)
{
  for (int i=0; i < bw; i++)
  {
    for (int j=0; j < bh; j++)
    {
       int idx = j*bw + i;
       vec32s d = (vec32s) (psrc[idx]); 
       d += ioff;
       d <<= iscl;
       vecsat(&d, INT16_MIN, INT16_MAX);
       pdst[idx] = (vec16s) d;
    }
  }
}

void apu_input_format(vec16s* pdst, const vec08u* psrc, 
                      int16s bh, int16s bw, int16s ioff, int16s iscl)
{
  for (int i=0; i < bw; i++)
  {
    for (int j=0; j < bh; j++)
    {
       int idx = j*bw + i;
       vec32s d = (vec32s) (psrc[idx]); 
       d += ioff;
       d <<= iscl;
       vecsat(&d, INT16_MIN, INT16_MAX);
       pdst[idx] = (vec16s) d;
    }
  }
}

void apu_output_format(vec16s* pdst, const vec16s* psrc, 
                      int16s bh, int16s bw, int16s ooff, int16s oscl)
{
  for (int i=0; i < bw; i++)
  {
    for (int j=0; j < bh; j++)
    {
       int idx = j*bw + i;
       vec32s d = (vec32s) (psrc[idx]); 
       d += ooff;
       d >>= oscl;
       vecsat(&d, INT16_MIN, INT16_MAX);
       pdst[idx] = (vec16s) d;
    }
  }
}

void apu_output_format(vec08u* pdst, const vec16s* psrc, 
                      int16s bh, int16s bw, int16s ooff, int16s oscl)
{
  for (int i=0; i < bw; i++)
  {
    for (int j=0; j < bh; j++)
    {
       int idx = j*bw + i;
       vec32s d = (vec32s) (psrc[idx]); 
       d += ooff;
       d >>= oscl;
       vecsat(&d, 0, UINT8_MAX);
       pdst[idx] = (vec08u) d;
    }
  }
}


void apu_average(vec16s* pdst, const vec16s* psrc,
                 int16s bh, int16s bw, int16s asiz, 
                 int16s ascl, int16s arnd) 
{
  for (int i=0; i < bw; i++) 
  {
    for (int j=0, l=0; j < bh; j+=asiz, l++) 
    {
      vec32s d = 0;
      for (int ak=0; ak<asiz; ak++)
         d += ((vec32s)psrc[(j+ak)*bw + i]);
      d  += arnd;
      d >>= ascl;
      vecsat(&d, INT16_MIN, INT16_MAX);
      pdst[l*bw + i] = (vec16s)d;
    }
  }
}

void apu_filter_generic(vec16s* pdst, const vec16s* psrc,
                        int16s bh, int16s bw, const int16s* flt, 
                        int16s fsiz, int16s fscl, int16s frnd)
{
  psrc -= (fsiz >> 1)*bw;
  for (int i=0; i < bw; i++)
  {
    for (int j=0; j < bh; j++)
    {
      vec32s d = 0;
      for (int fk=0; fk<fsiz; ++fk)
         d += ((vec32s)psrc[(j+fk)*bw + i] * (int32_t)flt[fk]);
      d  += frnd;
      d >>= fscl;
      vecsat(&d, INT16_MIN, INT16_MAX);
      pdst[j*bw + i] = (vec16s)d;
    }
  }
}

// IS this method even used any more?
// If so, review why X is the outer loop.  This causes problems with rsz_desc->src_offs
// See vec08u implementation below.
#if 0
void apu_resize_polyphase(vec16s* pdst, const vec16s* psrc, RESIZE_DESCRIPTOR* rsz_desc)
{
  const int16s dscl = RSZ_DECIMAL_SCL;
  const int16s mask = (1 << dscl) - 1; //! Mask to get fraction
  const int16s frac = rsz_desc->src_offs & mask; //! Accumulated fraction
  for (int16s sx = 0, src_end = (rsz_desc->dst_bh - 1) * rsz_desc->scl_fact + frac; 
       sx < rsz_desc->dst_bw; sx++)
  {
    for (int16s sy = frac, dy = 0; 
         sy <= src_end && dy < rsz_desc->dst_size - rsz_desc->dst_offs; 
         sy += rsz_desc->scl_fact, rsz_desc->src_offs += rsz_desc->scl_fact, dy++)
    {
      const vec16s*  s  = psrc + ((sy >> dscl) * rsz_desc->dst_bw + sx); //! Source index
      const int16s* flt= rsz_desc->fbnk + (rsz_desc->taps *  //! Filter bank selection
                        ((rsz_desc->src_offs & mask) >> (dscl - rsz_desc->phases))); //! Filter Phase
      vec32s d = 0; //! FIR accumulator
      for (int16s fk=0; fk < rsz_desc->taps; ++fk)
         d += ((vec32s)s[fk*rsz_desc->dst_bw] * (int32_t)flt[fk]);  //! 32 -> 16 bit ?
      d  += rsz_desc->out_round; //! Rounding
      d >>= rsz_desc->out_scale; //! Scale down
      vecsat(&d, INT16_MIN, INT16_MAX);     
      pdst[dy * rsz_desc->dst_bw + sx] = (vec16s)d;
    }
  }
}
#endif

//vec08u version of the above
//void apu_resize_polyphase(vec08u* pdst, const vec08u* psrc, RESIZE_DESCRIPTOR* rsz_desc)
void apu_resize_polyphase(vec08u* pdst, const vec08u* psrc, int32s sh, RESIZE_DESCRIPTOR* rsz_desc)
{
  const int32s dscl = RSZ_DECIMAL_SCL;
  const int32s mask = (1 << dscl) - 1; //! Mask to get fraction
//  const int32s frac = rsz_desc->src_offs & mask; //! Accumulated fraction
  
  // adjustment top row for custom resize, taking into account north spatial dependency
  int32s north_dep = ((rsz_desc->taps-1)/2);

  // src_offs, src_current_line are fractional and integer source location values
  // which are static, and updated pass-to-pass of the kernel.
//  for (int32s sy = frac, dy = 0; 
//  for (int32s sy = (rsz_desc->src_offs - (rsz_desc->dst_offs << dscl)) , dy = 0; 
  for (int32s sy = (rsz_desc->src_offs - (rsz_desc->src_current_line << dscl)) , dy = 0; 
         dy < rsz_desc->dst_bh;
         sy += rsz_desc->scl_fact, rsz_desc->src_offs += rsz_desc->scl_fact, dy++)
  {
    for (int32s sx = 0 /*, src_end = (rsz_desc->dst_bh - 1) * rsz_desc->scl_fact + frac*/; 
       sx < rsz_desc->dst_bw; sx++)
    {
      const vec08u*  s  = psrc + (((sy >> dscl) - north_dep) * rsz_desc->dst_bw + sx); //! Source index
      const int16s* flt= rsz_desc->fbnk + (rsz_desc->taps *  //! Filter bank selection
                        ((rsz_desc->src_offs & mask) >> (dscl - rsz_desc->phases))); //! Filter Phase
      vec32s d = 0; //! FIR accumulator
      for (int16s fk=0; fk < rsz_desc->taps; ++fk)
         d += ((vec32s)s[fk*rsz_desc->dst_bw] * (int32_t)flt[fk]);  //! 32 -> 16 bit ?
      d  += rsz_desc->out_round; //! Rounding
      d >>= rsz_desc->out_scale; //! Scale down
      vecsat(&d, 0, UINT8_MAX);     
      pdst[dy * rsz_desc->dst_bw + sx] = (vec08u)d;
    }
  }
  
  // coopting the unused dst_offs.  todo: Revisit
//  rsz_desc->dst_offs += sh;
  rsz_desc->src_current_line += sh;
  
}



void apu_resize_integer(vec08u* pdst, const vec08u* psrc, RESIZE_DESCRIPTOR* rsz_desc)
{
  const int32s dscl = RSZ_DECIMAL_SCL;
  int32_t north_dep = ((rsz_desc->taps-1)/2);
  for (int32s sx = 0, src_end = (rsz_desc->dst_bh - 1) * rsz_desc->scl_fact; 
       sx < rsz_desc->dst_bw; sx++)
  {
    for (int32s sy = 0, dy = 0; 
         sy <= src_end 
#ifdef __APU_DEBUG__         
         && dy < rsz_desc->dst_size - rsz_desc->dst_offs 
#endif         
         ;sy += rsz_desc->scl_fact, dy++)
    {
      const vec08u*  s  = psrc + (((sy >> dscl) - north_dep) * rsz_desc->dst_bw + sx); //! Source index
      vec32s d = 0; //! FIR accumulator
      for (int16s fk=0; fk < rsz_desc->taps; ++fk)
         d += ((vec32s)s[fk*rsz_desc->dst_bw] * (int32_t)rsz_desc->fbnk[fk]);  //! 32 -> 16 bit ?
      d  += rsz_desc->out_round; //! Rounding
      d >>= rsz_desc->out_scale; //! Scale down
      vecsat(&d, 0, UINT8_MAX);     
      pdst[dy * rsz_desc->dst_bw + sx] = (vec08u)d;
    }
  }
}

const int16s flt_down_sampling1[FLT_BANK_SIZE] = FLT_DOWN_SAMPLING1;
const int16s flt_up_sampling   [FLT_BANK_SIZE] = FLT_UP_SAMPLING;
const int16s flt_down_sampling2[FLT_BANK_SIZE] = FLT_DOWN_SAMPLING2;
int get_polyphase_params(int16s ssiz, int16s dsiz, RESIZE_DESCRIPTOR* rsz_desc)
{
  const int16s dscl = RSZ_DECIMAL_SCL;
  //! Polyphase filter banks

  int rmod = -1;
  if (ssiz >= 2*dsiz)
  {
     rmod  = 2;
     rsz_desc->phases= 2; //! 4 Phases
     rsz_desc->taps  = 8; 
     rsz_desc->fbnk  = flt_down_sampling2;
  }
  else if (ssiz > dsiz)
  {
     rmod  = 1;
     rsz_desc->phases= 3; //! 8 Phases
     rsz_desc->taps  = 4;
     rsz_desc->fbnk  = flt_down_sampling1;
  }
  else if  (ssiz < dsiz)
  {
     rmod  = 0;
     rsz_desc->phases= 3; //! 8 Phases
     rsz_desc->taps  = 4;
     rsz_desc->fbnk  = flt_up_sampling;
  }
  if (rmod<0) return rmod;
//  rsz_desc->dst_offs  = 0;
//  rsz_desc->dst_size  = dsiz;
  rsz_desc->src_current_line  = 0;
  rsz_desc->scl_fact  = ((ssiz /*- 1*/) * (1 << dscl)) / dsiz; // Discuss with Daniel
//  rsz_desc->src_offs  = (rsz_desc->scl_fact >> 1) - (((rsz_desc->taps - 1) >> 1) << dscl);
  rsz_desc->src_offs  = (rsz_desc->scl_fact >> 1);
  rsz_desc->out_scale = 8;
  rsz_desc->out_round = 1 << (rsz_desc->out_scale - 1);
  return rmod;
}








#endif //ACF_KERNEL_IMPLEMENTATION
