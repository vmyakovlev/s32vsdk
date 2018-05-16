/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
*  All Rights Reserved
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
* \file correlation_apu.cpp
* \brief Convolution with general filter 1D/2D
* \author Anca Dima
* \version
* \date
****************************************************************************/

// In order to compile it as CPP do following: 
// In Visual Studio->Solution Explorer->Project "correlation_Kernel", right click -> Properties -> C/C++ ->Advanced->CompileAs->Compile As C++
#ifdef ACF_KERNEL_IMPLEMENTATION

#include "correlation_apu.h"
#include "symmetry_flags.h"

corrKernelPtr corrKernelSelector[SYM_XY + 1] = { // This might be buggy. Prefer using the initFilters function
	  correlation_filter			// NO_SYM 0x0
	, correlation__antisymYfilter	// ANTI_SYM_Y 0x1 
	, correlation__antisymXfilter	// ANTI_SYM_X 0x2
	, correlation__antisymXYfilter	// ANTI_SYM_XY 0x3
	, correlation__symYfilter		// SYM_Y 0x4
	, 0								// empty 05
	, correlation__antisymXsymYfilter// ANTI_SYM_X_SYM_Y 0x6
	, 0								// empty 07
	, correlation__symXfilter		// SYM_X 0x8
	, correlation__symXantisymYfilter// SYM_X_ANTI_SYM_Y 0x9
	, 0								// empty 0xa
	, 0								// empty 0xb
	, correlation__symXYfilter		// SYM_XY 0xc
};



void initFilters() {
	// Not accepted by the Chess compiler
	//memset(corrKernelSelector, '\0', (SYM_XY + 1) * sizeof(corrKernelPtr)); // initialize with NULL
	for (int i = 0; i <= SYM_XY; ++i){
		corrKernelSelector[i] = '\0';
	}

	corrKernelSelector[NO_SYM] = correlation_filter;			// NO_SYM 0x0
	corrKernelSelector[ANTI_SYM_Y] = correlation__antisymYfilter;	// ANTI_SYM_Y 0x1 
	corrKernelSelector[ANTI_SYM_X] = correlation__antisymXfilter;	// ANTI_SYM_X 0x2
	corrKernelSelector[ANTI_SYM_XY] = correlation__antisymXYfilter;	// ANTI_SYM_XY 0x3
	corrKernelSelector[SYM_Y] = correlation__symYfilter;		// SYM_Y 0x4
	corrKernelSelector[ANTI_SYM_X_SYM_Y] = correlation__antisymXsymYfilter;// ANTI_SYM_X_SYM_Y 0x6
	corrKernelSelector[SYM_X] = correlation__symXfilter;		// SYM_X 0x8
	corrKernelSelector[SYM_X_ANTI_SYM_Y] = correlation__symXantisymYfilter;// SYM_X_ANTI_SYM_Y 0x9
	corrKernelSelector[SYM_XY] = correlation__symXYfilter;		// SYM_XY 0xc
}

void performCorrelation(int16u filterFlags, vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs) {
	/*int nrKernels = sizeof(corrKernelSelector) / sizeof(corrKernelPtr);
	if (filterFlags < nrKernels && corrKernelSelector[filterFlags] != 0) {
		corrKernelSelector[filterFlags](dst, src, sstr, bw, bh, destBw, xSkip, ySkip, filtWidth, filtHeight, scaleFact, filterCoefs);
	}*/
	correlation_filter(dst, src, sstr, bw, bh, destBw, xSkip, ySkip, filtWidth, filtHeight, scaleFact, filterCoefs);
}



