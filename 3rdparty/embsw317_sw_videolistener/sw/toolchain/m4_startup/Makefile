CC=c:/linaro/gcc-linaro-arm-none-eabi-4.9-2014.05_win32/bin/arm-none-eabi-gcc
AS=c:/linaro/gcc-linaro-arm-none-eabi-4.9-2014.05_win32/bin/arm-none-eabi-as
LD=c:/linaro/gcc-linaro-arm-none-eabi-4.9-2014.05_win32/bin/arm-none-eabi-ld


CFLAGS=                 \
    -Iinclude           \
    -I../a5_startup/include \
	-DCORE_M4_ENABLE    \
	-DPMC_CHECK_BYPASS  \
	-DSTCU_CHECK_BYPASS \
    -g                  \
    -mfpu=fpv4-sp-d16 -mfloat-abi=softfp \
	-mcpu=cortex-m4     \
	-march=armv7e-m \
	-mthumb             \
	-std=c99   
 
AFLAGS=                \
    -g                 \
	-mcpu=cortex-m4    \
	-march=armv7e-m \
	-mfpu=fpv4-sp-d16 -mfloat-abi=softfp \
	-mthumb
	
LDFLAGS= -T linker/linker.ld -Map=linker/binary.map -L. 



_OBJ =                       \
    startup/crt0_m4_0.o      \
	startup/linflex_serial.o \
	startup/m4_functions.o   \
	startup/pll.o            \
  startup/printf.o  \
  startup/SetupLPDDR2.o \
  startup/qspi_driver.o \
  startup/i2c.o   \
  startup/MIPI.o    \
  startup/HDMI.o      \
  startup/SetupDDR3.o   \
	main.o
	

OBJ = $(patsubst %,./%,$(_OBJ))

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.s
	$(AS) -c -o $@ $< $(AFLAGS)

binary: $(OBJ)
	$(LD) -o binary.elf $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f startup/*.o *~ core $(INCDIR)/*~ 
	rm -f *.o
	rm -f linker/binary.map
	rm -f *.elf
