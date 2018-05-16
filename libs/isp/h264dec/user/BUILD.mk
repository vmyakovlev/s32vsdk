##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2014-16 Freescale Semiconductor;
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

SDK_ROOT :=  ../../../../..

ARM_LIB =    libh264decdrv.a

# library sources
##############################################################################
# libfdmadrv.a
##############################################################################                            

ARM_LIB_SRCS +=                                                             \
        h264dec_user.cpp                                                    \

ifneq (,$(findstring v234ce,$(ODIR)))
  ARM_INCS +=                                                               \
    -I$(SDK_ROOT)/platform/s32_v234                                         \
    
else
  ARM_INCS +=                                                               \
    -I$(SDK_ROOT)/platform/fpga_vs4                                         \
    
endif

ARM_INCS +=                                                                 \
    -I$(SDK_ROOT)/isp/inc                                                   \
    -I$(SDK_ROOT)/include                                                   \

##############################################################################
