##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2013, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#
#  AUTHOR
#    Rostislav Hulik
#
#  DATE
#    2016-06-02
#
##############################################################################

check_defined = \
    $(foreach 1,$1,$(__check_defined))
__check_defined = \
    $(if $(value $1),, \
      $(error Please define $1 $(if $(value 2), ($(strip $2) ))))
      
#SDK_ROOT=$(shell pwd)/../

LINUX_ROOT = $(LINUX_S32V234_DIR)
LINUX_BRANCH = v4.1.26_bsp13.0
UBOOT_ROOT = $(UBOOT_S32V234_DIR)
UBOOT_BRANCH = v2016.01_bsp13.0
BUILDROOT_ROOT = $(BUILDROOT_S32V234_DIR)
BUILDROOT_BRANCH = v2015.08-rc1_s32v234_0.8
OUTPUT_SDK_ROOT = $(S32V234_SDK_ROOT)/../output
SDK_ROOT = $(S32V234_SDK_ROOT)
CMA_32M_CONFIG = $(SDK_ROOT)/os/linux/cma_32_config

#V234_EXEC_DIR=/home/b46809/bin/test

.PHONY: all linux uboot kernelmodules buildroot demos_linux demos_standalone apex apex_libs pack_all

all: check-env pack_all copy_out

prepare:
	rm -rf $(OUTPUT_SDK_ROOT); \
	mkdir $(OUTPUT_SDK_ROOT); \
	mkdir $(OUTPUT_SDK_ROOT)/s32v234_sdk; \
	mkdir $(OUTPUT_SDK_ROOT)/s32v234_sdk/os; \
	mkdir $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content; \
	mkdir $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build; \
	mkdir $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build; \
	
uboot: prepare
	cd $(UBOOT_ROOT); \
	git fetch; \
	git stash; \
	git checkout $(UBOOT_BRANCH); \
	sudo rm -rf *; \
	sudo rm -f .*; \
	git reset --hard; \
	make ARCH=aarch64 CROSS_COMPILE=aarch64-linux-gnu- s32v234evb_defconfig; \
	make CROSS_COMPILE=aarch64-linux-gnu-; \
	make CROSS_COMPILE=aarch64-linux-gnu- tools; \
	cp u-boot.s32 $(SDK_ROOT)/os/build_content/v234_linux_build/u-boot-29288.s32; \
	cp u-boot.s32 $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build/u-boot-29288.s32; \
	sudo rm -rf *; \
	sudo rm -f .*; \
	git reset --hard; \
	make ARCH=aarch64 CROSS_COMPILE=aarch64-linux-gnu- s32v234evb28899_defconfig; \
	make CROSS_COMPILE=aarch64-linux-gnu-; \
	make CROSS_COMPILE=aarch64-linux-gnu- tools; \
	cp u-boot.s32 $(SDK_ROOT)/os/build_content/v234_linux_build/u-boot-28899.s32; \
	cp u-boot.s32 $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build/u-boot-28899.s32; \
	cd -
	
uboot_pcie: prepare
	cd $(UBOOT_ROOT); \
	git fetch; \
	git stash; \
	git checkout $(UBOOT_BRANCH); \
	sudo rm -rf *; \
	sudo rm -f .*; \
	git reset --hard; \
	make ARCH=aarch64 CROSS_COMPILE=aarch64-linux-gnu- s32v234pcie_config; \
	make CROSS_COMPILE=aarch64-linux-gnu-; \
	make CROSS_COMPILE=aarch64-linux-gnu- tools; \
	cp u-boot.s32 $(SDK_ROOT)/os/build_content/v234_linux_build/u-boot-pcie.s32; \
	cp u-boot.s32 $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build/u-boot-pcie.s32; \
	cd -

linux: uboot uboot_pcie
	cd $(LINUX_ROOT); \
	git fetch; \
	git stash; \
	git checkout $(LINUX_BRANCH); \
	rm -rf *; \
	rm -f .*; \
	git reset --hard; \
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- s32v234_defconfig; \
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- KCONFIG_CONFIG=$(CMA_32M_CONFIG); \
	cp arch/arm64/boot/Image $(SDK_ROOT)/os/build_content/v234_linux_build; \
	cp arch/arm64/boot/Image $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build; \
	cp vmlinux $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build; \
	cd -

