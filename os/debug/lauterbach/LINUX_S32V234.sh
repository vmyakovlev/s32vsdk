#!/bin/bash


if [ -f "/var/tmp/T32_1000003store.bak" ]; then
#    echo "/var/tmp/T32_1000003store.bak exists, so delete /var/tmp/T32_1000003store.bak"
    sudo rm "/var/tmp/T32_1000003store.bak"
fi

if [ -f "/var/tmp/T32_1000003store.cmm" ]; then
#    echo "/var/tmp/T32_1000003store.cmm exists, so delete /var/tmp/T32_1000003store.cmm"
    sudo rm "/var/tmp/T32_1000003store.cmm"
fi

if [ -f "/var/tmp/T32_1000004store.bak" ]; then
#    echo "/var/tmp/T32_1000004store.bak exists, so delete /var/tmp/T32_1000004store.bak"
    sudo rm "/var/tmp/T32_1000004store.bak"
fi

if [ -f "/var/tmp/T32_1000004store.cmm" ]; then
#    echo "/var/tmp/T32_1000004store.cmm exists, so delete /var/tmp/T32_1000004store.cmm"
    sudo rm "/var/tmp/T32_1000004store.cmm"
fi

if [ -f "/var/tmp/trace32_usb_lck" ]; then
#    echo "/var/tmp/trace32_usb_lck exists, so delete /var/tmp/trace32_usb_lck"
    sudo rm "/var/tmp/trace32_usb_lck"
fi

${T32SYS}/bin/pc_linux64/t32marm-qt   -c "$S32V234_SDK_ROOT/os/debug/lauterbach/LINUX_S32V234_M4.t32" &
sleep 4
${T32SYS}/bin/pc_linux64/t32marm64-qt -c "$S32V234_SDK_ROOT/os/debug/lauterbach/LINUX_S32V234_A53.t32" &



