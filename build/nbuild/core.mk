##############################################################################
#
#  Copyright (c) 2011, Freescale Semiconductor. All Rights Reserved.
#  Freescale Confidential Proprietary
#
##############################################################################
#
#  CONTENT
#    generate a package with core specific names from the common _core
#    template
#
#  AUTHOR
#    Norbert Stoeffler
#
#  DATE
#    2011-04-04
#
#  LANGUAGE
#    make
#
##############################################################################


$(TOOLS)/nbuild/.%.mk: $(TOOLS)/nbuild/_core.mk
	@echo core.mk: generating $@
	$(NBQ)echo "# don't edit, generated from $(notdir $^)" > $@
	$(NBQ)sed >> $@ < $^ "s/&core&/$*/g"
	@$(NBQ)rm -f .makefile.d # invalid if created before corelib

