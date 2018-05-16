##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2011, Freescale Semiconductor, Inc.
#  Copyright 2017 NXP;
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
#    2017-02-27
#
#  LANGUAGE
#    make
#
##############################################################################
	
ACL_APP  := ocl_kernels_load
ACL_APP_SRCS := kernel_db.cpp

ACL_APP_LIBS := $(ACL_KERNEL_LIBS)		\
		$(SDK_ROOT)/libs/apex/acl_target/build-apu-tct-sa-d/libacl_target.a \
		$(SDK_ROOT)/libs/apex/common/build-apu-tct-sa-d/libapu_common.a
		
ACL_INCS := $(ACL_KERNEL_SRC_DIRS)		\
	    -I$(SDK_ROOT)/libs/apex/acl_target/include
	    
ACL_CCOPTS =   -p APU2 +W noodle,+Oivsq	\
		-include tct_opencl.h 			\
		-include tct_opencl_apu.h 		\
		-include tct_opencl_apu_builtin.h	\
		-include tct_opencl_apu_config.h	
		
ACL_LDOPTS = +W bridge,-m +W bridge,-fH +l 					\
	      linker.bcf
		

kernel_db.cpp: linker.bcf
	@$(eval SRC_CPP:=kernel_db.cpp)
	@echo "//////////////////////////////////////////////////////////////////////////////////"   > $(SRC_CPP)
	@echo "//  (C) Copyright CogniVue Corporation. 2014 All rights reserved."                   >> $(SRC_CPP)
	@echo "//"                                                                                  >> $(SRC_CPP)
	@echo "//  Confidential Information"                                                        >> $(SRC_CPP)
	@echo "//"                                                                                  >> $(SRC_CPP)
	@echo "//  All parts of the CogniVue software and documentation are confidential and"       >> $(SRC_CPP)
	@echo "//  proprietary to CogniVue Corp."                                                   >> $(SRC_CPP)
	
	@echo "/******************************************************************************"      > $(SRC_CPP)
	@echo "*  (C) Copyright CogniVue Corporation. 2014 All right reserved."                     >> $(SRC_CPP)
	@echo "* "                                                                                  >> $(SRC_CPP)
	@echo "*  Confidential Information"                                                         >> $(SRC_CPP)
	@echo "*"                                                                                   >> $(SRC_CPP)
	@echo "*  All parts of the CogniVue Program Source are protected by copyright law"          >> $(SRC_CPP)
	@echo "*  and all rights are reserved."                                                     >> $(SRC_CPP)
	@echo "*  This documentation may not, in whole or in part, be copied, photocopied,"         >> $(SRC_CPP)
	@echo "*  reproduced, translated, or reduced to any electronic medium or machine"           >> $(SRC_CPP)
	@echo "*  readable form without prior consent, in writing, from CogniVue."                  >> $(SRC_CPP)
	@echo "******************************************************************************/"     >> $(SRC_CPP)
	@echo "#include <apu_ocl.h>"                                                                >> $(SRC_CPP)
	@$(foreach NAME, $(ACL_KERNELS), echo "#include <$(NAME)_md_config.cpp>"                    >> $(SRC_CPP);)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "// Number of kernels registered in the database"                                     >> $(SRC_CPP)
	@echo "const int32_t CL_KRNL_DB_SIZE[1] = {$(words $(ACL_KERNELS))};"                       >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "// Kernel database"                                                                  >> $(SRC_CPP)
	@echo "// max number of entries: 16"                                                        >> $(SRC_CPP)
	@echo "const CL_KRNL_DB_ENTRY CL_KERNEL_DB[16] ="                                           >> $(SRC_CPP)
	@echo "   {"                                                                                >> $(SRC_CPP)
	@$(foreach NAME, $(ACL_KERNELS), printf "   {  \"$(NAME)\", \n      (int32_t)(&$(NAME)_md)\n   }, \n\n" >> $(SRC_CPP);)
	@echo "   {"                                                                                >> $(SRC_CPP)
	@echo "       0, 0"                                                                         >> $(SRC_CPP)
	@echo "   },"                                                                               >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "   {"                                                                                >> $(SRC_CPP)
	@echo "       0, 0"                                                                         >> $(SRC_CPP)
	@echo "   } "                                                                               >> $(SRC_CPP)
	@echo "};"                                                                                  >> $(SRC_CPP)

linker.bcf:
	@$(eval SRC_CPP:=linker.bcf)	
ifeq (1, $(words $(ACL_KERNELS)))
	@$(eval ACL_KERNELS_2 = "")
	@$(eval NLIST = 0)