kernelmodules: linux
	cd $(SDK_ROOT); \
	export LINUX_S32V234_DIR=$(LINUX_ROOT); \
	export SDK_ROOT_LIB=$(SDK_ROOT)/libs; \
	for i in $$(find $$SDK_ROOT_LIB -maxdepth 10 -type d); do \
	  if [ -d $$i/kernel/build-v234ce-gnu-linux-d ]; then \
	    cd $$i/kernel/build-v234ce-gnu-linux-d; \
	    make; \
	    cd -; \
	  fi; \
	done; \
	cd $(LINUX_ROOT); \
	patch -p1 -i $(SDK_ROOT)/os/linux/s32v234_dtsi.patch; \
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- KCONFIG_CONFIG=$(CMA_32M_CONFIG) dtbs; \
	cp arch/arm64/boot/dts/freescale/s32v234-evb.dtb $(SDK_ROOT)/os/build_content/v234_linux_build; \
	cp arch/arm64/boot/dts/freescale/s32v234-evb.dtb $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build; \
	cp arch/arm64/boot/dts/freescale/s32v234-pcie.dtb $(SDK_ROOT)/os/build_content/v234_linux_build; \
	cp arch/arm64/boot/dts/freescale/s32v234-pcie.dtb $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build; \
	cp arch/arm64/boot/dts/freescale/s32v234-evb28899.dtb $(SDK_ROOT)/os/build_content/v234_linux_build; \
	cp arch/arm64/boot/dts/freescale/s32v234-evb28899.dtb $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build; \
	git stash; \
	git stash drop; \
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- KCONFIG_CONFIG=$(CMA_32M_CONFIG)
	

buildroot: kernelmodules
	cd $(SDK_ROOT); \
	sudo rm -rf temp; \
	mkdir temp; \
	cd $(BUILDROOT_ROOT); \
	sudo rm -rf temp; \
	mkdir temp; \
	cd temp; \
	sudo tar -xvf $(SDK_ROOT)/../fsl-image-s32v2xx-s32v234evb.tar.gz; \
	export SDK_ROOT_LIB=$(SDK_ROOT)/libs; \
	sudo mkdir s32v234; \
	sudo mkdir lib/firmware; \
	sudo cp $(SDK_ROOT)os/linux/sja1105/sja1105_cfg_mra2lt_1-2.bin $(BUILDROOT_ROOT)/temp/lib/firmware; \
	sudo cp $(SDK_ROOT)os/linux/sja1105/sja1105_cfg_mra2lt_2-2.bin $(BUILDROOT_ROOT)/temp/lib/firmware; \
	sudo cp $(SDK_ROOT)os/linux/sja1105/sja1105.ko $(BUILDROOT_ROOT)/temp/s32v234; \
	for i in $$(find $$SDK_ROOT_LIB -maxdepth 10 -type d); do \
	  if [ -d $$i/kernel/build-v234ce-gnu-linux-d ]; then \
	    cd $$i/kernel/build-v234ce-gnu-linux-d; \
	    sudo cp *.ko $(BUILDROOT_ROOT)/temp/s32v234/; \
	    cd -; \
	  fi; \
	done; \
	for file_path in $(SDK_ROOT)3rdparty/ocv/linux-arm64/lib/libopencv*.so; do sudo cp $$file_path lib/$$(basename $$file_path).3.1; done; \
	for file_path in $(SDK_ROOT)3rdparty/boost_1_62_0/stage/lib/libboost*.so; do sudo cp $$file_path lib/$$(basename $$file_path).1.62.0; done; \
	sudo cp $(SDK_ROOT)3rdparty/ffmpeg/linux-arm64/lib/libavcodec.so lib/libavcodec.so.56; \
	sudo cp $(SDK_ROOT)3rdparty/ffmpeg/linux-arm64/lib/libavdevice.so lib/libavdevice.so.56; \
	sudo cp $(SDK_ROOT)3rdparty/ffmpeg/linux-arm64/lib/libavfilter.so lib/libavfilter.so.5; \
	sudo cp $(SDK_ROOT)3rdparty/ffmpeg/linux-arm64/lib/libavformat.so lib/libavformat.so.56; \
	sudo cp $(SDK_ROOT)3rdparty/ffmpeg/linux-arm64/lib/libavutil.so lib/libavutil.so.54; \
	sudo cp $(SDK_ROOT)3rdparty/ffmpeg/linux-arm64/lib/libswresample.so lib/libswresample.so.1; \
	sudo cp $(SDK_ROOT)3rdparty/ffmpeg/linux-arm64/lib/libswscale.so lib/libswscale.so.3; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/h264enc.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/fdma.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/seq.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/cam.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/csi.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/oal_cma.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/h264dcd.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/jpegdcd.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/viulite.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/apex.ko"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print "insmod /s32v234/sja1105.ko max_hz=5000 verbosity=1"; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	sudo awk '/LOAD_MODULE=modprobe/ { print; print ""; next }1' $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh > $(SDK_ROOT)/temp/modutils_tmp; \
	sudo cp $(SDK_ROOT)/temp/modutils_tmp $(BUILDROOT_ROOT)/temp/etc/init.d/modutils.sh; \
	cd -
	
