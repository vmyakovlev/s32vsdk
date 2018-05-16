##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2011, Freescale Semiconductor, Inc.
#  Copyright 2017 NXP;
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#
#
#  AUTHOR
#    Stefan Schwarz
#    Mihail Marian Nistor
#
#  DATE
#    2017-02-27
#
#  LANGUAGE
#    make
#
##############################################################################
OS_TYPE :=
READELF := read_elf
TCL_SCRIPT := $(SDK_ROOT)/build/nbuild/scripts/gen_apu_load_cl.tcl

ifeq ($(APU_COMP),tct)
############ Target Chess ####################################################
C1_APP := $(ACL_APP).elf
C1_APP_SRCS := $(ACL_APP_SRCS)
C1_APP_LIBS := $(ACL_APP_LIBS)

C1_LIB := $(ACL_LIB)
C1_LIB_SRCS := $(ACL_LIB_SRCS)
C1_LIB_LIBS := $(ACL_LIB_LIBS)

C1_DEFS := $(ACL_DEFS)
C1_INCS := $(ACL_INCS)

C1_DEFS +=
C1_INCS +=

# compiler specific options for platform, not in headdep
C1_CCOPTS += $(ACL_CCOPTS)		
C1_CXOPTS += $(ACL_CCOPTS)
C1_ASOPTS +=
C1_LDOPTS += $(ACL_LDOPTS)

########## Load generation settings
C1_LOAD_BINS := $(ACL_LOAD_BINS)
C1_LOAD_OBJS := $(ACL_LOAD_OBJS)
C1_LOAD_SRCS := $(ACL_LOAD_SRCS)
C1_LOAD_INCS := $(ACL_GRAPH_INCS)
C1_LOAD_LDOPTS := $(ACL_LOAD_LDOPTS)
C1_LOAD_CCOPTS = $(ACL_CCOPTS)

############ Auto Build (load_generation) ####################################

##############################################################################
# nbuild packages

C1_CC := chesscc -c +p4
C1_CX := chesscc -c +p4
C1_AS := chesscc -c +p4
C1_LD := chesscc
C1_AR := chesscc
C1_OD := darts

C1_PROMPT := apu/chess

C1_CCOPTS +=

C1_AROPTS += -a -o
C1_CXOPTS +=
C1_ASOPTS +=

C1_LDOPTS += 
C1_FCLEAN := *.h *.log *.a

include $(SDK_ROOT)/build/nbuild/.C1.mk
include $(SDK_ROOT)/build/nbuild/sub.mk

############ TARGET compiler #################################################
else
#echo "llvm apu-acl-sa-tct\n"
############ Target LLVM ####################################################
C1_APP := $(ACL_APP).elf
C1_APP_SRCS := $(ACL_APP_SRCS)
C1_APP_LIBS := $(ACL_APP_LIBS)

C1_LIB := $(ACL_LIB)
C1_LIB_SRCS := $(ACL_LIB_SRCS)
C1_LIB_LIBS := $(ACL_LIB_LIBS)

C1_DEFS := $(ACL_DEFS)
C1_INCS := $(ACL_INCS)

C1_DEFS +=
C1_INCS +=

# compiler specific options for platform, not in headdep
C1_CCOPTS += $(ACL_CCOPTS)
C1_CXOPTS += $(ACL_CCOPTS)
C1_ASOPTS +=
C1_LDOPTS += $(ACL_LDOPTS)

########## Load generation settings
C1_LOAD_BINS := $(ACL_LOAD_BINS)
C1_LOAD_OBJS := $(ACL_LOAD_OBJS)
C1_LOAD_SRCS := $(ACL_LOAD_SRCS)
C1_LOAD_INCS := $(ACL_GRAPH_INCS)
C1_LOAD_LDOPTS = $(ACL_LOAD_LDOPTS)
C1_LOAD_CCOPTS = \
  $(ACL_CCOPTS) \
  -x c++

############ Auto Build (load_generation) ####################################

##############################################################################
# nbuild packages

C1_CC := clang -O3 -c 
C1_CX := clang -O3 -c
C1_AS := clang -c
C1_LD := clang
C1_AR := ar
C1_OD := $(NXP_CMP_PATH)/llvm-objdump -arch-name=apex

C1_PROMPT := apu/clang

C1_CCOPTS += -x c++

C1_AROPTS += -r
C1_CXOPTS +=
C1_ASOPTS +=

C1_LDOPTS +=
C1_FCLEAN := *.h *.log *.a *.dis

include $(SDK_ROOT)/build/nbuild/.C1.mk

############ LLVM compiler #################################################
endif
