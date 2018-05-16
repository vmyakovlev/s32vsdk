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
* \file     h264enc_public.h
* \brief    public declarations for h264enc user space library
* \author   Tomas Babinec
* \version  0.1
* \date     03-November-2014
* \note
****************************************************************************/

#ifndef H264ENCPUBLIC_H
#define H264ENCPUBLIC_H

#include "../libs/isp/h264enc/kernel/include/h264enc.h"
#include "../libs/isp/h264enc/kernel/include/h264enc_types.h"
#include "../libs/isp/h264enc/kernel/include/h264enc_ioctl.h"

//*****************************************************************************
// types
//*****************************************************************************

enum H264ENC_LIB_RESULT
{
  H264ENC_LIB_SUCCESS = 0,
  H264ENC_LIB_FAILURE = 1
}; // H264ENC_LIB_RESULT

///< Pointer to a user function handling Sequencer event
typedef void(*H264ENC_EventHandler_t)(uint32_t aEventType);

class H264ENC_EventHandlerClass
{
public:
  virtual void H264EncEventHandler(uint32_t aEventType) = 0;
}; // H264ENC_EventHandlerClass

//*****************************************************************************
// public functionality declarations
//*****************************************************************************

/****************************************************************************/
/** Resets H264Enc registers to default values.
* 
* \return H264ENC_LIB_SUCCESS if all ok
*     H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_SwReset(void);

/****************************************************************************/
/** Reserves the H264 Encoder usage for this process.
* Opens the /dev/h264enc file - only one process at the time is allowed to do this.
* 
* \return H264ENC_LIB_SUCCESS if all ok
* 	  H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_Reserve(void);

/****************************************************************************/
/** Releases the H264 Encoder usage which was locked to this process.
* Closes the /dev/h264enc file.
* 
*****************************************************************************/
void H264ENC_Release(void);

/****************************************************************************/
/** Passes encoding configuration to the driver.
* 
* \param arcConfig structure defining encoding parameter values
* 
* \return H264ENC_LIB_SUCCESS if all ok
*         H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_Configure(const H264ENC_Config_t &arcConfig);

/****************************************************************************/
/** Sets up buffer addresses
* 
* \param arcBuffers structure with buffer descriptions
* 
* \return H264ENC_LIB_SUCCESS if all ok
*         H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_BuffersSet(const H264ENC_Buffers_t &arcBuffers);

/****************************************************************************/
/** Informs encoder HW more input lines are ready.
*   Invokes encoding start. 
* 
* \param aMoreLines number of lines added to the input buffer
* 
* \return H264ENC_LIB_SUCCESS if all ok
* 	  H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_LinesFetch(uint32_t aMoreLines);

/****************************************************************************/
/** Shifts bitstream buffer alarm address.
* 
* \param aAlarmShift number of bytes to shift the alarm address
* 
* \return H264ENC_LIB_SUCCESS if all ok
*     H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_AlarmShift(uint32_t aAlarmShift);

/****************************************************************************/
/**  
* 
* \param arStatus[out] encoder status description
* 
* \return H264ENC_LIB_SUCCESS if all ok
*         H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_StatusGet(H264ENC_Status_t &arStatus);

/****************************************************************************/
/** Requests a graceful stop of the currently executed encoding.
* 
* 
* \return H264ENC_LIB_SUCCESS if all ok
* 	  H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_Stop();


/****************************************************************************/
/** Requests a graceful stop of the currently executed encoding.
* 
* 
* \return H264ENC_LIB_SUCCESS if all ok
* 	  H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_ResetVars();

/****************************************************************************/
/** Register event handler.
* NULL is a valid parameter. Removes the previous handler.
* 
* \param aHandler user defined Sequencer event handling function
* 
* \return H264ENC_LIB_SUCCESS if all ok
* 	  H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_EventHandlerSet(H264ENC_EventHandler_t aHandler);

/****************************************************************************/
/** Register event handler object.
* NULL is a valid parameter. Removes the previous handler.
* 
* \param arHandler user defined Sequencer event handling object
* 
* \return H264ENC_LIB_SUCCESS if all ok
* 	  H264ENC_LIB_FAILURE if opening of the device file fails
*****************************************************************************/
H264ENC_LIB_RESULT H264ENC_EventHandlerSet(H264ENC_EventHandlerClass *apHandler);


#endif /* H264ENCPUBLIC_H */

