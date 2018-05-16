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

#ifndef ICPDATATYPE_H
#define ICPDATATYPE_H

#include <stdint.h>

//here until the ICP_ variants disappear
#define ICP_DATATYPE     icp::DATATYPE
#define ICP_DATATYPE_08U icp::DATATYPE_08U
#define ICP_DATATYPE_08S icp::DATATYPE_08S
#define ICP_DATATYPE_16U icp::DATATYPE_16U
#define ICP_DATATYPE_16S icp::DATATYPE_16S
#define ICP_DATATYPE_32U icp::DATATYPE_32U
#define ICP_DATATYPE_32S icp::DATATYPE_32S

namespace icp {

/// \addtogroup datatype DATATYPE
/// @{
/// The DATATYPE typedef aliases _DATATYPE, which defines basic 8, 16, and 32 bit signed and unsigned data types.

typedef enum _DATATYPE
{
   DATATYPE_08U, ///<8-bit unsigned
   DATATYPE_08S, ///<8-bit signed
   DATATYPE_16U, ///<16-bit unsigned
   DATATYPE_16S, ///<16-bit signed
   DATATYPE_32U, ///<32-bit unsigned
   DATATYPE_32S  ///<32-bit signed
} DATATYPE;

/// @}

} /* namespace icp */

#ifdef __cplusplus
extern "C"
#endif
int32_t RetDataTypeSizeInBytes(icp::DATATYPE lDataType);

#endif /* ICPDATATYPE_H */