void correlation_filter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

   vec08u* fsrc = src;
   vec08u* crtLine = fsrc - sstr*halfFiltHeight;
   int16s maxH = bh - ySkip;
   int16s maxW = bw - xSkip;
   int16s vertSkip = ySkip * sstr;
 
   for (int16s by=0; by <= maxH; by += ySkip)  // y data row
   {
     for (int16s bx=0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
     {
       vec16s r = 0;
	   vec08u *tmpFSrc = crtLine;
	   
	   const int16s *crtFiltRow = filterCoefs; // go behind the pointer to land exactly on the beginning on the first iteration
	   for (int16u fyIdx = 0; fyIdx < filtHeight; ++fyIdx) {
		   for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth; fxIdx < filtWidth; ++fxIdx, ++srcIdx) {
			   int16s crtCoef = crtFiltRow[fxIdx];
			   if (crtCoef != 0) {
				   r += ((vec16s)(tmpFSrc[srcIdx])) * crtCoef;
			   }
		   } // one row of the filter was processed

		   crtFiltRow += filtWidth;
		   tmpFSrc += sstr; // next input row of the filter is sstr lines below
	   } // One pixel of the input img corrolved

	   // Scaling
       dst[dstX] =(r / scaleFact);
     }

     dst  += destBw;
     crtLine += vertSkip;
   }
} 


void correlation__symXfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec08u* fsrc = src;
	vec08u* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
			vec16s r = 0;
			vec08u *tmpFSrc = crtLine;

			const int16s *crtFiltRow = filterCoefs; // go behind the pointer to land exactly on the beginning on the first iteration
			for (int16u fyIdx = 0; fyIdx < filtHeight; ++fyIdx) {				
				for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth, backIdx = srcIdx+filtWidth-1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
					int16s crtCoef = crtFiltRow[fxIdx];
					if (crtCoef != 0) {
						r += ((vec16s)(tmpFSrc[srcIdx]) + (vec16s)(tmpFSrc[backIdx])) * crtCoef;
					}
				} // one row of the filter was processed

				int16s centerRowCoef = crtFiltRow[halfFiltWidth];
				if (centerRowCoef != 0){
					r += ((vec16s)(tmpFSrc[bx])) * centerRowCoef;
				}
				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
			} // One pixel of the input img corrolved

			//fsrc += xSkip; // go to the next pixel, i.e. go to the next line of the fsrc vect.

			// Scaling
			dst[dstX] =(r / scaleFact);
		}

		dst += destBw; // jump to the next output line of the destination buffer
		crtLine += vertSkip;
	}
}


void correlation__antisymXfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec08u* fsrc = src;
	vec08u* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
			vec16s r = 0;
			vec08u *tmpFSrc = crtLine;
			const int16s *crtFiltRow = filterCoefs; // go behind the pointer to land exactly on the beginning on the first iteration
			for (int16u fyIdx = 0; fyIdx < filtHeight; ++fyIdx) {
				for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
					int16s crtCoef = crtFiltRow[fxIdx];
					if (crtCoef != 0) {
						r += ((vec16s)(tmpFSrc[srcIdx]) - (vec16s)(tmpFSrc[backIdx])) * crtCoef;
					}
				} // one row of the filter was processed

				// corrolve also with the central filter value
				if (crtFiltRow[halfFiltWidth] != 0) {
					r += ((vec16s)(tmpFSrc[bx])) * crtFiltRow[halfFiltWidth];
				}

				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
			} // One pixel of the input img corrolved

			//fsrc += xSkip; // go to the next pixel, i.e. go to the next line of the fsrc vect.

			// Scaling
			dst[dstX] =(r / scaleFact);
		}
		dst += destBw;
		crtLine += vertSkip;
	}
}


void correlation__symYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec08u* fsrc = src;
	vec08u* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;

	const int16s* halfFiltRow = filterCoefs + halfFiltHeight * filtWidth;
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
			vec16s r = 0;
			vec08u *tmpFSrc = crtLine;
			vec08u *tmpBackSrc = crtLine + (filtHeight - 1) * sstr;
			const int16s *crtFiltRow = filterCoefs;
			for (int16u fyIdx = 0; fyIdx < halfFiltHeight; ++fyIdx) {
				for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth; fxIdx < filtWidth; ++fxIdx, ++srcIdx) {
					int16s crtCoef = crtFiltRow[fxIdx];
					if (crtCoef != 0) {
						r += ((vec16s)(tmpFSrc[srcIdx]) + (vec16s)(tmpBackSrc[srcIdx])) * crtCoef;
					}
				} // one row of the filter was processed
				
				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
				tmpBackSrc -= sstr;
			} // One pixel of the input img corrolved

			// corrolve also with the central row of the filter			
			for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth; fxIdx < filtWidth; ++fxIdx, ++srcIdx) {
				int16s crtHalfRowCoef = halfFiltRow[fxIdx];
				if (crtHalfRowCoef != 0) {
					r += ((vec16s)(tmpFSrc[srcIdx])) * crtHalfRowCoef;
				}
			}

			//fsrc += xSkip; // go to the next pixel, i.e. go to the next line of the fsrc vect.

			// Scaling
			dst[dstX] =(r / scaleFact);
		}
		dst += destBw;
		crtLine += vertSkip;
	}
}



