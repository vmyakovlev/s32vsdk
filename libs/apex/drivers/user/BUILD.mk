##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2014 Freescale Semiconductor;
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

ARM_LIB =      libapexdrv.a

##############################################################################
# libapexdrv.a
##############################################################################

VPATH += $(SDK_ROOT)/libs/apex/drivers/user/src
   
ARM_LIB_SRCS +=                                                              \
    apex.cpp                                                                 \
    apex_interrupt.cpp                                                       \
    apex_interrupt_common.cpp                                                \
    apu_hal.cpp                                                              \
    apumk_hal_cmdque.cpp                                                     \
    apumk_hal_comm.cpp                                                       \
    cmem_if_hal.cpp                                                          \
    global_hal.cpp                                                           \
    linked_list_utilities.cpp                                                \
    mcdma_hal.cpp                                                            \
    mce_hal.cpp                                                              \
    seq_hal.cpp                                                              \
    stio_hal.cpp                                                             \
    hw_reg_user.cpp

ARM_INCS +=                                                                  \
    -I$(SDK_ROOT)/include                                                    \
    -I$(SDK_ROOT)/libs/apex/common/include                                   \
    -I$(SDK_ROOT)/libs/apex/drivers/user/include                             \
    -I$(SDK_ROOT)/libs/apex/register_maps/include                            \
    -I$(SDK_ROOT)/libs/utils/common/include                                  \
    -I$(SDK_ROOT)/libs/utils/oal/user/include                                \
    -I$(SDK_ROOT)/libs/utils/communications/src                              \
    -I$(SDK_ROOT)/libs/apex/icp/include                                      \
    -I$(SDK_ROOT)/libs/apex/acf/include
    
ifneq (,$(findstring ghs-integrity,$(ODIR)))

   VPATH += $(SDK_ROOT)/libs/apex/drivers/user/src/integrity
      
else
ifneq (,$(findstring gnu-linux,$(ODIR)))

   VPATH += $(SDK_ROOT)/libs/apex/drivers/user/src/linux

else
endif
endif

ifneq (,$(findstring sa,$(ODIR)))
    
   VPATH += $(SDK_ROOT)/libs/apex/drivers/user/src/standalone

else
endif

##############################################################################
