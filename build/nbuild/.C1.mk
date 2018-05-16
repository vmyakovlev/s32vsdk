# don't edit, generated from _core.mk
##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2009, Freescale Semiconductor, Inc.
#  Copyright 2017 NXP;
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    (tempate for a) generated make package to compile C1 firmware,
#    typically included by a core and compiler specific make package
#
#  PARAMETERS
#    C1_APP	   	name of the application to generate
#    C1_APP_SRCS	source files of the application
#    C1_APP_LIBS    full paths of libraries linked to the application
#
#    C1_LIB	   	name of the library to generate (instead or in
#			addition to the app
#    C1_LIB_SRCS	source files of the library
#    C1_LIB_LIBS    full paths of libraries required by the libs
#
#
#    C1_INCS      	include paths (beside of the paths of sub-libs), used
#			also in header dependency generation
#    C1_DEFS      	defines (used also in header dependency generation
#
#    C1_LDSCRIPT  	linker script (including switch -xxx if required)
#
#    C1_CCOPTS	additional compiler options (C and C++)
#    C1_CXOPTS	additional compiler options (C++ only)
#    C1_ASOPTS	additional assembler options
#    C1_LDOPTS   	additional linker options
#    C1_AROPTS   	additional archiver options
#
#
#    C1_CC		C compiler
#    C1_CX		C++ compiler
#    C1_AS		assembler
#    C1_LD		linker
#    C1_AR		archiver
#
#    C1_PROMPT	nice string that starts short messages
#
#    C1_OD		optional, default is objdump
#    C1_OC		optional, default is objcopy
#    C1_NM		optional, default is nm
#
#    C1_SRSECS	sections to be extracted when generating an srecord
#			file
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
##############################################################################

##############################################################################
# check required parameters
##############################################################################

ifeq ($(origin SDK_ROOT),undefined)
  $(error error: SDK_ROOT undefined)
endif

# ifeq ($(strip $(C1_APP) $(C1_LIB)),)
  # $(error error: no C1 target (lib/app) set)
# endif

# ifeq ($(strip $(C1_APP_SRCS) $(C1_LIB_SRCS)),)
  # $(error error: no C1 sources set)
# endif

ifeq ($(strip $(C1_CC)),)
  $(error error: C1_CC undefined)
endif

ifeq ($(strip $(C1_CX)),)
  $(error error: C1_CX undefined)
endif

ifeq ($(strip $(C1_AS)),)
  $(error error: C1_AS undefined)
endif

ifeq ($(strip $(C1_LD)),)
  $(error error: C1_LD undefined)
endif

ifeq ($(strip $(C1_AR)),)
  $(error error: C1_AR undefined)
endif

# defaults for some rare customization options
C1_OD ?= objdump
C1_OC ?= objcopy
C1_NM ?= nm
C1_CCOO ?= -o $@
C1_LDOO ?= -o $@
C1_AROO ?= $@
C1_SRSECS ?= .text .data .rodata


##############################################################################

include $(SDK_ROOT)/build/nbuild/helpers.mk

##############################################################################
# phony targets
##############################################################################

.PHONY: clean all pre_all debugC1

all:: pre_all

pre_all:: # just a hook, maybe useful in special applications


##############################################################################
# build the load binary
##############################################################################
ifneq ($(C1_LOAD_BINS),)
_C1_LOAD_OBJS := $(APU_LOAD_OBJS)
_C1_LOAD_HEADERS := $(patsubst %.elf,%.h,$(C1_LOAD_BINS))

# +W bridge options ask the linker (bridge) to generate a map file with Hex addresses
%_APU_LOAD.elf: %.o %__MKDBstub.o $(C1_APP_LIBS)
	$(call nbmsg,"$(C1_PROMPT) LD" $@ " <= " $+) \
	$(C1_LD) $(C1_LDOO) $+ $(C1_LOAD_LDOPTS) $(ERRORFILT) ; \
	echo "------------------------------------------------------------"


%_APU_LOAD.h: %_APU_LOAD.elf
	$(call nbmsg,"$(C1_PROMPT) READELF" $@) \
	$(READELF) -pPMh=32 -mDMb=8 -mVMb=1024 $< -eGb -t $(TCL_SCRIPT) -Tload_name=$(subst _APU_LOAD.elf,,$<) > $@

apu_graphs: $(_C1_LOAD_OBJS)

apu_graphs_link: $(C1_LOAD_BINS) $(_C1_LOAD_HEADERS)

clean::
	$(NBQ)rm -f $(strip $(C1_LOAD_BINS) $(_C1_LOAD_OBJS) $(C1_FCLEAN))
endif

##############################################################################
# build the app
###########################################APU_INCS###################################