void correlation__antisymYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec08u* fsrc = src;
	vec08u* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;

	const int16s* halfFiltRow = filterCoefs + halfFiltHeight * filtWidth;
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
			vec16s r = 0;
			vec08u *tmpFSrc = crtLine;
			vec08u *tmpBackSrc = crtLine + (filtHeight - 1) * sstr;
			const int16s *crtFiltRow = filterCoefs;
			for (int16u fyIdx = 0; fyIdx < halfFiltHeight; ++fyIdx) {
				for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth; fxIdx < filtWidth; ++fxIdx, ++srcIdx) {
					int16s crtCoef = crtFiltRow[fxIdx];
					if (crtCoef != 0) {
						r += ((vec16s)(tmpFSrc[srcIdx]) - (vec16s)(tmpBackSrc[srcIdx]) ) * crtCoef;
					}
				} // one row of the filter was processed

				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
				tmpBackSrc -= sstr;
			} // One pixel of the input img corrolved

			// corrolve also with the central row of the filter			
			for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth; fxIdx < filtWidth; ++fxIdx, ++srcIdx) {
				int16s crtHalfRowCoef = halfFiltRow[fxIdx];
				if (crtHalfRowCoef != 0) {
					r += ((vec16s)(tmpFSrc[srcIdx])) * crtHalfRowCoef;
				}
			}

			//fsrc += xSkip;//sstr;// go to the next pixel, i.e. go to the next line of the fsrc vect.

			// Scaling
			dst[dstX] =(r / scaleFact);
		}
		dst += destBw;
		crtLine += vertSkip;
	}
}



void correlation__symXYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec08u* fsrc = src;
	vec08u* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;

	const int16s* halfFiltRow = filterCoefs + halfFiltHeight * filtWidth;
	int16s centerCoef = halfFiltRow[halfFiltWidth];
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
			vec16s r = 0;
			vec08u *tmpFSrc = crtLine;
			vec08u *tmpBackSrc = crtLine + (filtHeight - 1) * sstr;

			const int16s *crtFiltRow = filterCoefs; // go behind the pointer to land exactly on the beginning on the first iteration
			for (int16u fyIdx = 0; fyIdx < halfFiltHeight; ++fyIdx) {
				for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
					int16s crtCoef = crtFiltRow[fxIdx];
					if (crtCoef != 0) {
						r += ((vec16s)(tmpFSrc[srcIdx]) + (vec16s)(tmpFSrc[backIdx]) + (vec16s)(tmpBackSrc[srcIdx]) + (vec16s)(tmpBackSrc[backIdx])) * crtCoef;
					}
				} // one row of the filter was processed

				if (crtFiltRow[halfFiltWidth] != 0){
					r += ((vec16s)(tmpFSrc[bx]) + (vec16s)(tmpBackSrc[bx])) * crtFiltRow[halfFiltWidth];
				}
				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
				tmpBackSrc -= sstr; //prev input row for the y-symmetry
			} // One pixel of the input img corrolved

			// corrolve also with the central row of the filter			
			for (int16s fxIdx = 0, srcIdx = bx - halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
				int16s crtHalfRowCoef = halfFiltRow[fxIdx];
				if (crtHalfRowCoef != 0) {
					r += ((vec16s)(tmpFSrc[srcIdx]) + (vec16s)(tmpFSrc[backIdx])) * crtHalfRowCoef;
				}
			}


			if (centerCoef != 0){
				r += ((vec16s)(tmpFSrc[bx])) * centerCoef;
			}


			//fsrc += xSkip; // go to the next pixel, i.e. go to the next line of the fsrc vect.

			// Scaling
			dst[dstX] =(r / scaleFact);
		}
		dst += destBw;
		crtLine += vertSkip;
	}
}



