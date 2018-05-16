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

APU_LIB = arithmetic_kernels.a

X86_LIB =	$(addsuffix acf.a,$(basename $(APU_LIB)))
X86_LIB_SRCS =	$(APU_LIB_SRCS)
##############################################################################
# APU Kernel library built (code for APU - kernel implementation)
##############################################################################

APU_INCS +=                                                                  \
    -I./                                                                     \
    -I$(SDK_ROOT)/include

APU_LIB_SRCS +=                                                              \
    add_acf.cpp                                                              \
    add_apu.cpp                                                         \
    max_acf.cpp                                                              \
    max_apu.cpp                                                         \
    difference_acf.cpp                                                \
    difference_apu.cpp                                           \
    dot_multiplic_acf.cpp                                             \
    dot_multiplic_apu.cpp                                        \
    dot_division_acf.cpp                                              \
    dot_division_apu.cpp                                         \
    dot_sqr_acf.cpp                                                   \
    dot_sqr_apu.cpp

APU_DEFS += -DACF_KERNEL_IMPLEMENTATION

X86_INCS += -I$(SDK_ROOT)/include

##############################################################################
# Kernel db generator - builds an exe application and generates .h header with
#                       kernel description
# Do not change following code
##############################################################################

##############################################################################
# APU Kernel library built (code for ACF - kernel metadata)
##############################################################################

X86_LIB 	 =	$(addsuffix acf.a,$(basename $(APU_LIB)))
X86_LIB_SRCS =	$(APU_LIB_SRCS)

X86_KERNEL_DB = "on"

##############################################################################