apex:
	cd $(SDK_ROOT); \
	export SDK_ROOT_APEX=$(SDK_ROOT); \
	for i in $$(find $$SDK_ROOT_APEX -maxdepth 10 -type d); do \
	  if [ -d $$i/graphs/build-apu-tct-sa-d ]; then \
	    cd $$i/graphs/build-apu-tct-sa-d; \
	    make allsub; \
	    find . ! -name '*APU_LOAD.h' -and ! -name '*.hpp' -and ! -name 'Makefile' -type f -exec rm -f {} + ; \
	    rm -f *nativekernel.hpp; \
	    cd -; \
	  fi; \
	done; \
	for i in $$(find $$SDK_ROOT_APEX -maxdepth 10 -type d); do \
	  if [ -d $$i/graphs/build-apu-nxp-sa-d ]; then \
	    cd $$i/graphs/build-apu-nxp-sa-d; \
	    make allsub; \
	    find . ! -name '*APU_LOAD.h' -and ! -name '*.hpp' -and ! -name 'Makefile' -type f -exec rm -f {} + ; \
	    rm -f *nativekernel.hpp; \
	    cd -; \
	  fi; \
	done; 

apex_libs:
	cd $(SDK_ROOT); \
	export SDK_ROOT_APEX=$(SDK_ROOT)/libs/apex; \
	for i in $$(find $$SDK_ROOT_APEX -maxdepth 10 -type d); do \
	  if [ -d $$i/build-apu-tct-sa-d ]; then \
	    cd $$i/build-apu-tct-sa-d; \
	    make allsub; \
	    find . ! -name '*.a' -type f -exec rm -f {} + ; \
	    cd -; \
	  fi; \
	done; \
	for i in $$(find $$SDK_ROOT_APEX -maxdepth 10 -type d); do \
	  if [ -d $$i/build-apu-nxp-sa-d ]; then \
	    cd $$i/build-apu-nxp-sa-d; \
	    make allsub; \
	    find . ! -name '*.a' -type f -exec rm -f {} + ; \
	    cd -; \
	  fi; \
	done; \
	for i in $$(find $$SDK_ROOT_APEX -maxdepth 10 -type d); do \
	  if [ -d $$i/build-x86-gnu-linux-d ]; then \
	    cd $$i/build-x86-gnu-linux-d; \
	    make allsub; \
	    find . ! -name '*.a' -type f -exec rm -f {} + ; \
	    cd -; \
	  fi; \
	done;
	
demos_linux: buildroot apex #apex_libs
	cd $(SDK_ROOT); \
	sudo mkdir $(BUILDROOT_ROOT)/temp/home/root/vsdk; \
	export SDK_ROOT_DEMOS=$(SDK_ROOT)/demos; \
	for i in $$(find $$SDK_ROOT_DEMOS -maxdepth 10 -type d); do \
	  if [ -d $$i/build-v234ce-gnu-linux-o ]; then \
	    cd $$i/build-v234ce-gnu-linux-o; \
	    make allsub; \
	    sudo cp $$(basename $$i)".elf" $(BUILDROOT_ROOT)/temp/home/root/vsdk; \
	    cd -; \
	  fi; \
	done; \
	cd $(SDK_ROOT)/demos/isp/isp_sonyimx224_csi_dcu/build-v234ce-gnu-linux-o ;\
	make cleansub; \
	make allsub; \
	sudo cp -rpav isp_sonyimx224_csi_dcu.elf $(BUILDROOT_ROOT)/temp/home/root/vsdk; \
	cd $(SDK_ROOT)/demos/isp/isp_sonyimx224_rgb_yuv_gs8/build-v234ce-gnu-linux-o ;\
	make cleansub; \
	make allsub; \
	sudo cp -rpav isp_sonyimx224_rgb_yuv_gs8.elf $(BUILDROOT_ROOT)/temp/home/root/vsdk; \
	sudo rm -rf $(BUILDROOT_ROOT)/temp/home/root/vsdk/apexcv_box_generic.elf; \
	sudo rm -rf $(BUILDROOT_ROOT)/temp/home/root/vsdk/isp_sonyimx224_generic.elf; \
	sudo rm -rf $(BUILDROOT_ROOT)/temp/home/root/vsdk/isp_h264dec_4stream.elf; \
	sudo rm -rf $(BUILDROOT_ROOT)/temp/home/root/vsdk/isp_jpeg_single_stream.elf; \
	sudo rm -rf $(BUILDROOT_ROOT)/temp/home/root/vsdk/isp_sonyimx224_quad.elf; \
	sudo rm -rf $(BUILDROOT_ROOT)/temp/home/root/vsdk/isp_ov10635_quad.elf; \
	sudo rm -rf $(BUILDROOT_ROOT)/temp/home/root/vsdk/isp_sonyimx224_yuv_grey_pyramid.elf;
	
	
	