void correlation__symXantisymYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec08u* fsrc = src;
	vec08u* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;

	const int16s* halfFiltRow = filterCoefs + halfFiltHeight * filtWidth;
	int16s centerCoef = halfFiltRow[halfFiltWidth];
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
#ifdef _DEBUG
//			fprintf(stderr, "i = %d, j = %d\n", bx, by);
#endif
			vec16s r = 0;
			
			vec08u *tmpFSrc = crtLine;
			vec08u *tmpBackSrc = crtLine + (filtHeight - 1) * sstr;

			const int16s *crtFiltRow = filterCoefs; // go behind the pointer to land exactly on the beginning on the first iteration
			for (int16u fyIdx = 0; fyIdx < halfFiltHeight; ++fyIdx) {
				for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
					int16s crtCoef = (int16s) crtFiltRow[fxIdx];
#ifdef _DEBUG
					//fprintf(stderr, "%d\t%d\t\n%d\t%d\t\n", vget(tmpFSrc[srcIdx], 0), vget(tmpFSrc[backIdx], 0), vget(tmpBackSrc[srcIdx], 0), vget(tmpBackSrc[backIdx], 0));
#endif

					if (crtCoef != 0) {
						r += ((vec16s)(tmpFSrc[srcIdx]) + (vec16s)(tmpFSrc[backIdx]) - ((vec16s)(tmpBackSrc[srcIdx]) + (vec16s)(tmpBackSrc[backIdx]))) * crtCoef;
					}
				} // one row of the filter was processed

				if (crtFiltRow[halfFiltWidth] != 0){
					r += ((vec16s)(tmpFSrc[bx]) - (vec16s)(tmpBackSrc[bx])) * crtFiltRow[halfFiltWidth];
				}

#ifdef _DEBUG
	//			fprintf(stderr, "%d\t%d\t\n", vget(tmpFSrc[bx], 0), vget(tmpBackSrc[bx], 0));
#endif

				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
				tmpBackSrc -= sstr; //prev input row for the y-symmetry
			} // One pixel of the input img corrolved


			// corrolve also with the central row of the filter			
			for (int16s fxIdx = 0, srcIdx = bx - halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
				int16s crtHalfRowCoef =  halfFiltRow[fxIdx];
				if (crtHalfRowCoef != 0) {
					r += ((vec16s)(tmpFSrc[srcIdx]) + (vec16s)(tmpFSrc[backIdx])) * crtHalfRowCoef;
#ifdef _DEBUG
//					fprintf(stderr, "%d\t%d\t\n", vget(tmpFSrc[srcIdx], 0), vget(tmpFSrc[backIdx], 0), vget(r, 0));
#endif
				}
			}

			if (centerCoef != 0){
				r += ((vec16s)(tmpFSrc[bx])) * centerCoef;
			}
#ifdef _DEBUG
	//		fprintf(stderr, "%d\n", vget(r, 0));
#endif

			//fsrc += xSkip;//sstr;// go to the next pixel, i.e. go to the next line of the fsrc vect.

			// Scaling
			dst[dstX] = r / scaleFact;
		}
		dst += destBw;
		crtLine += vertSkip;
	}
}

