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
#    (tempate for a) generated make package to compile C0 firmware,
#    typically included by a core and compiler specific make package
#
#  PARAMETERS
#    C0_APP	   	name of the application to generate
#    C0_APP_SRCS	source files of the application
#    C0_APP_LIBS    full paths of libraries linked to the application
#
#    C0_LIB	   	name of the library to generate (instead or in
#			addition to the app
#    C0_LIB_SRCS	source files of the library
#    C0_LIB_LIBS    full paths of libraries required by the libs
#
#
#    C0_INCS      	include paths (beside of the paths of sub-libs), used
#			also in header dependency generation
#    C0_DEFS      	defines (used also in header dependency generation
#
#    C0_LDSCRIPT  	linker script (including switch -xxx if required)
#
#    C0_CCOPTS	additional compiler options (C and C++)
#    C0_CXOPTS	additional compiler options (C++ only)
#    C0_ASOPTS	additional assembler options
#    C0_LDOPTS   	additional linker options
#    C0_AROPTS   	additional archiver options
#
#
#    C0_CC		C compiler
#    C0_CX		C++ compiler
#    C0_AS		assembler
#    C0_LD		linker
#    C0_AR		archiver
#
#    C0_PROMPT	nice string that starts short messages
#
#    C0_OD		optional, default is objdump
#    C0_OC		optional, default is objcopy
#    C0_NM		optional, default is nm
#
#    C0_SRSECS	sections to be extracted when generating an srecord
#			file
#  AUTHOR
#    Norbert Stoeffler
#    Mihail Marian Nistor
#
#  DATE
#    202017-02-27
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

ifeq ($(strip $(C0_APP) $(C0_LIB)),)
  $(error error: no C0 target (lib/app) set)
endif

ifeq ($(strip $(C0_APP_SRCS) $(C0_LIB_SRCS)),)
  $(error error: no C0 sources set)
endif

ifeq ($(strip $(C0_CC)),)
  $(error error: C0_CC undefined)
endif

ifeq ($(strip $(C0_CX)),)
  $(error error: C0_CX undefined)
endif

ifeq ($(strip $(C0_AS)),)
  $(error error: C0_AS undefined)
endif

ifeq ($(strip $(C0_LD)),)
  $(error error: C0_LD undefined)
endif

ifeq ($(strip $(C0_AR)),)
  $(error error: C0_AR undefined)
endif

# defaults for some rare customization options
C0_OD ?= objdump
C0_OC ?= objcopy
C0_NM ?= nm
C0_CCOO ?= -o $@
C0_LDOO ?= -o $@
C0_AROO ?= $@
C0_SRSECS ?= .text .data .rodata


##############################################################################

include $(SDK_ROOT)/build/nbuild/helpers.mk

##############################################################################
# phony targets
##############################################################################

.PHONY: all clean pre_all debugC0

all:: pre_all

pre_all:: # just a hook, maybe useful in special applications



##############################################################################
# build the app
##############################################################################

ifneq ($(basename $(C0_APP)),)

_C0_APP_OBJS := $(call _src2obj,$(C0_APP_SRCS))

_C0_APP := $(basename $(C0_APP))

$(C0_APP): $(_C0_APP_OBJS) $(C0_APP_LIBS)
	$(call nbmsg,"$(C0_PROMPT) LD " $@ " <= " $+) \
	$(C0_LD) $(C0_LDSCRIPT) $(C0_LDOO) $(C0_LDOPTS_PRE_LIBS) $+ $(C0_LDOPTS_POST_LIBS) \
	$(C0_LDOPTS) $(ERRORFILT) ; \
	echo "------------------------------------------------------------"
ifneq (,$(findstring -sa-,$(ODIR)))
	@aarch64-elf-objcopy -O binary $(_C0_APP).elf $(_C0_APP).bin
endif

_C0_APP_CLEAN = $(C0_APP) $(_C0_APP_OBJS) \
	$(_C0_APP).tsym $(_C0_APP)_sym.h $(_C0_APP).dis $(_C0_APP).srec

all:: $(C0_APP)

clean::
	$(NBQ)rm -f $(strip $(_C0_APP_CLEAN) $(C0_FCLEAN))

