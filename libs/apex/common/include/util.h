/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define INLINE_VDIV 1

typedef short         int16s;
typedef unsigned char int08u;

//ALLIGNMENT MACRO
//example for 16-byte alignment: DECLARE_ALIGNED(int32_t, myAlignedBuffer[256], 16);
#ifdef __clang__
#define DECLARE_BYTE_ALIGNED(_TYPE, _NAME, _BYTE_ALIGNMENT) \
_TYPE _NAME __attribute__((aligned(_BYTE_ALIGNMENT)))
#elif __chess__
#define DECLARE_BYTE_ALIGNED(_TYPE, _NAME, _BYTE_ALIGNMENT) \
_TYPE chess_storage(%_BYTE_ALIGNMENT) _NAME
#endif

#define SWAP(_A,_B)  vswap((v##_A),(v##_B), (v##_A) < (v##_B))

int08u cmem_read  (vec08u __cmem* src, int cu);
int16u cmem_read  (vec16u __cmem* src, int cu);
void   cmem_write (vec08u __cmem* dst, int cu, int08u val);
void   cmem_write (vec16u __cmem* dst, int cu, int16u val);

void
TransferTileToCmem(unsigned char* input,
                   vec08u __cmem* dst,
                   int blkWidth,
                   int blkHeight,
                   int blkSpan,
                   int tileWidth,
                   int tileStride,
                   int tileOffsetX,
                   int tileOffsetY);

void
TransferTileToCmem(unsigned short* input,
                   vec16u __cmem* dst,
                   int blkWidth,
                   int blkHeight,
                   int blkSpan,
                   int tileWidth,
                   int tileStride,
                   int tileOffsetX,
                   int tileOffsetY);

void
CheckAgainstReference(int tileWidth,
                      int tileStride,
                      int tileHeight,
                      int blkWidth,
                      unsigned char* ref,
                      vec08u __cmem* vec,
                      int errorThreshold);
                      
void
CheckAgainstReference_YUV422(int tileWidth,
                             int tileStride,
                             int tileHeight,
                             int blkWidth,
                             unsigned short* ref,
                             vec16u __cmem* vec,
                             int errorThreshold);
                      
void
SetupPadding(vec08u __cmem* src,
             vec08u __cmem* padTop,
             vec08u __cmem* padBot,
             vec08u __cmem* padLeft,
             vec08u __cmem* padRight,
             int blkWidth,
             int blkHeight,
             int blkSpan,
             int filterSize);
             
void
GenBlockPadding (vec08u __cmem* src,
                 vec08u __cmem* padTop,
                 vec08u __cmem* padBot,
                 vec08u __cmem* padLeft,
                 vec08u __cmem* padRight,
                 vec08u __cmem* blk,
                 const int blkWidth,
                 const int blkHeight,
                 const int blkSpan,
                 const int padSize);
                 
void
GenBlockPadding_HARDCODED (vec08u __cmem* src,
                           vec08u __cmem* padTop,
                           vec08u __cmem* padBot,
                           vec08u __cmem* padLeft,
                           vec08u __cmem* padRight,
                           vec08u __cmem* blk);
       
#if (INLINE_VDIV == 1)                    
inline vec16s vdiv(vec16s dividend, vec08u divisor, int n)
{
   vbool negsign = (dividend < 0);
   
   vif (negsign)
   {
      dividend = -dividend;
   }
   vendif
   
   vec16s quotient  = 0;
   vec16s remainder = dividend;
      
   for (int N = n-1; N >= 0; N--)
   {
      vif((remainder >> N) >= (vec16s)divisor)
      {
         remainder -= (vec16s)(divisor << N);
         quotient  += (1 << N);
      }
      vendif
   }
   
   vif(negsign)
      quotient = -quotient;
   vendif
   
   return quotient;
}
#else
vec16s vdiv(vec16s dividend, vec08u divisor, int n);
#endif

#endif /* UTIL_H */
