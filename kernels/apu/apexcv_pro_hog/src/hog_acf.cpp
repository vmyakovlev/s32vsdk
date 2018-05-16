/**************************************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright 2015-2017 NXP
* All Rights Reserved
*
*****************************************************************************
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
**************************************************************************************************/

 #include "hog_apu.h"
 #include "hog_config.h"

#ifdef ACF_KERNEL_METADATA

KERNEL_INFO _kernel_info_hog_get_gradient
(
  "HOG_getGradient",
  3,
  __port(__index(0),
         __identifier("IMAGE"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(1, 1, 1, 1),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(1),
         __identifier("GRADIENT_MAGNITUDE"),
         __attributes(ACF_ATTR_VEC_OUT),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(2),
         __identifier("GRADIENT_ANGLE_BIN"),
         __attributes(ACF_ATTR_VEC_OUT),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1))
);

KERNEL_INFO _kernel_info_hog_get_cell_histograms
(
  "HOG_getCellHistograms",
  3,
  __port(__index(0),
         __identifier("CELL_HISTOGRAMS"),
         __attributes(ACF_ATTR_VEC_OUT),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16u),
         __e0_size(N_BINS, 1),
         __ek_size(1, 1)),
  __port(__index(1),
         __identifier("GRADIENT_MAGNITUDE"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(DefaultParams.winStride.width, DefaultParams.winStride.height)),
  __port(__index(2),
         __identifier("GRADIENT_ANGLE_BIN"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(DefaultParams.winStride.width, DefaultParams.winStride.height))
);

KERNEL_INFO _kernel_info_hog_compute_block_histograms
(
  "HOG_computeBlockHistograms",
  3,
  __port(__index(0),
         __identifier("BLOCK_HISTOGRAMS"),
         __attributes(ACF_ATTR_VEC_OUT_STATIC),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(N_BINS, 1),
         __ek_size(1, DefaultParams.winStridePerBlock.height * DefaultParams.winBlocks.height)),
  __port(__index(1),
         __identifier("CELL_HISTOGRAMS"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 1, 0, 1),
         __e0_data_type(d16u),
         __e0_size(N_BINS, 1),
         __ek_size(1, 1)),
  __port(__index(2),
         __identifier("ROW"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1))
);

KERNEL_INFO _kernel_info_hog_get_block_histograms
(
  "HOG_getBlockHistograms",
  2,
  __port(__index(0),
         __identifier("BLOCK_HISTOGRAMS"),
         __attributes(ACF_ATTR_VEC_OUT),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(N_BINS, 1)),
  __port(__index(1),
         __identifier("CELL_HISTOGRAMS"),
         __attributes(ACF_ATTR_VEC_IN),
         __spatial_dep(0, 1, 0, 1),
         __e0_data_type(d16u),
         __e0_size(N_BINS, 1),
         __ek_size(1, 1))
);

KERNEL_INFO _kernel_info_hog_get_scores
(
  "HOG_getScores",
  7,
  __port(__index(0),
         __identifier("SCORES"),
         __attributes(ACF_ATTR_VEC_OUT),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d16s),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(1),
         __identifier("BLOCK_HISTOGRAMS"),
         __attributes(ACF_ATTR_VEC_IN_STATIC),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(N_BINS, 1),
         __ek_size(1, DefaultParams.winStridePerBlock.height * DefaultParams.winBlocks.height)),
  __port(__index(2),
         __identifier("SVM_FUNCTION"),
         __attributes(ACF_ATTR_SCL_IN_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08s),
         __e0_size(1, 1),
         __ek_size(DefaultParams.svm.width + 2, DefaultParams.svm.height)),
  __port(__index(3),
         __identifier("X_LUT"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(DefaultParams.winBlocks.width, 1)),
  __port(__index(4),
         __identifier("R_LUT"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1)),
  __port(__index(5),
         __identifier("Y_LUT"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(DefaultParams.winStridePerBlock.height * DefaultParams.winBlocks.height, 1)),
  __port(__index(6),
         __identifier("ROW"),
         __attributes(ACF_ATTR_SCL_OUT_STATIC_FIXED),
         __spatial_dep(0, 0, 0, 0),
         __e0_data_type(d08u),
         __e0_size(1, 1),
         __ek_size(1, 1))
);

#endif   // #ifdef ACF_KERNEL_METADATA

#ifdef ACF_KERNEL_IMPLEMENTATION

void HOG_getGradient(kernel_io_desc image,
                     kernel_io_desc magnitude,
                     kernel_io_desc angleBin)
{
  const vec08u* iPtr = (const vec08u*)image.pMem;
  vec08u* mPtr = (vec08u*)magnitude.pMem;
  vec08u* aPtr = (vec08u*)angleBin.pMem;

  HOG_getGradient(mPtr, magnitude.chunkSpan,
                  aPtr, angleBin.chunkSpan,
                  iPtr, image.chunkSpan,
                  magnitude.chunkHeight,
                  magnitude.chunkWidth);
}

void HOG_getCellHistograms(kernel_io_desc cellHistograms,
                           kernel_io_desc magnitude,
                           kernel_io_desc angleBin)
{
  vec16u* c = (vec16u*)cellHistograms.pMem;
  const vec08u* m = (const vec08u*)magnitude.pMem;
  const vec08u* a = (const vec08u*)angleBin.pMem;

  HOG_getCellHistograms(c, cellHistograms.chunkSpan>>1,
                        m, magnitude.chunkSpan,
                        a, angleBin.chunkSpan,
                        magnitude.chunkWidth, magnitude.chunkHeight,
                        DefaultParams.winStride.width, DefaultParams.winStride.height);
}

void HOG_computeBlockHistograms(kernel_io_desc blockHistograms,
                                kernel_io_desc cellHistograms,
                                kernel_io_desc rowBuf)
{
  vec08u*  b = (vec08u*)blockHistograms.pMem;
  vec16u*  c = (vec16u*)cellHistograms.pMem;
  int08u row = *((int08u*)rowBuf.pMem);

  if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
  {
    row = 0;
  }

  HOG_getBlockHistograms(b + row * blockHistograms.chunkWidth * N_BINS,
                         blockHistograms.chunkSpan,
                         c, cellHistograms.chunkSpan>>1,
                         cellHistograms.chunkWidth,
                         cellHistograms.chunkHeight,
                         DefaultParams.winStridePerBlock.width,
                         DefaultParams.winStridePerBlock.height);

  row++;
  if(row == DefaultParams.winStridePerBlock.height * DefaultParams.winBlocks.height)
  {
    row = 0;
  }
  *(int08u*)(rowBuf.pMem) = row;
}

void HOG_getBlockHistograms(kernel_io_desc blockHistograms,
                            kernel_io_desc cellHistograms)
{
  vec08u*  b = (vec08u*)blockHistograms.pMem;
  vec16u*  c = (vec16u*)cellHistograms.pMem;

  HOG_getBlockHistograms(b, blockHistograms.chunkSpan,
                         c, cellHistograms.chunkSpan>>1,
                         cellHistograms.chunkWidth,
                         cellHistograms.chunkHeight,
                         DefaultParams.winStridePerBlock.width,
                         DefaultParams.winStridePerBlock.height);
}

void HOG_getScores(kernel_io_desc scores,
                   kernel_io_desc blockHistograms,
                   kernel_io_desc svmFunction,
                   kernel_io_desc x_lut,
                   kernel_io_desc r_lut,
                   kernel_io_desc y_lut,
                   kernel_io_desc rowBuf)
{
  vec16s* s = (vec16s*)scores.pMem;
  const vec08u* b = (const vec08u*)blockHistograms.pMem;
  const int08s* f = (const int08s*)svmFunction.pMem;
  int08u* x = (int08u*)x_lut.pMem;
  int08u* r = (int08u*)r_lut.pMem;
  int08u* y = (int08u*)y_lut.pMem;
  int08u row = *((int08u*)rowBuf.pMem);

  if(ACF_RET_VAR(ACF_VAR_FIRST_TILE_FLAG))
  {
    row = 0;

    HOG_createLUTs(x, r, y, 
        blockHistograms.chunkWidth, 
        DefaultParams.winStridePerBlock.width, 
        DefaultParams.winStridePerBlock.height, 
        DefaultParams.winBlocks.width, 
        DefaultParams.winBlocks.height);
  }

  HOG_getScores(s,
                b,
                blockHistograms.chunkWidth,
                blockHistograms.chunkHeight,
                blockHistograms.chunkSpan,
                f,
                x,
                y,
                r,
                row,
                DefaultParams.winStridePerBlock.height,
                DefaultParams.winBlocks.width,
                DefaultParams.winBlocks.height,
                DefaultParams.svm.width * DefaultParams.svm.height);

  row++;
  if(row == DefaultParams.winStridePerBlock.height * DefaultParams.winBlocks.height)
  {
    row = 0;
  }
  *(int08u*)(rowBuf.pMem) = row;
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