else
	@$(eval ACL_KERNELS_2 = $(filter-out $(word 1,$(ACL_KERNELS)), $(ACL_KERNELS)))
	@$(eval NLIST = $(shell for x in {1..$(words $(ACL_KERNELS_2))}; do echo "$$x"; done))
endif
	@echo "//////////////////////////////////////////////////////////////////////////////////"   > $(SRC_CPP)
	@echo "//  (C) Copyright CogniVue Corporation. 2014 All rights reserved."                   >> $(SRC_CPP)
	@echo "//  "                                                                                >> $(SRC_CPP)
	@echo "//  Confidential Information"                                                        >> $(SRC_CPP)
	@echo "//"                                                                                  >> $(SRC_CPP)
	@echo "//  All parts of the CogniVue software and documentation are confidential and "      >> $(SRC_CPP)
	@echo "//  proprietary to CogniVue Corp."                                                   >> $(SRC_CPP)
	@echo "//  This file may not, in whole or in part, be copied, photocopied,"                 >> $(SRC_CPP)
	@echo "//  reproduced, translated, or reduced to any electronic medium or machine"          >> $(SRC_CPP)
	@echo "//  readable form without prior consent, in writing, from CogniVue."                 >> $(SRC_CPP)
	@echo "//////////////////////////////////////////////////////////////////////////////////"  >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "// --------------------------------------------"                                     >> $(SRC_CPP)
	@echo "// WARNING: DON'T MODIFY THE FOLLOWING SECTION!"                                     >> $(SRC_CPP)
	@echo "// --------------------------------------------"                                     >> $(SRC_CPP)
	@echo "// need to include APU2_init.s so that main_init is included!"                       >> $(SRC_CPP)
	@echo "_entry_point _main_init"                                                             >> $(SRC_CPP)
	@echo "_symbol _main_init 0"                                                                >> $(SRC_CPP)
	@echo "_symbol _main _after _main_init"                                                     >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "_always_include _main_argv_area"                                                     >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "_stack DMb 0x0000 0x0800"                                                            >> $(SRC_CPP)
	@echo "_stack VMb 0x0000 0x0400"                                                            >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "_symbol DMEM_SHARED  0x7000"                                                         >> $(SRC_CPP)
	@echo "_symbol DMEM_CMD_CNT 0x7020"                                                         >> $(SRC_CPP)
	@echo "_symbol DMEM_CMD_QUE 0x7040"                                                         >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "//CMEM: ALWAYS RESERVED"                                                             >> $(SRC_CPP)
	@echo "_symbol CMEM_RESERVED     4096 _no_init     // END OF CMEM"                          >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "// VIRTUAL"                                                                          >> $(SRC_CPP)
	@echo "_symbol DMEM_KERNEL_NUM    0x10800"                                                  >> $(SRC_CPP)
	@echo "_symbol DMEM_KERNEL_DB     0x10804"                                                  >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo ""                                                                                    >> $(SRC_CPP)
	@echo "// --------------------------------------------"                                     >> $(SRC_CPP)
	@echo "// APEX-CL KERNEL AND METADATA DATABASE"                                             >> $(SRC_CPP)
	@echo "// MAX DATABASE ENTRY: 16"                                                           >> $(SRC_CPP)
	@echo "// --------------------------------------------"                                     >> $(SRC_CPP)
	@echo "_symbol CL_KRNL_DB_SIZE        0x10D00"                                              >> $(SRC_CPP)
	@echo "_symbol CL_KERNEL_DB           0x10D04"                                              >> $(SRC_CPP)
	@echo "_symbol $(word 1,$(ACL_KERNELS))_md _after CL_KERNEL_DB"                                >> $(SRC_CPP)
ifneq ($(words $(ACL_KERNELS)), 1)
	@$(foreach NUM, $(NLIST), echo "_symbol $(word $(NUM),$(ACL_KERNELS_2))_md _after $(word $(NUM),$(ACL_KERNELS))_md" >> $(SRC_CPP);)
endif	

all:: kernel_db.cpp ocl_kernels_load.elf ocl_kernels_APU_LOAD.h

ocl_kernels_APU_LOAD.h : ocl_kernels_load.elf
	$(READELF) -pPMh=32 -mDMb=8 -mVMb=256 ocl_kernels_load.elf -eGb -t $(TCL_SCRIPT) -Tload_name=APU_OCL > APU_OCL_LOAD.hpp
		
clean::
	$(NBQ)rm -f $(KERNEL_EXES) *_apu_process_desc_main.* build_args.txt *.P *.c *.cpp *.map *.bcf *.h *.hpp
