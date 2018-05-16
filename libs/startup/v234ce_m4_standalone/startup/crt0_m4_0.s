/*========================================================================*
 * COPYRIGHT:                                                             *
 *  Freescale Semiconductor, INC. All Rights Reserved. You are hereby     *
 *  granted a copyright license to use, modify, and distribute the        *
 *  SOFTWARE so long as this entire notice is retained without alteration *
 *  in any modified and/or redistributed versions, and that such modified *
 *  versions are clearly identified as such. No licenses are granted by   *
 *  implication, estoppel or otherwise under any patentsor trademarks     *
 *  of Freescale Semiconductor, Inc. This software is provided on an      *
 *  "AS IS" basis and without warranty.                                   *
 *                                                                        *
 *  To the maximum extent permitted by applicable law, Freescale          *
 *  Semiconductor DISCLAIMS ALL WARRANTIES WHETHER EXPRESS OR IMPLIED,    *
 *  INCLUDING IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A      *
 *  PARTICULAR PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH REGARD  *
 *  TO THE SOFTWARE (INCLUDING ANY MODIFIED VERSIONS THEREOF) AND ANY     *
 *  ACCOMPANYING WRITTEN MATERIALS.                                       *
 *                                                                        *
 *  To the maximum extent permitted by applicable law, IN NO EVENT        *
 *  SHALL Freescale Semiconductor BE LIABLE FOR ANY DAMAGES WHATSOEVER    *
 *  (INCLUDING WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,  *
 *  BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER         *
 *  PECUNIARY LOSS) ARISING OF THE USE OR INABILITY TO USE THE SOFTWARE.  *
 *                                                                        *
 *  Freescale Semiconductor assumes no responsibility for the             *
 *  maintenance and support of this software                              *
 *                                                                        *
 **************************************************************************/


.globl _start_cm4_0
.globl _exit
.globl interrupt_enable
.globl m4_hard_fault_handler
.extern sys_init_test
.extern m4_hard_fault_routine 
.extern nvic_init
.extern __CM4_0_STACK_ADDRESS

.thumb
///////////////////////////////////////////////
// Initialize the GPRs
///////////////////////////////////////////////
_start_cm4_0:
    MOV     r0,#0                   
	MOV     r1,#0
	MOV     r2,#0
	MOV     r3,#0
	MOV     r4,#0
	MOV     r5,#0
	MOV     r6,#0
	MOV     r7,#0
	MOV     r8,r7
	MOV     r9,r7
	MOV     r10,r7
	MOV     r11,r7
	MOV     r12,r7
  
// set stack   
  ldr r0, =__CM4_0_STACK_ADDRESS
  mov sp, r0

// Disable Watchdog        
  MOVW     r0,#0xA010   // swt0_sr (0x4000a010)
  MOVT     r0,#0x4000
	MOVW     r1,#0xC520
	STR      r1, [r0]
	MOVW     r1,#0xD928
	STR      r1, [r0]
	MOVW     r0,#0xA000   // swto_cr (0x4000A000)
	MOVT     r0,#0x4000
	MOVW     r1,#0x0000
	MOVT     r1,#0xFF00
	STR      r1, [r0]
  
  MOVW     r0,#0x6010   //swt4_sr (0x40086010UL)   
  MOVT     r0,#0x4008
	MOVW     r1,#0xC520   //key
	STR      r1, [r0]
	MOVW     r1,#0xD928
	STR      r1, [r0]
	MOVW     r0,#0x6000   //swt4_cr (0x40086000UL)
	MOVT     r0,#0x4008
	MOVW     r1,#0x0000  //swt disable
	MOVT     r1,#0xFF00
	STR      r1, [r0]

	BL 	main

__done:
_exit:
    B   __done

interrupt_enable:
    CPSIE i
    bx    lr

m4_hard_fault_handler:
    mov r0, sp
    push {lr}
    bl m4_hard_fault_routine
    pop {pc}
     
     
