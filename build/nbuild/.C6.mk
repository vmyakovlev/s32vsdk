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
#    (tempate for a) generated make package to compile C6 firmware,
#    typically included by a core and compiler specific make package
#
#  PARAMETERS
#    C6_APP	   	name of the application to generate
#    C6_APP_SRCS	source files of the application
#    C6_APP_LIBS    full paths of libraries linked to the application
#
#    C6_LIB	   	name of the library to generate (instead or in
#			addition to the app
#    C6_LIB_SRCS	source files of the library
#    C6_LIB_LIBS    full paths of libraries required by the libs
#
#
#    C6_INCS      	include paths (beside of the paths of sub-libs), used
#			also in header dependency generation
#    C6_DEFS      	defines (used also in header dependency generation
#
#    C6_LDSCRIPT  	linker script (including switch -xxx if required)
#
#    C6_CCOPTS	additional compiler options (C and C++)
#    C6_CXOPTS	additional compiler options (C++ only)
#    C6_ASOPTS	additional assembler options
#    C6_LDOPTS   	additional linker options
#    C6_AROPTS   	additional archiver options
#
#
#    C6_CC		C compiler
#    C6_CX		C++ compiler
#    C6_AS		assembler
#    C6_LD		linker
#    C6_AR		archiver
#
#    C6_PROMPT	nice string that starts short messages
#
#    C6_OD		optional, default is objdump
#    C6_OC		optional, default is objcopy
#    C6_NM		optional, default is nm
#
#    C6_SRSECS	sections to be extracted when generating an srecord
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

ifeq ($(strip $(C6_APP) $(C6_LIB)),)
  $(error error: no C6 target (lib/app) set)
endif

ifeq ($(strip $(C6_APP_SRCS) $(C6_LIB_SRCS)),)
  $(error error: no C6 sources set)
endif

ifeq ($(strip $(C6_CC)),)
  $(error error: C6_CC undefined)
endif

ifeq ($(strip $(C6_CX)),)
  $(error error: C6_CX undefined)
endif

ifeq ($(strip $(C6_AS)),)
  $(error error: C6_AS undefined)
endif

ifeq ($(strip $(C6_LD)),)
  $(error error: C6_LD undefined)
endif

ifeq ($(strip $(C6_AR)),)
  $(error error: C6_AR undefined)
endif

# defaults for some rare customization options
C6_OD ?= objdump
C6_OC ?= objcopy
C6_NM ?= nm
C6_CCOO ?= -o $@
C6_LDOO ?= -o $@
C6_AROO ?= $@
C6_SRSECS ?= .text .data .rodata .vectable


##############################################################################

include $(SDK_ROOT)/build/nbuild/helpers.mk

##############################################################################
# phony targets
##############################################################################

.PHONY: clean all pre_all debugC6

all:: pre_all

pre_all:: # just a hook, maybe useful in special applications



##############################################################################
# build the app
##############################################################################

ifneq ($(basename $(C6_APP)),)

_C6_APP_OBJS := $(call _src2obj,$(C6_APP_SRCS))

_C6_APP := $(basename $(C6_APP))

$(C6_APP): $(_C6_APP_OBJS) $(C6_APP_LIBS)
	$(call nbmsg,"$(C6_PROMPT) LD " $@ " <= " $+) \
	$(C6_LD) $(C6_LDSCRIPT) $(C6_LDOO) $+ $(C6_LDOPTS) $(ERRORFILT) ; \
	echo "------------------------------------------------------------"

_C6_APP_CLEAN =	$(C6_FCLEAN) $(C6_APP) $(_C6_APP_OBJS) \
	$(_C6_APP).tsym $(_C6_APP)_sym.h $(_C6_APP).dis

all:: $(C6_APP)

clean::
	$(NBQ)rm -f $(strip $(_C6_APP_CLEAN))

debugC6::
	@echo "C6_APP = $(C6_APP)"
	@echo "_C6_APP_OBJS = $(_C6_APP_OBJS)"

# show all libs
showlibs::
	@echo "---------------------------"
	@echo $(C6_APP)
	@echo "---------------------------"
	@echo "libs with relative pathname for linker cmd"
	@echo "---------------------------"
	@for i in $(C6_APP_LIBS); do echo $$i ; done
	@echo "---------------------------"

endif


##############################################################################
# build the lib
##############################################################################

ifneq ($(C6_LIB),)

