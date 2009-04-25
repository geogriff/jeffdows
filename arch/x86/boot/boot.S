#include "boot.h"

    .text
    .code32
    .globl      _start
_start:
    jmp         entry

    .align      4
mboot:
    .long       MULTIBOOT_MAGIC
    .long       MULTIBOOT_FLAGS
    .long       MULTIBOOT_CHECKSUM

    .long       mboot
    .long       _start
    .long       _edata
    .long       _end
    .long       _start

    .code32
entry:
    // setup stack
    movl        $(stack+STACK_SIZE), %esp

    // reset eflags
    pushl $0
    popf

    // do i386 initialization using info given by multiboot
    pushl       %ebx
    pushl       %eax
    call        multiboot_init

    // jump to machine-independent main function
    call        main

    hlt

    .comm       stack, STACK_SIZE
stack_end:
        