void correlation__symXantisymYfilter_16s(vec16s* dst, vec16s* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec16s* fsrc = src;
	vec16s* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;

	const int16s* halfFiltRow = filterCoefs + halfFiltHeight * filtWidth;
	int32s centerCoef = (int32s) halfFiltRow[halfFiltWidth];
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
#ifdef _DEBUG
	//		fprintf(stderr, "i = %d, j = %d\n", bx, by);
#endif
			//vec16s r = 0;
			vec32s tmpR = 0;
			vec16s *tmpFSrc = crtLine;
			vec16s *tmpBackSrc = crtLine + (filtHeight - 1) * sstr;

			const int16s *crtFiltRow = filterCoefs; // go behind the pointer to land exactly on the beginning on the first iteration
			for (int16u fyIdx = 0; fyIdx < halfFiltHeight; ++fyIdx) {
				for (int16s fxIdx = 0, srcIdx = bx - halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
					int32s crtCoef = (int32s) crtFiltRow[fxIdx];
#ifdef _DEBUG
	//				fprintf(stderr, "%d\t%d\t\n%d\t%d\t\n", vget(tmpFSrc[srcIdx], 0), vget(tmpFSrc[backIdx], 0), vget(tmpBackSrc[srcIdx], 0), vget(tmpBackSrc[backIdx], 0));
#endif

					if (crtCoef != 0) {
						tmpR += ((vec32s)(tmpFSrc[srcIdx]) + (vec32s)(tmpFSrc[backIdx]) - ((vec32s)(tmpBackSrc[srcIdx]) + (vec32s)(tmpBackSrc[backIdx]))) * crtCoef;
					}
				} // one row of the filter was processed

				if (crtFiltRow[halfFiltWidth] != 0){
					tmpR += ((vec32s)(tmpFSrc[bx]) - (vec32s)(tmpBackSrc[bx])) * crtFiltRow[halfFiltWidth];
				}

#ifdef _DEBUG
	//			fprintf(stderr, "%d\t%d\t\n", vget(tmpFSrc[bx], 0), vget(tmpBackSrc[bx], 0));
#endif

				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
				tmpBackSrc -= sstr; //prev input row for the y-symmetry
			} // One pixel of the input img corrolved


			// corrolve also with the central row of the filter			
			for (int16s fxIdx = 0, srcIdx = bx - halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
				int32s crtHalfRowCoef = (int32s) halfFiltRow[fxIdx];
				if (crtHalfRowCoef != 0) {
					tmpR += ((vec32s)(tmpFSrc[srcIdx]) + (vec32s)(tmpFSrc[backIdx])) * crtHalfRowCoef;
#ifdef _DEBUG
	//				fprintf(stderr, "%d\t%d\t\n", vget(tmpFSrc[srcIdx], 0), vget(tmpFSrc[backIdx], 0), vget(r, 0));
#endif
				}
			}

			if (centerCoef != 0){
				tmpR += ((vec32s)(tmpFSrc[bx])) * centerCoef;
			}
#ifdef _DEBUG
		//	fprintf(stderr, "%d\n", vget(r, 0));
#endif

			//fsrc += xSkip;//sstr;// go to the next pixel, i.e. go to the next line of the fsrc vect.

			// Scaling
			dst[dstX] = vec16s(tmpR / (int32s) scaleFact);
		}
		dst += destBw;
		crtLine += vertSkip;
	}
}



void correlation__antisymXsymYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec08u* fsrc = src;
	vec08u* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;

	const int16s* halfFiltRow = filterCoefs + halfFiltHeight * filtWidth;
	int16s centerCoef = halfFiltRow[halfFiltWidth];
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
			vec16s r = 0;
			vec08u *tmpFSrc = crtLine;
			vec08u *tmpBackSrc = crtLine + (filtHeight - 1) * sstr;
#ifdef _DEBUG
//			fprintf(stderr, "i = %d, j = %d\n", bx, by);
#endif
			const int16s *crtFiltRow = filterCoefs; // go behind the pointer to land exactly on the beginning on the first iteration
			for (int16u fyIdx = 0; fyIdx < halfFiltHeight; ++fyIdx) {
				for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
					int16s crtCoef = crtFiltRow[fxIdx];
#ifdef _DEBUG
//					fprintf(stderr, "%d\t%d\t\n%d\t%d\t\n", vget(tmpFSrc[srcIdx], 0), vget(tmpFSrc[backIdx], 0), vget(tmpBackSrc[srcIdx], 0), vget(tmpBackSrc[backIdx], 0));
#endif

					if (crtCoef != 0) {
						r += ((vec16s)(tmpFSrc[srcIdx]) - (vec16s)(tmpFSrc[backIdx]) + ((vec16s)(tmpBackSrc[srcIdx]) - (vec16s)(tmpBackSrc[backIdx]))) * crtCoef;
					}
				} // one row of the filter was processed

				if (crtFiltRow[halfFiltWidth] != 0){
					r += ((vec16s)(tmpFSrc[bx]) + (vec16s) (tmpBackSrc[bx])) * crtFiltRow[halfFiltWidth];
				}

