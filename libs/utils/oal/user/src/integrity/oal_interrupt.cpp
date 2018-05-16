/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
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
/**
* \file oal_interrupt.cpp
* \brief OAL interrupt handling functionality
* \author Rostislav Hulik
* \version
* \date 3-March-2016
****************************************************************************/

#include <oal.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////
/// Initialize an interrupt and add a service routine to it.
////////////////////////////////////////////////////////////////////////
int32_t OAL_IRQInit(void (*/*pServiceRoutineFunc*/)(int), unsigned int /*irq*/, unsigned int /*priority*/, IRQ_MODE /*mode*/)
{
  printf("OAL IRQ Functionality not available in INTEGRITY. Please refer to concrete device driver\n");
  return OAL_FAILURE;
}

////////////////////////////////////////////////////////////////////////
/// Enables the specified interrupt.
////////////////////////////////////////////////////////////////////////
void OAL_IRQEnable(unsigned int /*irq*/)
{
  printf("OAL IRQ Functionality not available in INTEGRITY. Please refer to concrete device driver\n");
}

////////////////////////////////////////////////////////////////////////
/// Deinitialize an interrupt and removes the service routine link.
////////////////////////////////////////////////////////////////////////
int32_t OAL_IRQDelete(unsigned int /*irq*/)
{
  printf("OAL IRQ Functionality not available in INTEGRITY. Please refer to concrete device driver\n");
  return OAL_FAILURE;
}

////////////////////////////////////////////////////////////////////////
/// Disables the specified interrupt.
////////////////////////////////////////////////////////////////////////
void OAL_IRQDisable(unsigned int /*irq*/)
{
  printf("OAL IRQ Functionality not available in INTEGRITY. Please refer to concrete device driver\n");
}
