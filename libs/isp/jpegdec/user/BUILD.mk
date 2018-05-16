##############################################################################
#
# NXP Confidential Proprietary
#
# Copyright (c) 2014-16 Freescale Semiconductor
# Copyright 2017 NXP
# All Rights Reserved
#
##############################################################################
#
# THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
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

ARM_LIB =    libjpegdecdrv.a

# library sources
##############################################################################
# libjpegdecdrv.a
##############################################################################

ARM_LIB_SRCS +=                                                             \
        jpegdec_user.cpp                                                    \

ifneq (,$(findstring v234ce,$(ODIR)))
  ARM_INCS +=                                                               \
    -I$(SDK_ROOT)/platform/s32_v234                                         \

else
  ARM_INCS +=                                                               \
    -I$(SDK_ROOT)/platform/fpga_vs4                                         \

endif

ifneq (,$(findstring linux,$(ODIR)))
  ARM_INCS +=								                                \
    -I$(SDK_ROOT)/libs/isp/jpegdec/kernel/linux/include                     \

endif

ARM_INCS +=                                                                 \
    -I$(SDK_ROOT)/isp/inc                                                   \
    -I$(SDK_ROOT)/include                                                   \
    -I$(SDK_ROOT)/libs/isp/jpegdec/kernel/common/include                    \

##############################################################################
