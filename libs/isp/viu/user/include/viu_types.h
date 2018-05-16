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

/*=============================================================================
Revision History:
                        Modification       Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Tomas Babinec           20-Apr-2016                   Init version
Xuyen Dao               05-Aug-2016        VSDK-251   Update template
Cristian Tomescu        17-Oct-2016        VSDK-270   Adding the user space drv
Cristian Tomescu        15-Nov-2016        VSDK-495   Remove the 'fsl'
Cristian Tomescu        08-May-2017        VSDK-560   Fix MISRA violations
=============================================================================*/
#ifndef VIUTYPES_H
#define VIUTYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           viu_types.h
* @brief          types declarations for VIU receiver driver
*/

/*==============================================================================
*                               INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#if !defined(__KERNEL__) && !defined(__STANDALONE__)
#include <stdint.h>

#endif // if !defined(__KERNEL__) && !defined(__STANDALONE__)

/*==============================================================================
*                               CONSTANTS
==============================================================================*/


/*==============================================================================
*                        DEFINES AND MACROS
==============================================================================*/


/*==============================================================================
*                              ENUMS
==============================================================================*/

/**
* @brief          Declare enum VIU_IDX
* @details        Declare enum VIU_IDX
* @pre
* @post
* @implements     VIU_IDX_enumeration
*/
enum VIU_IDX
{
  VIU_IDX_0 = 0,
  VIU_IDX_1 = 1,
  VIU_IDX_INVALID = 2
}; // VIU index definition

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/


/*==============================================================================
*                        GLOBAL VARIABLE DECLARATIONS
==============================================================================*/


/*==============================================================================
*                           FUNCTION PROTOTYPES
==============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* VIUTYPES_H */

/** @} */