#ifdef _DEBUG
//				fprintf(stderr, "%d\t%d\t\n", vget(tmpFSrc[bx], 0), vget(tmpBackSrc[bx], 0));
#endif

				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
				tmpBackSrc -= sstr; //prev input row for the y-symmetry
			} // One pixel of the input img corrolved


			// corrolve also with the central row of the filter			
			for (int16s fxIdx = 0, srcIdx = bx - halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
				int16s crtHalfRowCoef = halfFiltRow[fxIdx];
				if (crtHalfRowCoef != 0) {
					r += ((vec16s)(tmpFSrc[srcIdx]) - (vec16s)(tmpFSrc[backIdx])) * crtHalfRowCoef;
#ifdef _DEBUG
//					fprintf(stderr, "%d\t%d\t\n", vget(tmpFSrc[srcIdx], 0), vget(tmpFSrc[backIdx], 0));
#endif

				}
			}


			if (centerCoef != 0){
				r += ((vec16s)(tmpFSrc[bx])) * centerCoef;
			}

			//fsrc += xSkip;//sstr;// go to the next pixel, i.e. go to the next line of the fsrc vect.
#ifdef _DEBUG
//			fprintf(stderr, "%d\n",  vget(r, 0));
#endif

			// Scaling
			dst[dstX] =(r / scaleFact);
		}
		dst += destBw;
		crtLine += vertSkip;
	}
}

