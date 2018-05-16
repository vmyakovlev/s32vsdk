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

include  ../SDK_ROOT_Graph.mk  
KERNELS  := $(SDK_ROOT)/kernels/apu

##############################################################################
# acf kernels and graphs
##############################################################################

APU_GRAPH_INCS += -I.                                                        \
    -I../../                                                                 \
    -I../graphs                                                              \
    -I$(SDK_ROOT)/include                                                    \
    -I$(SDK_ROOT)/kernels/apu/                                               \
	

#APU_GRAPH_LIBS =                                                             \
 #   $(KERNELS)/arithmetic_kernels/build-apu-tct-sa-d/arithmetic_kernels.a
#APU_GRAPHS =                                                                 \
 #   ../apu_add_apu_process_desc.hpp

include ../Build_GraphImport.mk
