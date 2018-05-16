 #./Kbuild

check_defined = \
    $(foreach 1,$1,$(__check_defined))
__check_defined = \
    $(if $(value $1),, \
      $(error Please define $1 pointing to kernel directory $(if $(value 2), ($(strip $2) ))))

obj-m	:= apex.o
apex-objs := 	../src/linux/kernel_module.o \

KERNELDIR ?= $(LINUX_S32V234_DIR)
CROSS_COMPILE ?= aarch64-linux-gnu-

#-D DEBUG_OUTPUT \

CFLGS     := "-DDEBUG \
		-D__KERNEL_MODULE \
		-D__ARMV8 \
		-I$(CURDIR)/../include/ \
		-I$(CURDIR)/../../user/include/ \
		-I$(CURDIR)/../../../common/include/ \
		-I$(CURDIR)/../../../register_maps/include/ \
		-I$(CURDIR)/../../../../../include/ \
		$(APEXDRIVER_ADDITIONAL_FLAGS)"

.PHONY: clean cleansub all allsub

all allsub: check-env
	$(MAKE) KCPPFLAGS=$(CFLGS) ARCH=arm64 CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(CURDIR)

check-env:
	$(call check_defined, LINUX_S32V234_DIR)

clean cleansub:
	$(MAKE) KCPPFLAGS=$(CFLGS) ARCH=arm64 CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(CURDIR) clean
	rm -rf $(apex2-objs) ../src/linux/.*.cmd ../src/linux/*.o ../../common/src/*.o ../../common/src/.*.cmd