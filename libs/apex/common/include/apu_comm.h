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

#ifndef APUCOMM_H
#define APUCOMM_H

#define FIFO_SIZE_LOG2 6
#define FIFO_SIZE      (1 << FIFO_SIZE_LOG2)

#define APU_MAX_NUMBER_OF_APUS 4

// *****************************************
#define _PROC_TARGET_ARM 0
#define _PROC_TARGET_APU 1

#define _PROC_TARGET _PROC_TARGET_ARM

#if (_PROC_TARGET == _PROC_TARGET_APU)

#define TX_FIFO_ID 0
#define RX_FIFO_ID 1

#else

#define TX_FIFO_ID 1
#define RX_FIFO_ID 0

#endif
// *****************************************



struct _FIFO
{
   unsigned int d[FIFO_SIZE];
   unsigned int r;
   unsigned int w;
};

struct _APU_COMM
{
   // From the point of view of the APUs
   // Tx : APU Writes
   // Rx : APU Reads
   // -------------------------------------------
   struct _FIFO FIFO[2];
};

typedef struct _APU_COMM APU_COMM;

#endif /* APUCOMM_H */

