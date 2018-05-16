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
#    (tempate for a) generated make package to compile C2 firmware,
#    typically included by a core and compiler specific make package
#
#  PARAMETERS
#    C2_APP	   	name of the application to generate
#    C2_APP_SRCS	source files of the application
#    C2_APP_LIBS    full paths of libraries linked to the application
#
#    C2_LIB	   	name of the library to generate (instead or in
#			addition to the app
#    C2_LIB_SRCS	source files of the library
#    C2_LIB_LIBS    full paths of libraries required by the libs
#
#
#    C2_INCS      	include paths (beside of the paths of sub-libs), used
#			also in header dependency generation
#    C2_DEFS      	defines (used also in header dependency generation
#
#    C2_LDSCRIPT  	linker script (including switch -xxx if required)
#
#    C2_CCOPTS	additional compiler options (C and C++)
#    C2_CXOPTS	additional compiler options (C++ only)
#    C2_ASOPTS	additional assembler options
#    C2_LDOPTS   	additional linker options
#    C2_AROPTS   	additional archiver options
#
#
#    C2_CC		C compiler
#    C2_CX		C++ compiler
#    C2_AS		assembler
#    C2_LD		linker
#    C2_AR		archiver
#
#    C2_PROMPT	nice string that starts short messages
#
#    C2_OD		optional, default is objdump
#    C2_OC		optional, default is objcopy
#    C2_NM		optional, default is nm
#
#    C2_SRSECS	sections to be extracted when generating an srecord
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

ifeq ($(strip $(C2_APP) $(C2_LIB)),)
  #$(warning warning: no C2 target (lib/app) set)
endif

# ifeq ($(strip $(C2_APP_SRCS) $(C2_LIB_SRCS)),)
  # $(error error: no C2 sources set)
# endif

ifeq ($(strip $(C2_CC)),)
  $(error error: C2_CC undefined)
endif

ifeq ($(strip $(C2_CX)),)
  $(error error: C2_CX undefined)
endif

ifeq ($(strip $(C2_AS)),)
  $(error error: C2_AS undefined)
endif

ifeq ($(strip $(C2_LD)),)
  $(error error: C2_LD undefined)
endif

ifeq ($(strip $(C2_AR)),)
  $(error error: C2_AR undefined)
endif

# defaults for some rare customization options
C2_OD ?= objdump
C2_OC ?= objcopy
C2_NM ?= nm
C2_CCOO ?= -o $@
C2_LDOO ?= -o $@
C2_AROO ?= $@
C2_SRSECS ?= .text .data .rodata


##############################################################################

include $(SDK_ROOT)/build/nbuild/helpers.mk

##############################################################################
# phony targets
##############################################################################

.PHONY:	clean all pre_all debugC2

all:: pre_all

pre_all:: # just a hook, maybe useful in special applications


##############################################################################
# build the app (and run it, to create *_db.h )
##############################################################################

ifneq ($(basename $(C2_APP)),)

_C2_APP_OBJS := $(call _src2obj2,$(C2_APP_SRCS))

_C2_APP = $(basename $(C2_APP))

$(C2_APP): $(_C2_APP_OBJS) $(C2_APP_LIBS)
	$(call nbmsg,"$(C2_PROMPT) LD " $@ " <= " $+) \
	$(C2_LD) $(C2_LDSCRIPT) $(C2_LDOO) $+ $(C2_LDOPTS) $(ERRORFILT) ; \
	echo "------------------------------------------------------------"

_C2_APP_CLEAN = $(C2_APP) $(_C2_APP_OBJS) \
		$(_C2_APP).tsym $(_C2_APP)_sym.h $(_C2_APP).dis $(C2_FCLEAN)

all:: $(C2_APP)

clean::
	$(NBQ)rm -f $(strip $(_C2_APP_CLEAN))

debugC2::
	@echo "C2_APP = $(C2_APP)"
	@echo "_C2_APP_OBJS = $(_C2_APP_OBJS)"

