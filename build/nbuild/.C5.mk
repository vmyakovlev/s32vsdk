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
#    (tempate for a) generated make package to assembly C5 firmware,
#    typically included by a core and assembly specific make package
#
#  PARAMETERS
#    C5_APP name of the application to generate
#    C5_APP_SRCS source files of the application
#
#    C5_INCS include paths (beside of the paths of sub-libs)
#    C5_DEFS defines
#
#    C5_ASOPTS additional assembler options
#
#    C5_AS assembler
#	 C5_SR2ROM srecord to rom utility
#
#    C5_PROMPT nice string that starts short messages
#
#    C5_OD optional, default is objdump
#    C5_OC optional, default is objcopy
#    C5_NM optional, default is nm
#
#    C5_SRSECS sections to be extracted when generating an srecord file
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
##############################################################################

##############################################################################
# check required parameters
##############################################################################

ifeq ($(origin SDK_ROOT),undefined)
  $(error error: SDK_ROOT undefined)
endif

ifeq ($(strip $(C5_APP)),)
  $(error error: no C5 target app set)
endif

ifeq ($(strip $(C5_APP_SRCS)),)
  $(error error: no C5 sources set)
endif

ifeq ($(strip $(C5_AS)),)
  $(error error: C5_AS undefined)
endif

# defaults for some rare customization options
C5_OD ?= objdump
C5_OC ?= objcopy
C5_NM ?= nm
C5_SRSECS ?= .text .data .rodata
C5_SR2ROM ?= sr2rom


##############################################################################

include $(SDK_ROOT)/build/nbuild/helpers.mk

##############################################################################
# phony targets
##############################################################################

.PHONY: clean all pre_all debugC5

all:: pre_all

pre_all:: # just a hook, maybe useful in special applications



##############################################################################
# build the app
##############################################################################

ifneq ($(basename $(C5_APP)),)

_C5_APP := $(basename $(C5_APP))

$(C5_APP): $(C5_APP_SRCS)
	$(call nbmsg,"$(C5_PROMPT) AS" $@ " <= " $(C5_APP_SRCS)) \
	$(C5_AS) $(C5_ASOPTS) $(C5_DEFS) $(C5_INCS) $(C5_APP_SRCS) -o $(C5_APP) $(ERRORFILT) ; \
	echo "------------------------------------------------------------"

_C5_APP_CLEAN =	$(C5_APP) \
		$(_C5_APP).tsym $(_C5_APP)_sym.h $(_C5_APP).dis $(_C5_APP)_kmem.c $(_C5_APP).srec $(C5_FCLEAN)

all:: $(C5_APP) #$(_C5_APP).dis $(_C5_APP)_kmem.c $(_C5_APP)_sym.h

clean::
	$(NBQ)rm -f $(strip $(_C5_APP_CLEAN))

debugC5::
	@echo "C5_APP = $(C5_APP)"
	@echo "C5_APP_SRCS = $(C5_APP_SRCS)"

endif


##############################################################################
# override compile/assemble rules for C5 targets
##############################################################################

# prefer files found locally over VPATH

VPATH := . .. $(VPATH)

# show the variables
debugC5::
	@echo "C5_DEFS = $(C5_DEFS)"
	@echo "C5_INCS = $(C5_INCS)"
	@echo "VPATH = $(VPATH)"


##############################################################################
# implicit rules for auxiliary files. not built by default, using Makefile
# must add them to all:: if required
##############################################################################

ifneq ($(_C5_APP),)
# disassembly
$(_C5_APP).dis: $(_C5_APP).elf
	@echo disassembling: $< to $@
	$(NBQ)$(C5_OD) -S $< > $@
	$(NBQ)(echo ; echo DATA: ; $(C5_NM) -n $< | \
	grep -i " D "||echo none) >> $@

# text symbols for nicer listing of PC regs
$(_C5_APP).tsym: $(_C5_APP).elf
	@echo "generating: $@"
	$(NBQ)$(C5_NM) -n $< | grep -i " t " > $@

# include file with symbols
$(_C5_APP)_sym.h: %_sym.h: %.elf
	@echo "generating: $@"
	@$(C5_NM) -n $< | gawk -v name=$* > $@ \
	'BEGIN{gsub("/","_",name);\
	printf("#ifndef %s_SYM_H\n#define %s_SYM_H\n",name,name);}\
	{ if($$2=="T" || $$2=="D") \
	{gsub("[.]","_",$$3); printf("#define %s_%s 0x%s\n",name,$$3,$$1);}}\
	END{printf("#endif\n");}'

# srecord file
$(_C5_APP).srec: $(_C5_APP).elf
	@echo "generating: $@"
	$(NBQ)$(C5_OC) -O srec \
	  $(addprefix -j ,$(C5_SRSECS)) $< $@

# memory dump of srecord file
$(_C5_APP)_kmem.c: %_kmem.c: $(_C5_APP).srec
	@echo "generating: $@"
	@$(C5_SR2ROM) -w 8 -p none -f hex -q $< $@
endif # _C5_APP

##############################################################################
# another hook
##############################################################################

all:: post_all

post_all::

##############################################################################
# variables exported to sub.mk (only effective if these packages are used)
##############################################################################

# include directories
SUB_DIRS := $(SUB_DIRS) $(subst -I,,$(C5_INCS))

# directories of outlying source file
SUB_DIRS := $(SUB_DIRS) $(foreach d,$(filter ../%,\
		$(C5_APP_SRCS)),$(dir $d))

##############################################################################
# C5 (as-IPUV) support assembler code that should not be preprocessed.
##############################################################################
