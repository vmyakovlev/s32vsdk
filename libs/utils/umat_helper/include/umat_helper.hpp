/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2017 NXP Semiconductor;
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
****************************************************************************/

#ifndef UMAT_HELPER_HPP
#define UMAT_HELPER_HPP

#include <umat.hpp>
#include <icp_datatype.h>

vsdk::UMat CreateAcfFriendlyUMat(int32_t lRows, int32_t lCols, int32_t lType);
vsdk::UMat CreateAcfFriendlyUMat(int32_t lRows, int32_t lCols, int32_t lType,
                                 int32_t lPadL, int32_t lPadR, int32_t lPadT, int32_t lPadB);
void FillZero(vsdk::UMat lUMat);
void FillRandom(vsdk::UMat lUMat, uint32_t* lpSeedVal);
int32_t Compare(vsdk::UMat lUMatA, vsdk::UMat lUMatB);
                    
#endif //UMAT_HELPER_HPP
