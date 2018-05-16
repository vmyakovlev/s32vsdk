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

#ifndef VIULITEPUBLIC_H
#define VIULITEPUBLIC_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           viulite_public.h
* @brief          public declarations for viulite user space library
*/

/*==============================================================================
*                            INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/

#include "../libs/isp/viu/user/include/viu_types.h"
#include "../libs/isp/viu/kernel/include/viulite_linux.h"

/*==============================================================================
*                             CONSTANTS
==============================================================================*/


/*==============================================================================
*                          DEFINES AND MACROS
==============================================================================*/


/*==============================================================================
*                              ENUMS
==============================================================================*/
/**
* @brief          Declare enum VIU_LIB_RESULT
* @details        Enum VIU_LIB_RESULT define status viulite lib result.
* @pre
* @post
* @implements     VIU_LIB_RESULT_enumeration
*/
enum VIU_LIB_RESULT
{
  VIU_LIB_SUCCESS = 0,
  VIU_LIB_FAILURE = 1
}; /* VIU_LIB_RESULT */

/*==============================================================================
*                        STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/


/*==============================================================================
*                        GLOBAL VARIABLE DECLARATIONS
==============================================================================*/


/*=============================================================================
 *                       FUNCTION PROTOTYPES
=============================================================================*/

/**
 * @brief          Prepares the VIU driver to be used.
 * @details        In Linux the fsl_viulite0 special device file is opened.
 * @param[in]      aViuIdx index of viu interface to enable.
 * @return         VIU_LIB_SUCCESS if all ok
 *                 VIU_LIB_FAILURE if opening of the device file fails
 */
VIU_LIB_RESULT VIU_Open(const VIU_IDX aViuIdx);


/**
 * @brief          Deinitializes the VIU driver use.
 * @details        In Linux the fsl_viulite0 special device file is closed.
 * @param[in]      aViuIdx index of viu interface to disable.
 * @return         VIU_LIB_SUCCESS if all ok
 *                 VIU_LIB_FAILURE if the close fails
 */
VIU_LIB_RESULT VIU_Close(const VIU_IDX aViuIdx);

/**
 * @brief          Configure VIU interface.
 * @details        All setup at once.
 * @param[in]      aViuIdx index of viu interface to configure.
 * @param[in]      apcViuDmaConfig SRAM buffers related setup
 * @param[in]      apcViuDataInterface signal polling
 * @param[in]      apcViuInputFormat input data format
 * @param[in]      aIrqMask set bit to 1 for IRQ enable. Bits as in VIU RM
                   chapter.
 * @return         VIU_LIB_SUCCESS if all ok
 *                 VIU_LIB_FAILURE if the close fails
 */
VIU_LIB_RESULT VIU_Config(const VIU_IDX aViuIdx,
                          DMA_CONFIG *apcViuDmaConfig,
                          VIU_DATA_INTERFACE *apcViuDataInterface,
                          VIU_INPUT_FORMAT *apcViuInputFormat,
                          const uint8_t aIrqMask
                         );

/**
 * @brief          Configure which IRQs are to be enabled.
 * @details        Irq mask setup.
 * @param[in]      aViuIdx index of viu interface to configure.
 * @param[in]      aIrqMask set bit to 1 for IRQ enable. Bits as in VIU RM
                   chapter.
 * @return         VIU_LIB_SUCCESS if all ok
 *                 VIU_LIB_FAILURE if the close fails
 */
VIU_LIB_RESULT VIU_IrqConfig(const VIU_IDX aViuIdx, const uint8_t aIrqMask);

/**
 * @brief          Start VIU interface.
 * @details
 * @param[in]      aViuIdx index of viu interface to enable.
 * @return         VIU_LIB_SUCCESS if all ok
 *                 VIU_LIB_FAILURE if the close fails
 */
VIU_LIB_RESULT VIU_Start(const VIU_IDX aViuIdx);

/**
 * @brief          Stop VIU interface.
 * @details
 * @param[in]      aViuIdx index of viu interface to enable.
 * @return         VIU_LIB_SUCCESS if all ok
 *                 VIU_LIB_FAILURE if the close fails
 */
VIU_LIB_RESULT VIU_Stop(const VIU_IDX aViuIdx);

/**
 * @brief          Invoke SW reset in VIU HW..
 * @details
 * @param[in]      aViuIdx index of viu interface to enable
 * @return         VIU_LIB_SUCCESS if all ok
 *                 VIU_LIB_FAILURE if the close fails
 */
VIU_LIB_RESULT VIU_SwReset(const VIU_IDX aViuIdx);


/**
 * @brief          Invoke DMA Start.
 * @details
 * @param[in]      aViuIdx index of viu interface to enable.
 * @return         VIU_LIB_SUCCESS if all ok
 *                 VIU_LIB_FAILURE if the close fails
 */
VIU_LIB_RESULT VIU_DmaStart(const VIU_IDX aViuIdx);

/**
 * @brief          Invoke DMA Stop.
 * @details
 * @param[in]      aViuIdx index of viu interface to enable.
 * @return         VIU_LIB_SUCCESS if all ok
 *                 VIU_LIB_FAILURE if the close fails
 */
VIU_LIB_RESULT VIU_DmaStop(const VIU_IDX aViuIdx);


#ifdef __cplusplus
}
#endif

#endif /* VIULITEPUBLIC_H */

/** @} */