ifneq ($(basename $(C1_APP)),)

_C1_APP_OBJS := $(call _src2obj,$(C1_APP_SRCS))

_C1_APP := $(basename $(C1_APP))

$(C1_APP): $(_C1_APP_OBJS) $(C1_APP_LIBS)
	$(call nbmsg,"$(C1_PROMPT) LD" $@ " <= " $+") \
	$(C1_LD) $(C1_LDSCRIPT) $(C1_LDOO) $+ $(C1_LDOPTS) $(ERRORFILT) ; \
	echo "------------------------------------------------------------"

_C1_APP_CLEAN = $(C1_APP) $(_C1_APP_OBJS) $(C1_FCLEAN)

all:: $(C1_APP)

clean::
	$(NBQ)rm -f $(strip $(_C1_APP_CLEAN))

debugC1::
	@echo "C1_APP = $(C1_APP)"
	@echo "_C1_APP_OBJS = $(_C1_APP_OBJS)"

# show all libs
showlibs::
	@echo "---------------------------"
	@echo $(C1_APP)
	@echo "---------------------------"
	@echo "libs with relative pathname for linker cmd"
	@echo "---------------------------"
	@for i in $(C1_APP_LIBS); do echo $$i ; done
	@echo "---------------------------"

endif


##############################################################################
# build the lib
##############################################################################

ifneq ($(C1_LIB),)

_C1_LIB_OBJS := $(call _src2obj,$(C1_LIB_SRCS))

$(C1_LIB): $(_C1_LIB_OBJS)
	$(call nbmsg,"$(C1_PROMPT) AR " $(C1_AROPTS) $(C1_AROO) " <= " $^) \
	rm -f $@ ; \
	$(C1_AR) $(C1_AROPTS) $(C1_AROO) $^ $(ERRORFILT) ; \
	echo "------------------------------------------------------------"

all:: $(C1_LIB)

#### Supress lib removal (make clean L=0)
L ?= 1 
ifneq ($(L),1)
  C1_LIB =
endif

clean::
	$(NBQ)rm -f $(strip $(C1_LIB) $(_C1_LIB_OBJS) $(C1_FCLEAN))

debugC1::
	@echo "C1_LIB = $(C1_LIB)"
	@echo "_C1_LIB_OBJS = $(_C1_LIB_OBJS)"

endif


##############################################################################
# override compile/assemble rules for C1 targets
##############################################################################

# prefer files found locally over VPATH

VPATH := . .. $(VPATH)

# collect all our options into the final compile rules

_C1_LIBINCS :=

_C1_4ALLOPTS := $(C1_DEFS) $(C1_INCS) $(_C1_LIBINCS)

_C1_OBJS := $(_C1_APP_OBJS) $(_C1_LIB_OBJS)

$(_C1_OBJS): COMPILE.c = $(call nbmsg,"$(C1_PROMPT) CC" $@) \
	$(C1_CC) $(C1_CCOPTS) $(_C1_4ALLOPTS)

$(_C1_OBJS): COMPILE.cpp = $(call nbmsg,"$(C1_PROMPT) CX" $@) \
	$(C1_CX) $(C1_CCOPTS) $(C1_CXOPTS) $(_C1_4ALLOPTS)

$(_C1_OBJS): COMPILE.S = $(call nbmsg,"$(C1_PROMPT) AS" $@) \
	$(C1_AS) $(C1_ASOPTS) $(_C1_4ALLOPTS)

$(_C1_OBJS): COMPILE.s = $(call nbmsg,"$(C1_PROMPT) AS" $@) \
	$(C1_AS) $(C1_ASOPTS) $(_C1_4ALLOPTS)

$(_C1_OBJS): COMPILE.cl = $(call nbmsg,"$(C1_PROMPT) CL" $@) \
	$(C1_CC) $(C1_CCOPTS) $(_C1_4ALLOPTS)

$(_C1_OBJS): OUTPUT_OPTION = $(C1_CCOO)

$(_C1_LOAD_OBJS): COMPILE.cpp = $(call nbmsg,"$(C1_PROMPT) CX" $@) \
	$(C1_CX) $(C1_LOAD_CCOPTS)

$(_C1_LOAD_OBJS): $(APU_LOAD_SRCS) $(NBMAKEFILE)
	$(call nbmsg,"$(C1_PROMPT) CX" $@) \
	$(C1_CX) $(C1_LOAD_CCOPTS) $(patsubst %.o,%.cpp,$@) -o $@

# show the variables
debugC1:: 
	@echo "C1_DEFS = $(C1_DEFS)"
	@echo "C1_INCS = $(C1_INCS)"
	@echo "_C1_LIBINCS = $(_C1_LIBINCS)"
	@echo "_C1_4ALLOPTS = $(_C1_4ALLOPTS)"
	@echo "APU_LOAD_SRCS = $(APU_LOAD_SRCS)"
	@echo "_C1_LOAD_OBJS = $(_C1_LOAD_OBJS)"
	@echo "VPATH = $(VPATH)"


##############################################################################
# implicit rules for auxiliary files. not built by default, using Makefile
# must add them to all:: if required
##############################################################################

# disassembly
%_APU_LOAD.dis: %_APU_LOAD.elf
ifneq ($(_C1_APP),)
$(_C1_APP).dis: $(_C1_APP).elf
endif
	@echo disassembling: $< to $@
ifeq ($(APU_COMP),tct)
	$(NBQ)$(C1_OD) -d +Mhex +Ihex +nanno -o$@ $<
else
	$(NBQ)$(C1_OD) -d $< > $@
	$(NBQ)(echo ; echo DATA: ; $(C1_NM) -n $< | \
	grep -i " D "||echo none) >> $@
endif


##############################################################################
# another hook
##############################################################################

all:: post_all

post_all::

##############################################################################
# variables exported to sub.mk (only effective if these packages are used)
##############################################################################

SUB_MAKES := $(SUB_MAKES) $(foreach d,$(C1_LIB_LIBS),$(dir $d))

ifneq ($(_C1_APP),)
SUB_MAKES := $(SUB_MAKES) $(foreach d,$(C1_APP_LIBS),$(dir $d))
endif

SUB_MAKES := $(SUB_MAKES) $(foreach d,$(C1_OTHER_LIBS),$(dir $d))

SUB_MAKES := $(SUB_MAKES) $(foreach d,$(APU_GRAPH_LIBS),$(dir $d))


# include directories
SUB_DIRS := $(SUB_DIRS) $(subst -I,,$(C1_INCS))

# directories of outlying source file
SUB_DIRS := $(SUB_DIRS) $(foreach d,$(filter ../%,\
	$(C1_APP_SRCS) $(C1_LIB_SRCS)),$(dir $d))

##############################################################################
# dependency generation
##############################################################################

# sources for header dependencies
C1_HDEP_SRCS := $(filter-out $(HDEP_IGNORE),$(call uniq,$(C1_APP_SRCS) $(C1_LIB_SRCS)))
C1_HDEP_SUF ?= $(HDEP_AS_SUF) $(HDEP_CC_SUF) $(HDEP_CX_SUF)
C1_HDEP_INC := $(call _src2d,$(C1_HDEP_SRCS),$(C1_HDEP_SUF),C1)

ifneq ($(C1_HDEP_INC),)
ifneq ($(HEADDEP),0)
ifeq ($(filter $(MAKECMDGOALS),$(HDEP_SKIP)),)
ifndef _C1_HEADDEP_MK
_C1_HEADDEP_MK = true

# flags used during header dependency generation (include pathes and defines)
C1_HDEP_FLAGS := -x assembler-with-cpp $(_C1_4ALLOPTS)
C1_HDEP_CC_FLAGS := -x c++ $(_C1_4ALLOPTS)
C1_HDEP_CX_FLAGS := -x c++ $(_C1_4ALLOPTS)
# compiler used during header dependency generation (include pathes and defines)
C1_HDEP_GCC := gcc
C1_HDEP_GCX := g++

# create rules for the header dependencies

# HDEP_AS_SUF
$(foreach suf, $(HDEP_AS_SUF),\
  $(foreach c1src, $(filter %.$(suf),$(C1_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c1src),$(suf),C1,$(C1_HDEP_GCC) $(C1_HDEP_FLAGS),o))))

# HDEP_CC_SUF
$(foreach suf, $(HDEP_CC_SUF),\
  $(foreach c1src, $(filter %.$(suf),$(C1_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c1src),$(suf),C1,$(C1_HDEP_GCC) $(C1_HDEP_CC_FLAGS),o))))

# HDEP_CX_SUF
$(foreach suf, $(HDEP_CX_SUF),\
  $(foreach c1src, $(filter %.$(suf),$(C1_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c1src),$(suf),C1,$(C1_HDEP_GCX) $(C1_HDEP_CX_FLAGS),o))))

# now include the dependencies
-include $(C1_HDEP_INC)
endif # _C1_HEADDEP_MK
endif
endif

#if a .d file cannot be built, don't care and continue
$(C1_HDEP_INC) :

##############################################################################
# rebuild from scratch
##############################################################################

# remove all the .d files
clean::
	$(NBQ)rm -f $(C1_HDEP_INC)

endif
