##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2017, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    some internal utility functions that are reused by several packages
#
#  AUTHOR
#    Mihail Nistor
#
#  DATE
#    2017-01-12
#
#  LANGUAGE
#    make
#
##############################################################################

ifndef _UTILITIES_MK
_UTILITIES_MK = true

# The relative Linux-style path between two different drives is not supported by the APU compilers on Window host.
# Solution: The absolute path will be used when the relative path should be computed for two different drivers.
ifeq ($(NBUILD_PLATFORM),msys)
fix_up_win_path = $(if $(findstring $(2),$(1)),$(subst \,\\,$(shell cygpath -w $(1))),$(1))
else
fix_up_win_path = $(1)
endif

# make function to make a path relative to a directory or file.
#
# path_relative_to = $(shell realpath --relative-to=$(1) $(2))
path_relative_to =	$(call fix_up_win_path,$(patsubst %/,%,$(shell echo $(2) | awk -vwd=$(1) -F/ 'BEGIN{ \
		  $$0=wd;for(f=2;f<=NF;f++) c[f]=$$f \
		}{ \
		  if(NF>1 && $$1!=".."){ \
		    printf("%s",$$1); \
		    for(f=2;f<=NF&&c[f]==$$f;f++); \
		    for(i=f;c[i];i++); \
		    for(j=f;j<i;j++)    printf("../"); \
		    for(i=f;$$i;i++) {if(i>f) printf("/"); printf("%s",$$i); } \
		    printf("\n"); \
		  }else \
		    print; \
		}')),$(2))

endif