_C6_LIB_OBJS := $(call _src2obj,$(C6_LIB_SRCS))

$(C6_LIB): $(_C6_LIB_OBJS)
	$(call nbmsg,"$(C6_PROMPT) AR " $(C6_AROPTS) $(C6_AROO) " <= " $^) \
	rm -f $@ ; \
	$(C6_AR) $(C6_AROPTS) $(C6_AROO) $^ $(ERRORFILT) ; \
	echo "------------------------------------------------------------"

all:: $(C6_LIB)

#### Supress lib removal (make clean L=0)
L ?= 1
ifneq ($(L),1)
  C6_LIB =
endif

clean::
	$(NBQ)rm -f $(strip $(C6_LIB) $(_C6_LIB_OBJS) $(C6_FCLEAN))

debugC6::
	@echo "C6_LIB = $(C6_LIB)"
	@echo "_C6_LIB_OBJS = $(_C6_LIB_OBJS)"

endif


##############################################################################
# override compile/assemble rules for C6 targets
##############################################################################

# prefer files found locally over VPATH

VPATH := . .. $(VPATH)

# collect all our options into the final compile rules

_C6_LIBINCS :=

_C6_4ALLOPTS := $(C6_DEFS) $(C6_INCS) $(_C6_LIBINCS)

_C6_OBJS := $(_C6_APP_OBJS) $(_C6_LIB_OBJS)

$(_C6_OBJS): COMPILE.c = $(call nbmsg,"$(C6_PROMPT) CC" $@) \
	$(C6_CC) $(C6_CCOPTS) $(_C6_4ALLOPTS)

$(_C6_OBJS): COMPILE.cpp = $(call nbmsg,"$(C6_PROMPT) CX" $@) \
	$(C6_CX) $(C6_CXOPTS) $(_C6_4ALLOPTS)

$(_C6_OBJS): COMPILE.S = $(call nbmsg,"$(C6_PROMPT) AS" $@) \
	$(C6_AS) $(C6_ASOPTS) $(_C6_4ALLOPTS)

$(_C6_OBJS): COMPILE.s = $(call nbmsg,"$(C6_PROMPT) AS" $@) \
	$(C6_AS) $(C6_ASOPTS) $(_C6_4ALLOPTS)

$(_C6_OBJS): OUTPUT_OPTION = $(C6_CCOO)


# show the variables
debugC6::
	@echo "C6_DEFS = $(C6_DEFS)"
	@echo "C6_INCS = $(C6_INCS)"
	@echo "_C6_LIBINCS = $(_C6_LIBINCS)"
	@echo "_C6_4ALLOPTS = $(_C6_4ALLOPTS)"
	@echo "_C6_APP_OBJS = $(_C6_APP_OBJS)"
	@echo "VPATH = $(VPATH)"


##############################################################################
# implicit rules for auxiliary files. not built by default, using Makefile
# must add them to all:: if required
##############################################################################

ifneq ($(_C6_APP),)
# disassembly
$(_C6_APP).dis: $(_C6_APP).elf
	@echo disassembling: $< to $@
	$(NBQ)$(C6_OD) -S $< > $@
	$(NBQ)(echo ; echo DATA: ; $(C6_NM) -n $< | \
	grep -i " D "||echo none) >> $@

# text symbols for nicer listing of PC regs
$(_C6_APP).tsym: $(_C6_APP).elf
	@echo "generating: $@"
	$(NBQ)$(C6_NM) -n $< | grep -i " t " > $@

# include file with symbols
$(_C6_APP)_sym.h: %_sym.h: %.elf
	@echo "generating: $@"
	@$(C6_NM) -n $< | gawk -v name=$* > $@ \
	'BEGIN{gsub("/","_",name);\
	printf("#ifndef %s_SYM_H\n#define %s_SYM_H\n",name,name);}\
	{ if($$2=="T" || $$2=="D") \
	{gsub("[.]","_",$$3); printf("#define %s_%s 0x%s\n",name,$$3,$$1);}}\
	END{printf("#endif\n");}'

# srecord file
ifeq ($(TOOLCH),ghs)	### GHS ###########################
$(_C6_APP).srec: $(_C6_APP).elf
else	### GCC ###########################################
$(_C6_APP).run: $(_C6_APP).elf
endif
	@echo "generating: $@"
	$(NBQ)$(C6_OC) -O srec --srec-forceS3 \
	  $(addprefix -j ,$(C6_SRSECS)) $< $@
          
