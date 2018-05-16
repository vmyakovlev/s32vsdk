/*****************************************************************************
* 
* NXP Confidential Proprietary
*
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
#ifndef APEXINTERRUPT_H
#define APEXINTERRUPT_H

#ifdef __cplusplus 
extern "C" { 
#endif

//Interrupts
enum
{
   SEQUENCER_RES_30 = 0, //ACF uses to check for 'done' associated with APU0
   SEQUENCER_RES_31,     //ACF uses to check for 'done' associated with APU1
   NUM_INTERRUPTS
};

//This struct exists to capture interrupt state information (e.g. ISR, IMR), as required.
//e.g. the kernel will initialize an instance of this struct with the captured ISR state for the seq, mcdma, stio, and mce HW blocks,
//     and relay it to the user-space handler whenever the APEX global interrupt line asserts
typedef struct _APEX_INTERRUPTS
{
   uint32_t seq;   //sequencer interrupt state
   uint32_t mcdma; //multi-channel DMA interrupt state
   uint32_t mce;   //motion comp DMA interrupt state
} APEX_INTERRUPTS;

int32_t APEX_InterruptInit(void);
int32_t APEX_InterruptDeinit(void);
int32_t APEX_InterruptRegisterCallback(int32_t lApexId,
                                       int32_t lInterruptId,
                                       void (*lpCallback)(void* lpParam, int32_t* lpRetVal),
                                       void* lpParam,
                                       int32_t* lpRetVal);
int32_t APEX_InterruptClearCallback(int32_t lApexId, int32_t lInterruptId);

//internal use
void APEX_InterruptSignalHandlerCommon(int32_t lApexId, APEX_INTERRUPTS lIsrState);

#ifdef __cplusplus 
} 
#endif 

#endif /* APEXINTERRUPT_H */
