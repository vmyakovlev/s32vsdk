##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2011, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    full featured Makefile for desk builds (linux/x11) using nbuild packages.
#    internal use only
#
#  AUTHOR
#    Samuel Mudrik
#
#  DATE
#    2013-12-05
#
#  LANGUAGE
#    make
#
##############################################################################

include ../build-deskx11/Makefile
include $(SDK_ROOT)/build/nbuild/printvar.mk

clean::
	$(NBQ)rm -f *.gcda *.gcno

# gcov code coverage options
GCOV_OPTS = -fprofile-arcs -ftest-coverage

C0_CCOPTS += $(GCOV_OPTS)

C0_CXOPTS += $(GCOV_OPTS) -std=c++11 -fpermissive

C0_LDOPTS +=	-lgcov

