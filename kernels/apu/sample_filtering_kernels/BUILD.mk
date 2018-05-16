##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2016 Freescale Semiconductor;
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

SDK_ROOT := ../../../..

APU_LIB = filtering_kernels.a

##############################################################################
# APU Kernel library built (code for APU - kernel implementation)
##############################################################################

APU_INCS +=                                                                  \
    -I./                                                                     \
    -I$(SDK_ROOT)/include

APU_LIB_SRCS +=                                                         \
    filter_a_acf.cpp                                                    \
    filter_a_apu.cpp                                                    \
    filtering_sobel_3x3_acf.cpp                                         \
    filtering_sobel_3x3_apu.cpp                                         \
    gauss_3x1_acf.cpp                                                   \
    gauss_3x1_apu.cpp                                                   \
    gauss_3x3_acf.cpp                                                   \
    gauss_3x3_apu.cpp                                                   \
    gauss_5x5_acf.cpp                                                   \
    gauss_5x5_apu.cpp                                                   \
    gradient_acf.cpp                                                    \
    gradient_apu.cpp                                                    \
    nms_acf.cpp                                                         \
    nms_apu.cpp                                                         \
    saturate_nonzero_acf.cpp                                            \
    saturate_nonzero_apu.cpp                                            \
    col_filter_a_acf.cpp                                                \
    col_filter_a_apu.cpp                                                \
    row_filter_a_acf.cpp                                                \
    row_filter_a_apu.cpp                                                \
    correlation_acf.cpp                                                 \
    correlation_apu.cpp                                                 \
    filter_median_3x3_acf.cpp                                           \
    filter_median_3x3_apu.cpp                                          

APU_DEFS += -DACF_KERNEL_IMPLEMENTATION

X86_INCS += -I$(SDK_ROOT)/include

ifeq (,$(findstring -nxp,$(ODIR)))
  APU_CCOPTS += +W noodle,-Wnone
  APU_CXOPTS += +W noodle,-Wnone
endif

##############################################################################
# Kernel db generator - builds an exe application and generates .h header with
#                       kernel description
# Do not change following code
##############################################################################

X86_LIB 	 =	$(addsuffix acf.a,$(basename $(APU_LIB)))
X86_LIB_SRCS =	$(APU_LIB_SRCS)
X86_KERNEL_DB = "on"

##############################################################################

