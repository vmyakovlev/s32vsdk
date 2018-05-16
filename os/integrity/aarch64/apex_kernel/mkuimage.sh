gmemfile $1
mkimage -A arm -O linux -T kernel -C none -a 0x100000 -e 0x100000 \
               -n "INTEGRITY" -d $1.bin $1.uimage

