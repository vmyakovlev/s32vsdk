##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2015 Freescale Semiconductor;
# All Rights Reserved
#
##############################################################################
#
# THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
##############################################################################

SDK_ROOT :=    ../../../../..

ARM_LIB =      libh264encdrv.a

# library sources
##############################################################################
# libh264encdrv.a
##############################################################################

VPATH += $(SDK_ROOT)/libs/isp/h264enc/user/src

ARM_LIB_SRCS +=                                                              \
    h264enc_user.cpp     

ifneq (,$(findstring gnu-linux,$(ODIR)))
                                                        
else

ifneq (,$(findstring gnu-sa,$(ODIR)))
    VPATH += $(SDK_ROOT)/libs/isp/h264enc/kernel/src
    
    ARM_LIB_SRCS +=                                                          \
        h264enc_user.c                                                        
    
endif

  ARM_INCS +=                                                                \
    -I$(SDK_ROOT)/platform/s32_v234                                          \
        
endif

ARM_INCS +=                                                                 \
    -I$(SDK_ROOT)/isp/inc/                                                  \
    -I$(SDK_ROOT)/include/                                                  \
    -I$(SDK_ROOT)/platform/s32_v234 

##############################################################################
