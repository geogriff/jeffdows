#!/bin/sh

kernel=$1
shift
gdb -ex "target remote | exec qemu -cpu 486 -m 16 -vga std -kernel $kernel -S -gdb stdio -no-reboot -no-shutdown" \
    -ex "set architecture i386:intel" \
    -s $kernel \
    $@ 
