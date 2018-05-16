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

SDK_ROOT :=  ../../../..

ARM_LIB =    libv234ce.a

##############################################################################
# libacf2host.a
##############################################################################

ARM_LIB_SRCS +=                                                              \
    entry.s                                                                  \
    app_header.c                                                             \
    exceptions.c                                                             \
    startup_linaro.s                                                         \
    exceptions_table_linaro.s                                                \
    mmap.c                                                                   \
    gic400.c                                                                 \
    startup.c                                                                \
    pll.c                                                                    \
    HDMI.c

ARM_INCS +=                                                                  \
    -I../include                                                             \
    -I$(SDK_ROOT)/include                                                    \
    -I$(SDK_ROOT)/platform/s32_v234                                          \

##############################################################################