debugC0::
	@echo "C0_APP = $(C0_APP)"
	@echo "_C0_APP_OBJS = $(_C0_APP_OBJS)"

# show all libs
showlibs::
	@echo "---------------------------"
	@echo $(C0_APP)
	@echo "---------------------------"
	@echo "libs with relative pathname for linker cmd"
	@echo "---------------------------"
	@for i in $(C0_APP_LIBS); do echo $$i ; done
	@echo "---------------------------"

endif


##############################################################################
# build the lib
##############################################################################

ifneq ($(C0_LIB),)

_C0_LIB_OBJS := $(call _src2obj,$(C0_LIB_SRCS))

$(C0_LIB): $(_C0_LIB_OBJS)
	$(call nbmsg,"$(C0_PROMPT) AR " $(C0_AROPTS) $(C0_AROO) " <= " $^) \
	rm -f $@ ; \
	$(C0_AR) $(C0_AROPTS) $(C0_AROO) $^ $(ERRORFILT) ; \
	echo "------------------------------------------------------------"

all:: $(C0_LIB)

#### Supress lib removal (make clean L=0)
L ?= 1
ifneq ($(L),1)
  C0_LIB =
endif

clean::
	$(NBQ)rm -f $(strip $(C0_LIB) $(_C0_LIB_OBJS) $(C0_FCLEAN))

debugC0::
	@echo "C0_LIB = $(C0_LIB)"
	@echo "_C0_LIB_OBJS = $(_C0_LIB_OBJS)"

endif


##############################################################################
# override compile/assemble rules for C0 targets
##############################################################################

# prefer files found locally over VPATH

VPATH := . .. $(VPATH)

# collect all our options into the final compile rules

_C0_LIBINCS :=

_C0_4ALLOPTS := $(C0_DEFS) $(C0_INCS) $(_C0_LIBINCS)

_C0_OBJS := $(_C0_APP_OBJS) $(_C0_LIB_OBJS)

$(_C0_OBJS): COMPILE.c = $(call nbmsg,"$(C0_PROMPT) CC" $@) \
	$(C0_CC) $(C0_CCOPTS) $(_C0_4ALLOPTS)

$(_C0_OBJS): COMPILE.cpp = $(call nbmsg,"$(C0_PROMPT) CX" $@) \
	$(C0_CX) $(C0_CXOPTS) $(_C0_4ALLOPTS)

$(_C0_OBJS): COMPILE.S = $(call nbmsg,"$(C0_PROMPT) AS" $@) \
	$(C0_AS) $(C0_ASOPTS) $(_C0_4ALLOPTS)

$(_C0_OBJS): COMPILE.s = $(call nbmsg,"$(C0_PROMPT) AS" $@) \
	$(C0_AS) $(C0_ASOPTS) $(_C0_4ALLOPTS)

$(_C0_OBJS): OUTPUT_OPTION = $(C0_CCOO)


# show the variables
debugC0::
	@echo "C0_DEFS = $(C0_DEFS)"
	@echo "C0_INCS = $(C0_INCS)"
	@echo "_C0_LIBINCS = $(_C0_LIBINCS)"
	@echo "_C0_4ALLOPTS = $(_C0_4ALLOPTS)"
	@echo "VPATH = $(VPATH)"


##############################################################################
# implicit rules for auxiliary files. not built by default, using Makefile
# must add them to all:: if required
##############################################################################

ifneq ($(_C0_APP),)
# disassembly
$(_C0_APP).dis: $(_C0_APP).elf
	@echo disassembling: $< to $@
	$(NBQ)$(C0_OD) -S $< > $@
	$(NBQ)(echo ; echo DATA: ; $(C0_NM) -n $< | \
	grep -i " D "||echo none) >> $@

# text symbols for nicer listing of PC regs
$(_C0_APP).tsym: $(_C0_APP).elf
	@echo "generating: $@"
	$(NBQ)$(C0_NM) -n $< | grep -i " t " > $@