demos_standalone: #buildroot apex
	cd $(SDK_ROOT); \
	sudo mkdir $(BUILDROOT_ROOT)/temp/home/root/vsdk; \
	sudo cp -rpav $(SDK_ROOT)/demos/data $(BUILDROOT_ROOT)/temp/home/root/vsdk; \
	export SDK_ROOT_DEMOS=$(SDK_ROOT)/demos; \
	for i in $$(find $$SDK_ROOT_DEMOS -maxdepth 10 -type d); do \
	  if [ -d $$i/build-v234ce-gnu-sa-d ]; then \
	    cd $$i/build-v234ce-gnu-sa-d; \
	    make allsub; \
	    cd -; \
	  fi; \
	done; \
	cd $(SDK_ROOT)/demos/isp/isp_sonyimx224_csi_dcu/build-v234ce-gnu-sa-d ;\
	make cleansub; \
	make allsub; \
	cd $(SDK_ROOT)/demos/isp/isp_sonyimx224_rgb_yuv_gs8/build-v234ce-gnu-sa-d ;\
	make cleansub; \
	make allsub; 
	

pack_all: demos_linux demos_standalone
	cd $(BUILDROOT_ROOT)/temp; \
	sudo tar -cvf rootfs.tar *; \
	cp rootfs.tar $(SDK_ROOT)os/build_content/v234_linux_build; \
	cp rootfs.tar $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build; \
	sudo rm -f rootfs.tar; \
	sudo sed -i 's/LF0/LF1/g' etc/inittab; \
	sudo tar -cvf rootfs-pcie.tar *; \
	cp rootfs-pcie.tar $(SDK_ROOT)os/build_content/v234_linux_build; \
	cp rootfs-pcie.tar $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_linux_build; \
	cd -
	
copy_out: prepare pack_all
	cd $(SDK_ROOT); \
	cp --parents libs/apex/acf_offline/build-x86-gnu-linux-d/*.a  $(OUTPUT_SDK_ROOT)/s32v234_sdk; \
	cp --parents libs/apex/common/build-apu-tct-sa-d/*.a  $(OUTPUT_SDK_ROOT)/s32v234_sdk; \
	cp --parents libs/apex/acf_target/build-apu-tct-sa-d/*.a  $(OUTPUT_SDK_ROOT)/s32v234_sdk; \
	cp --parents libs/apex/acl_target/build-apu-tct-sa-d/*.a  $(OUTPUT_SDK_ROOT)/s32v234_sdk; \
	cp --parents libs/apex/common/build-apu-nxp-sa-d/*.a  $(OUTPUT_SDK_ROOT)/s32v234_sdk; \
	cp --parents libs/apex/acf_target/build-apu-nxp-sa-d/*.a  $(OUTPUT_SDK_ROOT)/s32v234_sdk; \
	cp --parents libs/apex/acl_target/build-apu-nxp-sa-d/*.a  $(OUTPUT_SDK_ROOT)/s32v234_sdk; \
	find . -name '*.elf' -wholename '*/demos/*/build-v234ce-gnu-sa-d/*.elf' -exec cp \{\} $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build \; ;\
	find . -name '*.bin' -wholename '*/demos/*/build-v234ce-gnu-sa-d/*.bin' -exec cp \{\} $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build \; ;\
	find . -name '*.cmm' -wholename '*/demos/*/build-v234ce-gnu-sa-d/*.cmm' -exec cp \{\} $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build \; ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/qspi_readwrite.bin ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/qspi_readwrite.elf ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/qspi_readwrite.cmm ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_sonyimx224_generic.bin ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_sonyimx224_generic.elf ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_sonyimx224_generic.cmm ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_sonyimx224_quad.bin ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_sonyimx224_quad.elf ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_sonyimx224_quad.cmm ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_ov10635_quad.bin ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_ov10635_quad.elf ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_ov10635_quad.cmm ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_sonyimx224_yuv_grey_pyramid.bin ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_sonyimx224_yuv_grey_pyramid.elf ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/isp_sonyimx224_yuv_grey_pyramid.cmm ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/apex_isp_fast9.bin ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/apex_isp_fast9.elf ;\
	sudo rm -rf $(OUTPUT_SDK_ROOT)/s32v234_sdk/os/build_content/v234_baremetal_build/apex_isp_fast9.cmm ;\
	cd - ;
	
check-env:
	$(call check_defined, LINUX_S32V234_DIR)
	$(call check_defined, UBOOT_S32V234_DIR)
	$(call check_defined, BUILDROOT_S32V234_DIR)