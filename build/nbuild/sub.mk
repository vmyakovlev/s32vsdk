##############################################################################
#
#  Copyright (c) 2006, Freescale Semiconductor. All Rights Reserved.
#  Copyright 2017 NXP;
#  Freescale Confidential Proprietary
#
##############################################################################
#
#  CONTENT
#    optional make package for recursive makes in projects that involve many
#    subdirs outside. most of the other make packages define some parameters
#    already
#
#  PARAMETERS
#    SUB_MAKES	directories to go and call make. add more with +=
#    SUB_DIRS	directories inolved, but without make (e.g. include dirs).
#		add more with +=
#
#  AUTHOR
#    Norbert Stoeffler
#    Mihail Marian Nistor
#
#  DATE
#    2017-03-3
#
#  LANGUAGE
#    make
#
##############################################################################

ifndef _SUB_MK
_SUB_MK = true

include $(CURR_SDK_ROOT)/build/nbuild/helpers.mk

##############################################################################
#  generate the cache with the variables
##############################################################################


_SUB_MAKES := $(call uniq,$(filter-out $(CURDIR),$(realpath $(SUB_MAKES))))
_SUB_DIRS := $(sort $(SUB_DIRS))

##############################################################################
#  check, if all sub directories exist ...
##############################################################################

_NOT_EXIST_SUB_DIRS := $(filter-out $(wildcard $(_SUB_DIRS)),$(_SUB_DIRS))
_NOT_EXIST_SUB_MAKES := $(filter-out $(wildcard $(SUB_MAKES)),$(SUB_MAKES))

define __sub_chkdirs
  $(if $(strip $(_NOT_EXIST_SUB_DIRS) $(_NOT_EXIST_SUB_MAKES)), \
   $(foreach d,$(_NOT_EXIST_SUB_DIRS),$(warning sub.mk: SUB_DIR $d is missing)) \
   $(foreach d,$(_NOT_EXIST_SUB_MAKES),$(warning SUB_MAKE $d is missing)) \
   $(error You can find the missing directory/directories here: $(_NOT_EXIST_SUB_DIRS) $(_NOT_EXIST_SUB_MAKES) \
     [$(abspath $(_NOT_EXIST_SUB_DIRS) $(_NOT_EXIST_SUB_MAKES))]))

endef

##############################################################################
#  targets to be applied to our sub makes
##############################################################################

.PHONY: all allsub cleansub showsub debugsub

# override to avoid recursion
SUB_CD ?= .

all::
	@$(call __sub_chkdirs)

# apply some standard targets on all SUB_MAKES
allsub cleansub:
	@$(call __sub_chkdirs)
	@for i in $(_SUB_MAKES) $(SUB_CD) ; do \
	  if [ -f $$i/$(firstword $(NBMAKEFILE)) ] ; then \
	    $(MAKE) -C $$i SUB_EXTERN_DEP=1 ${@:sub=} || { kill $$$$; exit 1; } \
	  else \
	    printf "make[-]: skipping directory $$i\n" ; fi \
	  done

# show all subdirs
showsub:
	@for i in $(_SUB_DIRS) ; do \
	  ( cd $$i || exit 1 ; pwd ) \
	done

# debugging
debugsub:
	@printf "vpath _____________\n"
	@echo $(VPATH)
	@printf "local sub_makes _____________\n"
	@for i in $(_SUB_MAKES) ; do echo $$i; done
	@printf "local sub_dirs _____________\n"
	@for i in $(_SUB_DIRS) ; do echo $$i; done

endif