void correlation__antisymXsymYfilter_16s(vec16s* dst, vec16s* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;
	vec32s vScaleFact = vec32s(scaleFact);

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec16s* fsrc = src;
	vec16s* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;

	const int16s* halfFiltRow = filterCoefs + halfFiltHeight * filtWidth;
	int32s centerCoef = (int32s) halfFiltRow[halfFiltWidth];
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
			//vec16s r = 0;
			vec32s tmpR = 0;
			vec16s *tmpFSrc = crtLine;
			vec16s *tmpBackSrc = crtLine + (filtHeight - 1) * sstr;
#ifdef _DEBUG
//			fprintf(stderr, "i = %d, j = %d\n", bx, by);
#endif
			const int16s *crtFiltRow = filterCoefs; // go behind the pointer to land exactly on the beginning on the first iteration
			for (int16u fyIdx = 0; fyIdx < halfFiltHeight; ++fyIdx) {
				for (int16s fxIdx = 0, srcIdx = bx - halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
					int32s crtCoef = (int32s) crtFiltRow[fxIdx];
#ifdef _DEBUG
					//fprintf(stderr, "%d\t%d\t\n%d\t%d\t\n", vget(tmpFSrc[srcIdx], 0), vget(tmpFSrc[backIdx], 0), vget(tmpBackSrc[srcIdx], 0), vget(tmpBackSrc[backIdx], 0));
#endif

					if (crtCoef != 0) {
						tmpR += ((vec32s)(tmpFSrc[srcIdx]) - (vec32s)(tmpFSrc[backIdx]) + ((vec32s)(tmpBackSrc[srcIdx]) - (vec32s)(tmpBackSrc[backIdx]))) * crtCoef;
					}
				} // one row of the filter was processed

				if (crtFiltRow[halfFiltWidth] != 0){
					tmpR += ((vec32s)(tmpFSrc[bx]) + (vec32s)(tmpBackSrc[bx])) * ((int32s)crtFiltRow[halfFiltWidth]);
				}

#ifdef _DEBUG
	//			fprintf(stderr, "%d\t%d\t\n", vget(tmpFSrc[bx], 0), vget(tmpBackSrc[bx], 0));
#endif

				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
				tmpBackSrc -= sstr; //prev input row for the y-symmetry
			} // One pixel of the input img corrolved


			// corrolve also with the central row of the filter			
			for (int16s fxIdx = 0, srcIdx = bx - halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
				int32s crtHalfRowCoef = (int32s) halfFiltRow[fxIdx];
				if (crtHalfRowCoef != 0) {
					tmpR += ((vec32s)(tmpFSrc[srcIdx]) - (vec32s)(tmpFSrc[backIdx])) * crtHalfRowCoef;
#ifdef _DEBUG
	//				fprintf(stderr, "%d\t%d\t\n", vget(tmpFSrc[srcIdx], 0), vget(tmpFSrc[backIdx], 0));
#endif

				}
			}


			if (centerCoef != 0){
				tmpR += ((vec32s)(tmpFSrc[bx])) * centerCoef;
			}

			//fsrc += xSkip;//sstr;// go to the next pixel, i.e. go to the next line of the fsrc vect.
#ifdef _DEBUG
	//		fprintf(stderr, "%d\n", vget(r, 0));
#endif

			// Scaling
			dst[dstX] = vec16s(tmpR / vScaleFact);
		}
		dst += destBw;
		crtLine += vertSkip;
	}
}
void correlation__antisymXYfilter(vec16s* dst, vec08u* src, int16s sstr, int16s bw, int16s bh, int16s destBw, int16s xSkip, int16s ySkip, int16u filtWidth, int16u filtHeight, int16s scaleFact, const int16s* filterCoefs)
{
	int16s halfFiltWidth = filtWidth >> 1;
	int16s halfFiltHeight = filtHeight >> 1;

	//assert(sstr > filtWidth && bw  > filtWidth && bh > filtHeight);

	vec08u* fsrc = src;
	vec08u* crtLine = fsrc - sstr*halfFiltHeight;
	int16s maxH = bh - ySkip;
	int16s maxW = bw - xSkip;
	int16s vertSkip = ySkip * sstr;

	const int16s* halfFiltRow = filterCoefs + halfFiltHeight * filtWidth;
	int16s centerCoef = halfFiltRow[halfFiltWidth];
	for (int16s by = 0; by <= maxH; by += ySkip)  // y data row
	{
		for (int16s bx = 0, dstX = 0; bx <= maxW; bx += xSkip, ++dstX) // x in the blck_tile
		{
			vec16s r = 0;
			vec08u *tmpFSrc = crtLine;
			vec08u *tmpBackSrc = crtLine + (filtHeight - 1) * sstr;

			const int16s *crtFiltRow = filterCoefs; // go behind the pointer to land exactly on the beginning on the first iteration
			for (int16u fyIdx = 0; fyIdx < filtHeight; ++fyIdx) {
				for (int16s fxIdx = 0, srcIdx = bx- halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
					int16s crtCoef = crtFiltRow[fxIdx];
					if (crtCoef != 0) {
						r += ((vec16s)(tmpFSrc[srcIdx]) - (vec16s)(tmpFSrc[backIdx]) - (vec16s)(tmpBackSrc[srcIdx]) + (vec16s)(tmpBackSrc[backIdx]))  * crtCoef;
					}
				} // one row of the filter was processed

				if (crtFiltRow[halfFiltWidth] != 0){
					r += ((vec16s)(tmpFSrc[bx]) - (vec16s)(tmpBackSrc[bx])) * crtFiltRow[halfFiltWidth];
				}

				crtFiltRow += filtWidth;
				tmpFSrc += sstr; // next input row of the filter is sstr lines below
				tmpBackSrc -= sstr; //prev input row for the y-symmetry
			} // One pixel of the input img corrolved


			// corrolve also with the central row of the filter			
			for (int16s fxIdx = 0, srcIdx = bx - halfFiltWidth, backIdx = srcIdx + filtWidth - 1; fxIdx < halfFiltWidth; ++fxIdx, ++srcIdx, --backIdx) {
				int16s crtHalfRowCoef = halfFiltRow[fxIdx];
				if (crtHalfRowCoef != 0) {
					r += ((vec16s)(tmpFSrc[srcIdx]) - (vec16s)(tmpFSrc[backIdx])) * crtHalfRowCoef;
				}
			}


			if (centerCoef != 0){
				r += ((vec16s)(tmpFSrc[bx])) * centerCoef;
			}

			//fsrc += xSkip;//sstr;// go to the next pixel, i.e. go to the next line of the fsrc vect.

			// Scaling
			dst[dstX] =(r / scaleFact);
		}
		dst += destBw;
		crtLine += vertSkip;
	}
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
