##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2015, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Google Protocol buffers NBUILD package
#
#  AUTHOR
#    Samuel Mudrik
#
#  DATE
#    2015-05-15
#
#  LANGUAGE
#    make
#
##############################################################################

GPROTOBUF_ROOT=$(SDK_ROOT)/gprotobuf

GPROTOBUF_COMPILER=$(GPROTOBUF_ROOT)/build-deskx11/bin/protoc*

ARM_INCS += -I$(GPROTOBUF_ROOT)/src/src

ARM_LDOPTS += -lm

SRC_DIR=../src/

all :: %.pb.cc

$(GPROTOBUF_COMPILER): $(GPROTOBUF_ROOT)/build-deskx11/lib/libprotobuf.a

%.pb.cc ::
	$(GPROTOBUF_COMPILER) -I$(SRC_DIR) $(addprefix $(SRC_DIR),$(PB_SRCS)) --cpp_out=.

clean ::
	rm -rf *.pb.h *.pb.cc
