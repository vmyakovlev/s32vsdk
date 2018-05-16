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

SDK_ROOT := ../../../../..
ARM_LIB =   liboal.a

##############################################################################
# liboal.a
##############################################################################
ifneq (,$(findstring ghs-integrity,$(ODIR)))
    
    VPATH += $(SDK_ROOT)/libs/utils/oal/user/src/integrity
    
    ARM_LIB_SRCS +=                                                          \
        oal_semaphore.cpp                                                    \
        oal_interrupt.cpp                                                    \
        oal_task.cpp                                                         \
        oal_memory_driver_if.cpp                                             \
        oal_memory.cpp 
        
else

ifneq (,$(findstring sa,$(ODIR)))
    
    VPATH += $(SDK_ROOT)/libs/utils/oal/user/src/standalone
    VPATH += $(SDK_ROOT)/libs/utils/oal/common/src
    
    ARM_INCS += -I$(SDK_ROOT)/libs/utils/oal/common/include
    
    ARM_LIB_SRCS +=                                                          \
        oal_allocator.c                                                      \
        oal_cma_list.c                                                       \
        oal_allocation_kernel.c                                              \
        oal_memory_driver_if.cpp                                             \
        oal_memory.cpp                                                       \
        oal_interrupt.cpp                                                    \
        oal_semaphore.cpp                                                    \
        oal_task.cpp
        
else
    VPATH +=	$(SDK_ROOT)/libs/utils/oal/user/src/linux
    
    ARM_LIB_SRCS +=                                                          \
        oal_memory.cpp                                                       \
        oal_memory_driver_if.cpp                                             \
        oal_interrupt.cpp                                                    \
        oal_semaphore.cpp                                                    \
        oal_task.cpp
        
endif
endif

ARM_INCS +=                                                                  \
    -I$(SDK_ROOT)/include                                \
    -I$(SDK_ROOT)/libs/utils/oal/user/include                              \
    -I$(SDK_ROOT)/libs/utils/oal/kernel/include                              \
    -I$(SDK_ROOT)/libs/utils/common/include

##############################################################################
