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

ARM_LIB = libgdi.a

##############################################################################
# libgdi.a
##############################################################################

ifneq ($(findstring sa,$(ODIR)),)

ARM_LIB_SRCS +=                                                              \
    gdi_image.cpp                                                            \
    gdi_mat.cpp                                                              \
    gdi_utils.cpp                                                            
else 
ARM_LIB_SRCS +=                                                              \
    gdi_image.cpp                                                            \
    gdi_utils.cpp                                                            \
    
endif

ARM_INCS +=  \
    -I$(OPENCV_ROOT)/include                                                 \
    -I$(SDK_ROOT)/libs/apex/common/include                                   \
    -I$(SDK_ROOT)/include                                                    \
    -I$(SDK_ROOT)/libs/io/gdi/include                                        \
    -I$(SDK_ROOT)/libs/io/framebuffer/user/include/linux

ifneq ($(findstring -vp,$(ODIR)),)
    ARM_DEFS += -DVLAB
endif

##############################################################################
# gdi_srv
##############################################################################

ifneq ($(findstring sa,$(ODIR)),)

else

ARM_APP_SRCS += gdi_srv.cpp

ARM_APP_LIBS =                                                               \
    $(SDK_ROOT)/libs/io/gdi/$(ODIR)/libgdi.a                                 \
    $(SDK_ROOT)/libs/utils/log/$(ODIR)/liblog.a                              \
    $(SDK_ROOT)/libs/utils/oal/user/$(ODIR)/liboal.a

ARM_INCS += -I$(OPENCV_ROOT)/include
 
ARM_LDOPTS +=                                                                \
    -lopencv_core                                                            \
    -lopencv_imgproc                                                         \
    -lopencv_highgui \
    -lavcodec                                                                \
    -lavdevice                                                               \
    -lavfilter                                                               \
    -lavformat                                                               \
    -lavutil                                                                 \
    -lswresample                                                             \
    -lswscale

ifneq ($(findstring -linux,$(ODIR)),)
ARM_LDOPTS +=                                                                \
    -lavcodec                                                                \
    -lavdevice                                                               \
    -lavfilter                                                               \
    -lavformat                                                               \
    -lavutil                                                                 \
    -lswresample                                                             \
    -lswscale
endif
endif
##############################################################################
