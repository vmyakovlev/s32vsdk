##############################################################################
#
#  Copyright (c) 2006, Freescale Semiconductor. All Rights Reserved.
#  Copyright 2017 NXP;
#  Freescale Confidential Proprietary
#
##############################################################################
#
#  CONTENT
#    centralize treatment of some platform dependent stuff
#
#  AUTHOR
#    Norbert Stoeffler
#    Mihail Marian Nistor
#
#  DATE
#    2017-02-27
#
#  LANGUAGE
#    make
#
#
##############################################################################

ifndef _PLATFORM_MK
_PLATFORM_MK =	true

SHELL = /bin/bash

.SECONDARY:

include $(CURR_SDK_ROOT)/build/nbuild/gmsl.mk
##############################################################################
# determine platform and give it a simple name
##############################################################################

ifeq ($(origin APU_COMP), undefined)
APU_COMP := tct
export APU_COMP
endif

# get the last part of the current working directory
ODIR := $(notdir $(patsubst %/,%,$(CURDIR)))

# enable to exit with error
ifeq ($(origin ERRORFILT), undefined)
ERRORFILT := || { exit 1; }
export ERRORFILT
endif

# global parameters for dependency files
ifeq ($(origin HEADDEP), undefined)
HEADDEP := 1
export HEADDEP
endif

ifeq ($(origin HDEP_SKIP), undefined)
HDEP_SKIP := clean cleansub allsub
export HDEP_SKIP
endif

ifeq ($(origin HDEP_IGNORE_ERROR), undefined)
HDEP_IGNORE_ERROR := 1
export HDEP_IGNORE_ERROR
endif

# Assembler code that must be preprocessed
ifeq ($(origin HDEP_AS_SUF), undefined)
HDEP_AS_SUF := S
export HDEP_AS_SUF
endif

# C source code that must be preprocessed
ifeq ($(origin HDEP_CC_SUF), undefined)
HDEP_CC_SUF := c C
export HDEP_CC_SUF
endif

# C++ source code that must be preprocessed
ifeq ($(origin HDEP_CX_SUF), undefined)
HDEP_CX_SUF := cpp cc
export HDEP_CX_SUF
endif

NBMAKEFILE := Makefile ../BUILD.mk

ifeq ($(origin SYS), undefined)
SYS := $(shell echo $$OSTYPE)
export SYS
endif

ifeq ($(origin NBUILD_PLATFORM), undefined)
ifeq ($(SYS),)
  NBUILD_PLATFORM := linux
else
ifneq ($(findstring solaris,$(SYS)),)
  NBUILD_PLATFORM := solaris
else
ifneq ($(findstring linux,$(SYS)),)
  NBUILD_PLATFORM := linux
else
ifneq ($(findstring INTEGRITY,$(SYS)),)
  NBUILD_PLATFORM := integrity
else
  NBUILD_PLATFORM := msys
endif
endif
endif
endif
export NBUILD_PLATFORM
endif

ifeq ($(origin BITS), undefined)
  BITS := $(shell arch)
  export BITS
endif

ifeq ($(origin HNAME), undefined)
HNAME := $(shell echo $$HOSTNAME)
export HNAME
endif

# bitwidth of the (linux) system
ifeq ($(origin NBUILD_PLATBITS), undefined)
ifeq ($(BITS),x86_64)
  NBUILD_PLATBITS =	64
else
  NBUILD_PLATBITS =	32
endif
export NBUILD_PLATBITS
endif

ifeq ($(origin OS_STYLE_CURR_SDK_ROOT), undefined)
ifeq ($(NBUILD_PLATFORM),msys)
  OS_STYLE_CURR_SDK_ROOT := $(shell cygpath -w $(CURR_SDK_ROOT))
ifeq ($(OS_STYLE_CURR_SDK_ROOT),)
$(error The cygpath cannot convert the current sdk root from Unix-style to Windows-style)
endif
else
  OS_STYLE_CURR_SDK_ROOT := $(CURR_SDK_ROOT)
endif
export OS_STYLE_CURR_SDK_ROOT
endif

all:: # protect debugplat

debugplat:
	@echo OSTYPE = $(SYS)
	@echo HOSTNAME = $(HNAME)
	@echo HOSTTYPE = $(BITS)
	@echo NBUILD_PLATFORM = $(NBUILD_PLATFORM)
	@echo NBUILD_PLATBITS = $(NBUILD_PLATBITS)


##############################################################################
# some assertions
##############################################################################

_EMPTY :=
_SPACE :=	$(_EMPTY) $(_EMPTY)

ifeq ($(findstring $(_SPACE),$(CURDIR)),$(_SPACE))
  $(error error: spaces in directory names (here: $(CURDIR))\
    not supported)
endif


ifeq ($(origin XLIBPATH), undefined)
ifeq ($(NBUILD_PLATBITS),64)
XLIBPATH = /usr/X11R6/lib64
else
XLIBPATH = /usr/X11R6/lib
endif
export XLIBPATH
endif

##############################################################################
# executable file name extension
##############################################################################
ifeq ($(origin EXE_EXT), undefined)
ifeq ($(NBUILD_PLATFORM),msys)
EXE_EXT = .exe
endif
ifeq ($(NBUILD_PLATFORM),linux)
EXE_EXT =
endif
export EXE_EXT
endif

##############################################################################
# verbosity
##############################################################################
V ?= 0
ifeq ($(V),0)
ifeq ($(origin NBQ), undefined)
NBQ := @
export NBQ
endif
endif
nbmsg = $(NBQ)echo "  "$(1) ;
endif
