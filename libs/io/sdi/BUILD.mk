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

SDK_ROOT := ../../../..

ARM_LIB =   libsdi.a

##############################################################################
# libsdi.a
##############################################################################
ARM_LIB_SRCS +=                                                              \
    sdi_graph.cpp                                                            \
    sdi_io.cpp                                                               \
    sdi.cpp 

ARM_INCS +=                                                                  \
    -I$(SDK_ROOT)/include                                                    \
    -I$(SDK_ROOT)/libs/utils/oal/user/include                                \
    -I$(SDK_ROOT)/libs/io/sdi/include                                        \
    -I$(SDK_ROOT)/libs/isp/cam_generic/kernel/include                        \
    -I$(SDK_ROOT)/libs/isp/csi/kernel/include                                \
    -I$(SDK_ROOT)/libs/isp/sequencer/kernel/include                          \
    -I$(SDK_ROOT)/libs/isp/fdma/kernel/include                               \
    -I$(SDK_ROOT)/isp/inc                                                    \
    -I$(SDK_ROOT)/libs/apex/common/include                                   \
    -I$(SDK_ROOT)/libs/isp/viu/user/include                                  
                
ifneq ($(findstring deskx11,$(ODIR)),)
ARM_LIB_SRCS +=                                                              \
    sdi_io.cpp                                                               \
    sdi.cpp     
endif                
                
ifneq (,$(findstring v234ce-gnu-sa-d,$(ODIR)))
ARM_INCS +=                                                                  \
    -I$(SDK_ROOT)/platform/s32_v234                                     
else 

ARM_INCS +=                                                                  \
    -I$(SDK_ROOT)/platform/s32_v234                              
endif                
                
ARM_INCS += -I$(OPENCV_ROOT)/include
 
ARM_LDOPTS +=                                                                \
    -lopencv_core                                                            \
    -lopencv_imgproc                                                         \
    -lopencv_highgui

##############################################################################
