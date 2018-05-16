/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
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

#ifndef SYMMETRYFLAGS_H
#define SYMMETRYFLAGS_H

// Bit flags for the different possible shapes of a filter
#define SYMMFLAG(symX, symY, antiSymX, antiSymY) (((symX) << 3) | ((symY) << 2) | ((antiSymX) << 1) | (antiSymY))
#define NO_SYM                   SYMMFLAG(0,0,0,0) // 0x0
#define ANTI_SYM_Y               SYMMFLAG(0,0,0,1) // 0x1
#define ANTI_SYM_X               SYMMFLAG(0,0,1,0) // 0x2
#define ANTI_SYM_XY              SYMMFLAG(0,0,1,1) // 0x3
#define SYM_Y                    SYMMFLAG(0,1,0,0) // 0x4
#define ANTI_SYM_X_SYM_Y SYMMFLAG(0,1,1,0) // 0x6
#define SYM_X                    SYMMFLAG(1,0,0,0) // 0x8
#define SYM_X_ANTI_SYM_Y SYMMFLAG(1,0,0,1) // 0x9
#define SYM_XY                   SYMMFLAG(1,1,0,0) // 0xc

#endif /* SYMMETRYFLAGS_H */