# C array from srecord file
ifeq ($(TOOLCH),ghs)	### GHS ###########################
$(_C6_APP).run: $(_C6_APP).elf
endif
$(_C6_APP)_srec.c: $(_C6_APP).run
ifeq ($(TOOLCH),gcc)
	@dos2unix $<
endif
	@echo "generating $(TOOLCH): $@" ; \
	echo "$<" ; \
	echo "/* Sequencer Firmware Srecord */" > $@ ; \
	echo "">> $@ ; \
	echo "char sequencer_srec[] = {\\" >> $@ ; \
	echo "\"\\">> $@ ; \
	awk '{print $$0"\\"}' $< >> $@ ; \
	echo "\"\\">> $@ ; \
	echo "};" >> $@
endif # _C6_APP

##############################################################################
# another hook
##############################################################################

all:: post_all

post_all::

##############################################################################
# variables exported to sub.mk (only effective if these packages are used)
##############################################################################

SUB_MAKES := $(SUB_MAKES) $(foreach d,$(C6_LIB_LIBS),$(dir $d))

ifneq ($(_C6_APP),)
SUB_MAKES := $(SUB_MAKES) $(foreach d,$(C6_APP_LIBS),$(dir $d))
endif

# include directories
SUB_DIRS := $(SUB_DIRS) $(subst -I,,$(C6_INCS))

# directories of outlying source file
SUB_DIRS := $(SUB_DIRS) $(foreach d,$(filter ../%,\
		$(C6_APP_SRCS) $(C6_LIB_SRCS)),$(dir $d))

##############################################################################
# dependency generation
##############################################################################

# sources for header dependencies
C6_HDEP_SRCS := $(filter-out $(HDEP_IGNORE),$(call uniq,$(C6_APP_SRCS) $(C6_LIB_SRCS)))
C6_HDEP_SUF ?= $(HDEP_AS_SUF) $(HDEP_CC_SUF) $(HDEP_CX_SUF)
C6_HDEP_INC := $(call _src2d,$(C6_HDEP_SRCS),$(C6_HDEP_SUF),C6)

ifneq ($(C6_HDEP_INC),)
ifneq ($(HEADDEP),0)
ifeq ($(filter $(MAKECMDGOALS),$(HDEP_SKIP)),)
ifndef _C6_HEADDEP_MK
_C6_HEADDEP_MK = true

# flags used during header dependency generation (include pathes and defines)
C6_HDEP_FLAGS := -x assembler-with-cpp $(_C6_4ALLOPTS)
C6_HDEP_CC_FLAGS := $(C6_CCOPTS) $(_C6_4ALLOPTS)
C6_HDEP_CX_FLAGS := $(C6_CCOPTS) $(C6_CXOPTS) $(_C6_4ALLOPTS)
# compiler used during header dependency generation (include pathes and defines)
C6_HDEP_GCC := $(firstword $(C6_CX))
C6_HDEP_GCX := $(firstword $(C6_CX))

# create rules for the header dependencies

# HDEP_AS_SUF
$(foreach suf, $(HDEP_AS_SUF),\
  $(foreach c6src, $(filter %.$(suf),$(C6_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c6src),$(suf),C6,$(C6_HDEP_GCC) $(C6_HDEP_FLAGS),o))))

# HDEP_CC_SUF
$(foreach suf, $(HDEP_CC_SUF),\
  $(foreach c6src, $(filter %.$(suf),$(C6_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c6src),$(suf),C6,$(C6_HDEP_GCC) $(C6_HDEP_CC_FLAGS),o))))

# HDEP_CX_SUF
$(foreach suf, $(HDEP_CX_SUF),\
  $(foreach c6src, $(filter %.$(suf),$(C6_HDEP_SRCS)),\
    $(eval $(call hdep_rules,$(c6src),$(suf),C6,$(C6_HDEP_GCX) $(C6_HDEP_CX_FLAGS),o))))

# now include the dependencies
-include $(C6_HDEP_INC)
endif # _C6_HEADDEP_MK
endif 
endif

#if a .d file cannot be built, don't care and continue
$(C6_HDEP_INC) :

##############################################################################
# rebuild from scratch
##############################################################################

# remove all the .d files
clean::
	$(NBQ)rm -f $(C6_HDEP_INC)
endif
