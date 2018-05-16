##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2015 Freescale Semiconductor;
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
#
#  CONTENT
#    sony_y ISP graph. Includes build of relevant kernels
#
#  AUTHOR
#    Armin Strobl
#
#  DATE
#    2015-05-12
#
#  LANGUAGE
#    make
#
##############################################################################

SDK_ROOT :=	../../../../../s32v234_sdk

##############################################################################
# ISP Graph
##############################################################################
ARM_LIB =	max9286_96705_4.a

ARM_LIB_SRCS =	\
		max9286_96705_4.c

ARM_INCS =	\
		-I../inc							\
		-I$(SDK_ROOT)/isp/inc						\
		-I$(SDK_ROOT)/platform/s32_v234					\

GDT_OPTS =	-m

##############################################################################
# IPUx kernels 
##############################################################################
IPUS_APP = ipus.elf
IPUS_APP_SRCS = \
  ../../../kernels/max9286_96705_4/src/copy_1to1_ipus.ipus \

KMEM = kmem
