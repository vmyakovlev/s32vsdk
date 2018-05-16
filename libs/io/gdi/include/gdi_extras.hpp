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
/**
 * \file 		gdi_extras.hpp
 * \brief 	Added gdi user interface functionality declarations.
 * \author 	Tomas Babinec
 * \version 	<0.1>
 * \date 		Aug 22, 2013
 ****************************************************************************/

#ifndef GDIEXTRAS_HPP
#define GDIEXTRAS_HPP

#include "vdb_log.h"

//  gdi currently specifies 7 (one entry empty) GDI_DISPLAY_IDs in enum GDI_DISPLAY_ID
//  followed by ALL and INVALID enum values.
#define GDI_DISPLAY_ID_NUM 7

/*****************************************************************************
* macros
*****************************************************************************/

#define GDI_LOG_LIB_RESULT(_command, _lres) \
{\
  LIB_RESULT _tmp_lres = _command;\
  if( _tmp_lres == LIB_SUCCESS)\
  {\
    VDB_LOG_NOTE(#_command " returned: LIB_SUCCESS.\n");\
  }else\
  {\
    (_lres) = LIB_FAILURE;\
    if(_tmp_lres == LIB_FAILURE)\
    {\
      VDB_LOG_ERROR(#_command " returned: LIB_FAILURE.\n");\
    }else\
    {\
      VDB_LOG_ERROR(#_command " returned: unknown result.\n");\
    }\
  }\
}
/*****************************************************************************
 * functions (prototypes)
 *****************************************************************************/

#endif /* GDIEXTRAS_HPP */
