/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/*!
* \file match_descriptors_apu.h
* \brief binary descriptor matching
* \addtogroup bdm
* @{
****************************************************************************/

#ifndef APUMATCHDESCRIPTORSIMPL_H
#define APUMATCHDESCRIPTORSIMPL_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif

/****************************************************************************/
/*!
   Matches binary descriptors from group A to binary descriptors from group B.
   Matches with hamming distance greater than provided threshold are rejected.
   Ambiguous matches (i.e. the next best match's hamming distance is lesser
   than the provided check range) are rejected.
   
   distance(desc0, desc1) = popcount(desc0 xor desc1)
   
   After a descriptor desc0 (from group A) has been matched with descriptor desc1 (group B), both desc0 and desc1 are removed from further processing.\n
   dist(desc0, desc1) has to be <= threshold, otherwise the descriptors won't be matched.\n
   If dist(desc0, desc1A) is the smallest distance for desc0 and dist(desc0, desc1B) is the second smallest distance for desc0, the difference of these distances has to be > range check, otherwise desc0 won't be matched to anything
   \brief Matches binary descriptors.

   \param apcDescriptors0   - [Input] First input descriptor array
   \param aDescriptor0Count - [Input] First input descriptor array size
   \param apcDescriptors1   - [Input] Second input descriptor array
   \param aDescriptor1Count - [Input] Second input descriptor array size
   \param apMatches0        - [Output] First elements of match pairs
   \param apMatches1        - [Output] Second elements of match pairs
   \param aThreshold        - [Input] Matching threshold (max Hamming distance)
   \param aRangeCheck       - [Input] Range check (min Hamming distance between
                              the closest and the second closest descriptors found)
*****************************************************************************/
void Match(const vec08u* apcDescriptors0, unsigned int aDescriptor0Count, const vec08u* apcDescriptors1, unsigned int aDescriptor1Count, int16s* apMatches0, int16s* apMatches1, int08u aThreshold, int08u aRangeCheck);

#endif /* APUMATCHDESCRIPTORSIMPL_H */

/*! @} */