# include file with symbols
$(_C0_APP)_sym.h: %_sym.h: %.elf
	@echo "generating: $@"
	@$(C0_NM) -n $< | gawk -v name=$* > $@ \
	'BEGIN{gsub("/","_",name);\
	printf("#ifndef %s_SYM_H\n#define %s_SYM_H\n",name,name);}\
	{ if($$2=="T" || $$2=="D") \
	{gsub("[.]","_",$$3); printf("#define %s_%s 0x%s\n",name,$$3,$$1);}}\
	END{printf("#endif\n");}'

# srecord file
$(_C0_APP).srec: $(_C0_APP).elf
	@echo "generating: $@"
	$(NBQ)$(C0_OC) -O srec \
	  $(addprefix -j ,$(C0_SRSECS)) $< $@
endif # _C0_APP

##############################################################################
# another hook
##############################################################################

all:: post_all

post_all::

##############################################################################
# variables exported to sub.mk (only effective if these packages are used)
##############################################################################

SUB_MAKES := $(SUB_MAKES) $(foreach d,$(C0_LIB_LIBS),$(dir $d))

ifneq ($(_C0_APP),)
SUB_MAKES := $(SUB_MAKES) $(foreach d,$(C0_APP_LIBS),$(dir $d))
endif

# include directories
SUB_DIRS := $(SUB_DIRS) $(subst -I,,$(C0_INCS))

# directories of outlying source file
SUB_DIRS := $(SUB_DIRS) $(foreach d,$(filter ../%,\
	$(C0_APP_SRCS) $(C0_LIB_SRCS)),$(dir $d))

##############################################################################
# dependency generation
##############################################################################

# sources for header dependencies
C0_HDEP_SRCS := $(filter-out $(HDEP_IGNORE),$(call uniq,$(C0_APP_SRCS) $(C0_LIB_SRCS)))
C0_HDEP_SUF ?= $(HDEP_AS_SUF) $(HDEP_CC_SUF) $(HDEP_CX_SUF)
C0_HDEP_INC := $(call _src2d,$(C0_HDEP_SRCS),$(C0_HDEP_SUF),C0)

ifneq ($(C0_HDEP_INC),)
ifneq ($(HEADDEP),0)
ifeq ($(filter $(MAKECMDGOALS),$(HDEP_SKIP)),)
ifndef _C0_HEADDEP_MK
_C0_HEADDEP_MK = true

# flags used during header dependency generation (include pathes and defines)
C0_HDEP_FLAGS := -x assembler-with-cpp $(_C0_4ALLOPTS) $(CROSS_COMPILE_SYSROOT)
C0_HDEP_CC_FLAGS := $(C0_CCOPTS) $(_C0_4ALLOPTS) $(CROSS_COMPILE_SYSROOT)
C0_HDEP_CX_FLAGS := $(C0_CXOPTS) $(_C0_4ALLOPTS) $(CROSS_COMPILE_SYSROOT)
# compiler used during header dependency generation (include pathes and defines)
C0_HDEP_GCC := $(firstword $(C0_CC))
C0_HDEP_GCX := $(firstword $(C0_CX))

# create rules for the header dependencies

# HDEP_AS_SUF
$(foreach suf, $(HDEP_AS_SUF),\
  $(foreach c0src, $(filter %.$(suf),$(C0_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c0src),$(suf),C0,$(C0_HDEP_GCC) $(C0_HDEP_FLAGS),o))))

# HDEP_CC_SUF
$(foreach suf, $(HDEP_CC_SUF),\
  $(foreach c0src, $(filter %.$(suf),$(C0_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c0src),$(suf),C0,$(C0_HDEP_GCC) $(C0_HDEP_CC_FLAGS),o))))

# HDEP_CX_SUF
$(foreach suf, $(HDEP_CX_SUF),\
  $(foreach c0src, $(filter %.$(suf),$(C0_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c0src),$(suf),C0,$(C0_HDEP_GCX) $(C0_HDEP_CX_FLAGS),o))))

# now include the dependencies
-include $(C0_HDEP_INC)
endif # _C0_HEADDEP_MK
endif
endif

#if a .d file cannot be built, don't care and continue
$(C0_HDEP_INC) :

##############################################################################
# rebuild from scratch
##############################################################################

# remove all the .d files
clean::
	$(NBQ)rm -f $(C0_HDEP_INC)
endif
