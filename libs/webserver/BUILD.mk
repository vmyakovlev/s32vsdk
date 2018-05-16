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

SDK_ROOT :=	../../..

##############################################################################
# ARM_LIB
##############################################################################

ARM_LIB = webserver.a

VPATH += ../src/websocket 

ifneq (,$(findstring -gnu-linux,$(ODIR)))
ARM_LIB_SRCS =                                                               \
    dataframe.cpp                                                            \
    dataframe_parser.cpp                                                     \
    request_handler.cpp                                                      \
    request_parser.cpp                                                       \
    room.cpp                                                                 \
    server.cpp                                                               \
    session.cpp                                                              \
    reply.cpp                                                                \
    webserver.cpp                                                            
    
ARM_INCS =                                                                   \
    -I.                                                                      \
    -I../include                                                             \
    -I$(SDK_ROOT)/include                                                    \
    -I$(SDK_ROOT)/3rdparty/boost_1_62_0                                      
    
endif

ifneq (,$(findstring gnu-sa,$(ODIR)))
ARM_LIB_SRCS =                                                               \
    webserver.cpp        

ARM_INCS =                                                                   \
    -I$(SDK_ROOT)/include                                                    

endif

    
##############################################################################
# STANDALONE SPECIFIC INCLUDES
##############################################################################	
