#!/bin/sh

/home/maxwell/localGIT/qemu/build/x86_64-softmmu/qemu-system-x86_64 -boot d \
-cdrom /home/maxwell/Downloads/DOS6.22_bootdisk.iso \
-drive file=/home/maxwell/Downloads/floppy.img,index=0,if=floppy,format=raw \
-m 512M