# show all libs
showlibs::
	@echo "---------------------------"
	@echo $(C2_APP)
	@echo "---------------------------"
	@echo "libs with relative pathname for linker cmd"
	@echo "---------------------------"
	@for i in $(C2_APP_LIBS); do echo $$i ; done
	@echo "---------------------------"

endif


##############################################################################
# build the lib
##############################################################################

ifneq ($(C2_LIB),)

_C2_LIB_OBJS := $(call _src2obj2,$(C2_LIB_SRCS))

$(C2_LIB): $(_C2_LIB_OBJS)
	$(call nbmsg,"$(C2_PROMPT) AR " $(C2_AROPTS) $(C2_AROO) " <= " $^) \
	rm -f $@ ; \
	$(C2_AR) $(C2_AROPTS) $(C2_AROO) $^ $(ERRORFILT) ; \
	echo "------------------------------------------------------------"

all:: $(C2_LIB)

#### Supress lib removal (make clean L=0)
L ?= 1
ifneq ($(L),1)
  C2_LIB =
endif

clean::
	$(NBQ)rm -f $(strip $(C2_LIB) $(_C2_LIB_OBJS) $(C2_FCLEAN))

debugC2::
	@echo "C2_LIB = $(C2_LIB)"
	@echo "_C2_LIB_OBJS = $(_C2_LIB_OBJS)"

endif


##############################################################################
# override compile/assemble rules for C2 targets
##############################################################################

# prefer files found locally over VPATH

VPATH := . .. $(VPATH)

# collect all our options into the final compile rules

_C2_LIBINCS :=

_C2_4ALLOPTS := $(C2_DEFS) $(C2_INCS) $(_C2_LIBINCS)

_C2_OBJS := $(_C2_APP_OBJS) $(_C2_LIB_OBJS)

$(_C2_OBJS): COMPILE.c = $(call nbmsg,"$(C2_PROMPT) CC" $@) \
	$(C2_CC) $(C2_CCOPTS) $(_C2_4ALLOPTS)

$(_C2_OBJS): COMPILE.cpp = $(call nbmsg,"$(C2_PROMPT) CX" $@) \
	$(C2_CX) $(C2_CCOPTS) $(C2_CXOPTS) $(_C2_4ALLOPTS)

$(_C2_OBJS): COMPILE.S = $(call nbmsg,"$(C2_PROMPT) AS" $@) \
	$(C2_AS) $(C2_ASOPTS) $(_C2_4ALLOPTS)

$(_C2_OBJS): COMPILE.s = $(call nbmsg,"$(C2_PROMPT) AS" $@) \
	$(C2_AS) $(C2_ASOPTS) $(_C2_4ALLOPTS)

$(_C2_OBJS): OUTPUT_OPTION = $(C2_CCOO)


# show the variables
debugC2::
	@echo "C2_DEFS = $(C2_DEFS)"
	@echo "C2_INCS = $(C2_INCS)"
	@echo "_C2_LIBINCS = $(_C2_LIBINCS)"
	@echo "_C2_4ALLOPTS = $(_C2_4ALLOPTS)"
	@echo "_C2_APP_OBJS = $(_C2_APP_OBJS)"
	@echo "VPATH = $(VPATH)"


##############################################################################
# implicit rules for auxiliary files. not built by default, using Makefile
# must add them to all:: if required
##############################################################################

ifneq ($(_C2_APP),)
# disassembly
$(_C2_APP).dis: $(_C2_APP).elf
	@echo disassembling: $< to $@
	$(NBQ)$(C2_OD) -S $< > $@
	$(NBQ)(echo ; echo DATA: ; $(C2_NM) -n $< | \
	grep -i " D "||echo none) >> $@

# text symbols for nicer listing of PC regs
$(_C2_APP).tsym: $(_C2_APP).elf
	@echo "generating: $@"
	$(NBQ)$(C2_NM) -n $< | grep -i " t " > $@

