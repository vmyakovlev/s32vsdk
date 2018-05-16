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

ARM_LIB = h264encoder.a


##############################################################################
# ARM_APP
##############################################################################

ARM_LIB_SRCS += h264encoder.cpp


ARM_INCS +=                                                                  \
    -I$(SDK_ROOT)/include                                                    \
    -I$(SDK_ROOT)/platform/s32_v234                                          \
    -I$(SDK_ROOT)/isp/inc/                                                   \
    -I$(SDK_ROOT)/3rdparty/boost_1_62_0
                                        

ARM_APP_LIBS =                                                               \
    $(SDK_ROOT)/libs/isp/h264enc/user/$(ODIR)/libh264encdrv.a                \
    $(SDK_ROOT)/libs/utils/log/$(ODIR)/liblog.a                              \
    $(SDK_ROOT)/libs/utils/oal/user/$(ODIR)/liboal.a                         \

ARM_LDOPTS +=                                                                \
    -lavcodec                                                                \
    -lavformat                                                               \
    -lavutil                                                                 \
    -lswresample                                                             \
    -lswscale
	
##############################################################################
