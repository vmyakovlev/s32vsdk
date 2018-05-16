##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2011, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    ACF Build Makefile for generated Kernels
#
#  AUTHOR
#    Anca Dima 
#
#  DATE
#    2015-03-26
#
#  LANGUAGE
#    make
#
##############################################################################

#SDK_ROOT := ../../../../../../s32v234_sdk
include ../../SDK_ROOT_Kernel.mk  

#APU_LIB = someCategory.a

##############################################################################
# APU Kernel library built (code for APU - kernel implementation)
##############################################################################

APU_INCS +=                                                                  \
    -I./                                                                     \
    -I$(SDK_ROOT)/include
	
include ../Build_KernelImport.mk

APU_DEFS += -DACF_KERNEL_IMPLEMENTATION

X86_INCS += -I$(SDK_ROOT)/include

##############################################################################
# Kernel db generator - builds an exe application and generates .h header with
#                       kernel description
# Do not change following code
##############################################################################


X86_LIB 	 =	$(addsuffix acf.a,$(basename $(APU_LIB)))
X86_LIB_SRCS =	$(APU_LIB_SRCS)

X86_KERNEL_DB = "on"

##############################################################################

