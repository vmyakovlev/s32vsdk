/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
* 
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

/*!
* \file dot_division_acf.cpp
* \addtogroup arithmetic
* \addtogroup division
* \ingroup arithmetic
* @{
* \brief Element-wise division
*/
#ifdef APEX2_EMULATE
#include "acf_kernel.hpp" // if using the ACF emulation library
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_METADATA
#include "dot_division_acf.h"

/****************************************************************************/
/*!
Integer division kernel metadata. Divides pixelwise two signed 32bit images.
Outputs signed 32bit integer division result.
\warning Division by zero returns zero and not an out of range number.
\param  DIV_In32s_Out_32s_KN	Define for Kernel name
\param 3						Number of ports
\param "Port DIV_KN_INA"		Define for name of first input image (signed 32bit).
\param "Port DIV_KN_INB"		Define for name of second input image (signed 32bit).
\param "Port DIV_KN_OUT"		Define for name of division result (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(DIV_In32s_Out_32s_K) (
   DIV_In32s_Out_32s_KN,
   3,
   __port(__index(0),
          __identifier(DIV_KN_INA),
          __attributes(ACF_ATTR_VEC_IN),
          __spatial_dep(0,0,0,0), 
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(1, 1)),

  __port(__index(1),
		  __identifier(DIV_KN_INB),
		  __attributes(ACF_ATTR_VEC_IN),
		  __spatial_dep(0,0,0,0), 
		  __e0_data_type(d32s),
		  __e0_size(1, 1),
		  __ek_size(1, 1)),

   __port(__index(2),
          __identifier(DIV_KN_OUT),
          __attributes(ACF_ATTR_VEC_OUT),
          __spatial_dep(0,0,0,0),
          __e0_data_type(d32s),
          __e0_size(1, 1),
          __ek_size(1, 1))
);


/****************************************************************************/
/*!
Computes log2 of the absolute value of each image pixel. 
Outputs log2( abs(Input) ) + 1.
\param  LOG2_KN					Define for Kernel name
\param 2						Number of ports
\param "Port LOG2_KN_IN"		Define for name of input image (signed 32bit).
\param "Port LOG2_KN_LOG2Fact"	Define for name of logarithm result (signed 32bit).
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(LOG2_K)(
	LOG2_KN,
	2,
	__port(__index(0),
	__identifier(LOG2_KN_IN),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

	__port(__index(1),
	__identifier(LOG2_KN_LOG2Fact),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);


/****************************************************************************/
/*!
Inverse computation kernel metadata. Computes inverse of each image pixel with the Newton-Raphson method
Outputs signed 32bit inverse of the input in fixed point Q<ShiftFact>.(31-<ShiftFact>) format

\param  COMPUTE_INV_KN			Define for Kernel name
\param 4						Number of ports
\param "Port INV_KN_INV_DIV"	Define for name of the inverse of div in  Q<ShiftFact>.(31-<ShiftFact>)
\param "Port INV_KN_DIV"		Define for name of the image containing 32bit numbers to be inverted
\param "Port INV_KN_LOG2Fact"	Define for name of the returned image of unsigned 8bit log_2 values of each member of div
\param "Port INV_KN_SHIFTFact"	Define for name of the shift factor for the fixed point Q<ShiftFact>.(31-<ShiftFact>) format
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(COMPUTE_INV_K) (
	COMPUTE_INV_KN,
	4,

	__port(__index(0),
	__identifier(INV_KN_INV_DIV),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

	__port(__index(1),
	__identifier(INV_KN_DIV),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

	__port(__index(2),
	__identifier(INV_KN_LOG2Fact),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

	__port(__index(3),
	__identifier(INV_KN_SHIFTFact),
	__attributes(ACF_ATTR_SCL_IN_FIXED),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d08s),
	__e0_size(1, 1),
	__ek_size(1, 1))

);


/****************************************************************************/
/*!
Integer division kernel metadata. Divides pixelwise a 64bit integer image with a 32bit integer.
Outputs signed 64bit integer division result.
\warning Division by zero returns zero and not an out of range number.
\param  DIV_N64s_D32s_Q64s_K	Define for Kernel name
\param 6								Number of ports
\param "Port DIV_N64s_D32s_Q64s_KN_NOM_HIGH" Define for name of signed 32bit high word of input image (i.e. the nominator)
\param "Port DIV_N64s_D32s_Q64s_KN_NOM_LOW"	 Define for name of unsigned 32bit low word of input image (i.e. the nominator)
\param "Port DIV_N64s_D32s_Q64s_KN_DIVISOR"	 Define for name of of divisor (i.e. the denominator) (signed 32bit)
\param "Port DIV_N64s_D32s_Q64s_KN_OUT_HIGH" Define for name of signed 32bit high word of division result
\param "Port DIV_N64s_D32s_Q64s_KN_OUT_LOW"  Define for name of unsigned 32bit low word of division result
\param "Port DIV_N64s_D32s_Q64s_KN_OUT_REM"  Define for name of integer remainder of the division (unsigned 32bit)
*****************************************************************************/
KERNEL_INFO kernelInfoConcat(DIV_N64s_D32s_Q64s_K)(
	DIV_N64s_D32s_Q64s_KN,
	6,
	__port(__index(0),
	__identifier(DIV_N64s_D32s_Q64s_KN_NOM_HIGH),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

	__port(__index(1),
	__identifier(DIV_N64s_D32s_Q64s_KN_NOM_LOW),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

	__port(__index(2),
	__identifier(DIV_N64s_D32s_Q64s_KN_DIVISOR),
	__attributes(ACF_ATTR_VEC_IN),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

	__port(__index(3),  
	__identifier(DIV_N64s_D32s_Q64s_KN_OUT_HIGH),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32s),
	__e0_size(1, 1),
	__ek_size(1, 1)),

	__port(__index(4),
	__identifier(DIV_N64s_D32s_Q64s_KN_OUT_LOW),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1)),

	__port(__index(5),
	__identifier(DIV_N64s_D32s_Q64s_KN_OUT_REM),
	__attributes(ACF_ATTR_VEC_OUT),
	__spatial_dep(0, 0, 0, 0),
	__e0_data_type(d32u),
	__e0_size(1, 1),
	__ek_size(1, 1))
);

#endif //#ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION
#include "dot_division_apu.h"
 
void apu_dot_division(kernel_io_desc lImgInA, kernel_io_desc lImgInB, kernel_io_desc lImgOut)
{
   vec32s*  lpvImgInA	= (vec32s*) lImgInA.pMem;
   vec32s*  lpvImgInB	= (vec32s*) lImgInB.pMem;
   vec32s*  lpvImgOut	= (vec32s*) lImgOut.pMem;
   int16s inDataSz = sizeof(int32s);
   int16s outDataSz = sizeof(int32s);

   dot_division_filter(lpvImgOut, lpvImgInA, lpvImgInB, lImgInA.chunkWidth, lImgInA.chunkHeight, lImgInA.chunkSpan / inDataSz, lImgOut.chunkSpan / outDataSz);
}


void apu_dot_log2(kernel_io_desc lInput, kernel_io_desc lLog2Fact)
{
	vec32s* input = (vec32s*)lInput.pMem;
	vec08u* log2Fact = (vec08u*)lLog2Fact.pMem;
	int16s inStrideWidth = lInput.chunkSpan / sizeof(int32s)
		, bw = lInput.chunkWidth
		, bh = lInput.chunkHeight
		, outStrideWidth = lLog2Fact.chunkSpan / sizeof(int08u);
	computeLog2(log2Fact, input, bw, bh, inStrideWidth, outStrideWidth);
}


void apu_dot_inv_NewtonRaphson( kernel_io_desc lInvDiv, kernel_io_desc lDiv,kernel_io_desc lLog2Fact, kernel_io_desc lShiftFact)
{
	vec32s* div = (vec32s*)lDiv.pMem;
	vec32s* invDiv = (vec32s*)lInvDiv.pMem;
	vec08u* downSampleFact = (vec08u*)lLog2Fact.pMem;
	int08s shiftFact = *((int08s*)lShiftFact.pMem);
	int16s inStrideWidth = lDiv.chunkSpan / sizeof(int32s)
		, bw = lDiv.chunkWidth
		, bh = lDiv.chunkHeight
		, outStrideWidth = lInvDiv.chunkSpan / sizeof(int32s);

	computeInv_NewtonRaphson(invDiv, div, downSampleFact, shiftFact, bw, bh, inStrideWidth, outStrideWidth);
}

void apu_dot_division_N64s_D32s_Q64s(kernel_io_desc lNom_high, kernel_io_desc lNom_low, kernel_io_desc lDivisor, kernel_io_desc lDst_high, kernel_io_desc lDst_low, kernel_io_desc lDst_rem)
{
	vec32s* dst_high = (vec32s*)lDst_high.pMem;
	vec32u* dst_low = (vec32u*)lDst_low.pMem; 
	vec32u* dst_rem = (vec32u*)lDst_rem.pMem;
	vec32s* nom_high = (vec32s*)lNom_high.pMem;
	vec32u* nom_low = (vec32u*)lNom_low.pMem;
	vec32s* divisor = (vec32s*)lDivisor.pMem;
	int16s inStrideWidth = lNom_high.chunkSpan / sizeof(int32s)
		 , bw = lNom_high.chunkWidth
		 , bh = lNom_high.chunkHeight
		 , outStrideWidth = lDst_high.chunkSpan / sizeof(int32s);
	dot_division_filter_N64s_D32s_Q64s(dst_high, dst_low, dst_rem, nom_high, nom_low, divisor, bw, bh, inStrideWidth, outStrideWidth);
}



#endif //#ifdef ACF_KERNEL_IMPLEMENTATION

/*! @} */
