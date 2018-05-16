##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2015, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Autoconf NBUILD package
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

SRC_DIR = ../src

all :: fix_paths autoconf $(ARM_LIB)

ifneq ($(ODIR),build-deskx11)
WITH_PROTOC=--with-protoc=../build-deskx11/bin/protoc*
HOST=--host=$(shell echo $(CROSS_COMPILE) | head -c -2)
endif

ifneq (,$(findstring CYGWIN,$(shell uname -s)))
PWD=$(shell cygpath -am .)
else
PWD=$(shell pwd)
endif

$(SRC_DIR)/Makefile:
	cd $(SRC_DIR); sh ./configure $(HOST) $(WITH_PROTOC) --prefix=$(PWD)

fix_paths:
	cd $(SRC_DIR); find . -type f -name "*.Plo*" -exec sed -i -- 's~c:\\~/cygdrive/c/~g' {} +
	cd $(SRC_DIR); find . -type f -name "*.Po*"  -exec sed -i -- 's~c:\\~/cygdrive/c/~g' {} +

autoconf: clean $(SRC_DIR)/Makefile
	make --no-print-directory -C $(SRC_DIR)
	make --no-print-directory fix_paths
	make --no-print-directory -C $(SRC_DIR) install
	cp lib/$(ARM_LIB) $(ARM_LIB)

clean:: fix_paths clean
	@rm -rf bin include lib $(ARM_LIB)
	-cd $(SRC_DIR); make distclean
