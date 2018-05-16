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
#    (tempate for a) generated make package to assembly C4 firmware,
#    typically included by a core and assembly specific make package
#
#  PARAMETERS
#    C4_APP name of the application to generate
#    C4_APP_SRCS source files of the application
#
#    C4_INCS include paths (beside of the paths of sub-libs)
#    C4_DEFS defines
#
#    C4_ASOPTS additional assembler options
#
#    C4_AS assembler
#	 C4_SR2ROM srecord to rom utility
#
#    C4_PROMPT nice string that starts short messages
#
#    C4_OD optional, default is objdump
#    C4_OC optional, default is objcopy
#    C4_NM optional, default is nm
#
#    C4_SRSECS sections to be extracted when generating an srecord file
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

ifeq ($(strip $(C4_APP)),)
  $(error error: no C4 target app set)
endif

ifeq ($(strip $(C4_APP_SRCS)),)
  $(error error: no C4 sources set)
endif

ifeq ($(strip $(C4_AS)),)
  $(error error: C4_AS undefined)
endif

# defaults for some rare customization options
C4_OD ?= objdump
C4_OC ?= objcopy
C4_NM ?= nm
C4_SRSECS ?= .text .data .rodata
C4_SR2ROM ?= sr2rom


##############################################################################

include $(SDK_ROOT)/build/nbuild/helpers.mk

##############################################################################
# phony targets
##############################################################################

.PHONY: clean all pre_all debugC4

all:: pre_all

pre_all:: # just a hook, maybe useful in special applications



##############################################################################
# build the app
##############################################################################

ifneq ($(basename $(C4_APP)),)

_C4_APP := $(basename $(C4_APP))

$(C4_APP): $(C4_APP_SRCS)
	$(call nbmsg,"$(C4_PROMPT) AS" $@ " <= " $(C4_APP_SRCS)) \
	$(C4_AS) $(C4_ASOPTS) $(C4_DEFS) $(C4_INCS) $(C4_APP_SRCS) -o $(C4_APP) $(ERRORFILT) ; \
	echo "------------------------------------------------------------"

_C4_APP_CLEAN =	$(C4_APP) \
		$(_C4_APP).tsym $(_C4_APP)_sym.h $(_C4_APP).dis $(_C4_APP)_kmem.c $(_C4_APP).srec $(C4_FCLEAN)

all:: $(C4_APP) #$(_C4_APP).dis $(_C4_APP)_kmem.c $(_C4_APP)_sym.h

clean::
	$(NBQ)rm -f $(strip $(_C4_APP_CLEAN))

debugC4::
	@echo "C4_APP = $(C4_APP)"
	@echo "C4_APP_SRCS = $(C4_APP_SRCS)"

endif


##############################################################################
# override compile/assemble rules for C4 targets
##############################################################################

# prefer files found locally over VPATH

VPATH := . .. $(VPATH)

# show the variables
debugC4::
	@echo "C4_DEFS = $(C4_DEFS)"
	@echo "C4_INCS = $(C4_INCS)"
	@echo "VPATH = $(VPATH)"


##############################################################################
# implicit rules for auxiliary files. not built by default, using Makefile
# must add them to all:: if required
##############################################################################

ifneq ($(_C4_APP),)
# disassembly
$(_C4_APP).dis: $(_C4_APP).elf
	@echo disassembling: $< to $@
	$(NBQ)$(C4_OD) -S $< > $@
	$(NBQ)(echo ; echo DATA: ; $(C4_NM) -n $< | \
	grep -i " D "||echo none) >> $@

# text symbols for nicer listing of PC regs
$(_C4_APP).tsym: $(_C4_APP).elf
	@echo "generating: $@"
	$(NBQ)$(C4_NM) -n $< | grep -i " t " > $@

# include file with symbols
$(_C4_APP)_sym.h: %_sym.h: %.elf
	@echo "generating: $@"
	@$(C4_NM) -n $< | gawk -v name=$* > $@ \
	'BEGIN{gsub("/","_",name);\
	printf("#ifndef %s_SYM_H\n#define %s_SYM_H\n",name,name);}\
	{ if($$2=="T" || $$2=="D") \
	{gsub("[.]","_",$$3); printf("#define %s_%s 0x%s\n",name,$$3,$$1);}}\
	END{printf("#endif\n");}'

# srecord file
$(_C4_APP).srec: $(_C4_APP).elf
	@echo "generating: $@"
	$(NBQ)$(C4_OC) -O srec \
	  $(addprefix -j ,$(C4_SRSECS)) $< $@

# memory dump of srecord file
$(_C4_APP)_kmem.c: %_kmem.c: $(_C4_APP).srec
	@echo "generating: $@"
	@$(C4_SR2ROM) -w 8 -p none -f hex -q $< $@
endif # _C4_APP

##############################################################################
# another hook
##############################################################################

all:: post_all

post_all::

##############################################################################
# variables exported to sub.mk (only effective if these packages are used)
##############################################################################

# include directories
SUB_DIRS := $(SUB_DIRS) $(subst -I,,$(C4_INCS))

# directories of outlying source file
SUB_DIRS := $(SUB_DIRS) $(foreach d,$(filter ../%,\
		$(C4_APP_SRCS)),$(dir $d))

##############################################################################
# C4 (as-IPUS) support assembler code that should not be preprocessed.
##############################################################################