# include file with symbols
$(_C2_APP)_sym.h: %_sym.h: %.elf
	@echo "generating: $@"
	@$(C2_NM) -n $< | gawk -v name=$* > $@ \
	'BEGIN{gsub("/","_",name);\
	printf("#ifndef %s_SYM_H\n#define %s_SYM_H\n",name,name);}\
	{ if($$2=="T" || $$2=="D") \
	{gsub("[.]","_",$$3); printf("#define %s_%s 0x%s\n",name,$$3,$$1);}}\
	END{printf("#endif\n");}'

# srecord file
$(_C2_APP).srec: $(_C2_APP).elf
	@echo "generating: $@"
	$(NBQ)$(C2_OC) -O srec \
	  $(addprefix -j ,$(C2_SRSECS)) $< $@
endif # _C2_APP

##############################################################################
# another hook
##############################################################################

all:: post_all

post_all::

##############################################################################
# variables exported to sub.mk (only effective if these packages are used)
##############################################################################

SUB_MAKES := $(SUB_MAKES) $(foreach d,$(C2_LIB_LIBS),$(dir $d))

ifneq ($(_C2_APP),)
SUB_MAKES := $(SUB_MAKES) $(foreach d,$(C2_APP_LIBS),$(dir $d))
endif

# include directories
SUB_DIRS := $(SUB_DIRS) $(subst -I,,$(C2_INCS))

# directories of outlying source file
SUB_DIRS := $(SUB_DIRS) $(foreach d,$(filter ../%,\
		$(C2_APP_SRCS) $(C2_LIB_SRCS)),$(dir $d))

##############################################################################
# dependency generation
##############################################################################

# sources for header dependencies
C2_HDEP_SRCS := $(filter-out $(HDEP_IGNORE),$(call uniq,$(C2_APP_SRCS) $(C2_LIB_SRCS)))
C2_HDEP_SUF ?= $(HDEP_AS_SUF) $(HDEP_CC_SUF) $(HDEP_CX_SUF)
C2_HDEP_INC := $(call _src2d,$(C2_HDEP_SRCS),$(C2_HDEP_SUF),C2)

ifneq ($(C2_HDEP_INC),)
ifneq ($(HEADDEP),0)
ifeq ($(filter $(MAKECMDGOALS),$(HDEP_SKIP)),)
ifndef _C2_HEADDEP_MK
_C2_HEADDEP_MK = true

# flags used during header dependency generation (include pathes and defines)
C2_HDEP_FLAGS := -x assembler-with-cpp $(_C2_4ALLOPTS)
C2_HDEP_CC_FLAGS := $(C2_CCOPTS) $(_C2_4ALLOPTS)
C2_HDEP_CX_FLAGS := $(C2_CCOPTS) $(C2_CXOPTS) $(_C2_4ALLOPTS)
# compiler used during header dependency generation (include pathes and defines)
C2_HDEP_GCC :=  $(firstword $(C2_CC))
C2_HDEP_GCX :=  $(firstword $(C2_CC))

# create rules for the header dependencies

# HDEP_AS_SUF
$(foreach suf, $(HDEP_AS_SUF),\
  $(foreach c2src, $(filter %.$(suf),$(C2_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c2src),$(suf),C2,$(C2_HDEP_GCC) $(C2_HDEP_FLAGS),obj))))

# HDEP_CC_SUF
$(foreach suf, $(HDEP_CC_SUF),\
  $(foreach c2src, $(filter %.$(suf),$(C2_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c2src),$(suf),C2,$(C2_HDEP_GCC) $(C2_HDEP_CC_FLAGS),obj))))

# HDEP_CX_SUF
$(foreach suf, $(HDEP_CX_SUF),\
  $(foreach c2src, $(filter %.$(suf),$(C2_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c2src),$(suf),C2,$(C2_HDEP_GCX) $(C2_HDEP_CX_FLAGS),obj))))

# now include the dependencies
-include $(C2_HDEP_INC)
endif # _C2_HEADDEP_MK
endif
endif 

#if a .d file cannot be built, don't care and continue
$(C2_HDEP_INC) :

##############################################################################
# rebuild from scratch
##############################################################################

# remove all the .d files
clean::
	$(NBQ)rm -f $(C2_HDEP_INC)
endif
