##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2011, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#
#
#  AUTHOR
#    Stefan Schwarz
#
#  DATE
#    2013-08-05
#
#  LANGUAGE
#    make
#
##############################################################################
ifeq ($(X86_KERNEL_DB),"on")
$(X86_APP)_db.h: $(APU_LIB) $(X86_APP)
	@echo "Generating _db.h file..."
	@./$(X86_APP) $(basename $(X86_APP))_db >> $(basename $(X86_APP)).log
	@echo "------------------------------------------------------------"

all:: $(X86_APP)_db.h
else
all::
endif
