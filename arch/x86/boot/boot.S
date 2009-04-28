#define __ASM__
#include <mem/page.h>
#include "multiboot.h"
    
    .text
    .global _start
_start:
    jmp start

    .align 4
mboot:
    .long MULTIBOOT_HEADER_MAGIC
    .long MULTIBOOT_HEADER_FLAGS
    .long MULTIBOOT_HEADER_CHECKSUM

    .long (mboot-KERNEL_BASE)
    .long (_start-KERNEL_BASE)
    .long (_edata-KERNEL_BASE)
    .long (_end-KERNEL_BASE)
    .long (_start-KERNEL_BASE)

start:
    /* note that until paging is set up, every pointer must have KERNEL_BASE
    subtracted from it (remember short jumps don't need this) */

    /*sub MULTIBOOT_MAGIC, %eax
    jnz non_multiboot*/

    mov %ebx, %eax
    jmp kernel_start

non_multiboot:
    hlt
