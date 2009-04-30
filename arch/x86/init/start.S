#define __ASM__
#include <mem/page.h>
#include <mem/segment.h>

#define BOOT_STACK_SIZE 0x4000

#define BOOT_PDE_FLAGS 0b0000000000000011100111

    .global kernel_start
kernel_start:
    /* note that we have a physical address to a boot_info_t in eax. for now, we
    will just stick the pointer in esi for later use. also, until paging is
    set up, every pointer must have KERNEL_BASE subtracted from it (remember
    short jumps don't need this) */

    // TODO: there should be a well-defined state on entry to kernel_start
    mov %eax, %esi

    // set up boot gdt, set data segment registers
    lgdt (boot_gdtd-KERNEL_BASE)
    mov $BOOT_DS, %eax
    mov %eax, %ds
    mov %eax, %es
    mov %eax, %fs
    mov %eax, %gs

    // initialize identity 4MB pages in page dir
    mov $(((0x00000000 >> 22) << 22) + \
          (BOOT_PDE_FLAGS)), %eax               // initial pde value
    mov $(boot_page_dir_base-KERNEL_BASE), %edi // pointer to first pde
    mov $(_end-KERNEL_BASE), %ecx               // calculate number of pages
    shr $22, %ecx
    inc %ecx            /* this might cause an extra 4MB page to be mapped if
                        size of kernel image happens to be divisible by 4MB */
identity_bigpage_init_loop:  
    stosl                               // store pde value, increment di
    add $(1 << 22), %eax                // next pde points to next 4MB mem block
    loop identity_bigpage_init_loop     // loop till (--ecx == 0)

    // initialize kernel 4MB pages in page dir
    mov $(((0x00000000 >> 22) << 22) + \
         (BOOT_PDE_FLAGS)), %eax                // initial pde value
    mov $((boot_page_dir_base-KERNEL_BASE) + \
          ((KERNEL_BASE >> 22) << 2)), %edi     // pointer to first pde
    mov $(_end-KERNEL_BASE), %ecx
    shr $22, %ecx                               // calulate number of pages
    inc %ecx                                    // same issue here as above
kernel_bigpage_init_loop:
    stosl                               // store pde value, increment di
    add $(1 << 22), %eax                // next pde points to next 4MB mem block
    loop kernel_bigpage_init_loop       // loop till (--ecx == 0)

    // enable 4MB pages (PSE bit 4 in cr4)
    mov %cr4, %eax
    orl $(1 << 4), %eax
    mov %eax, %cr4

    // set page dir (bits 12-31 in cr3)
    mov $(boot_page_dir_base-KERNEL_BASE), %eax
    mov %eax, %cr3

    // enable paging (PE bit 31 in cr0)
    mov %cr0, %eax
    orl $(1 << 31), %eax
    mov %eax, %cr0

    // set CS and jump to kernel pages
    ljmp $BOOT_CS, $kernel_real_start

kernel_real_start:
    // we are no longer offset by KERNEL_BASE

    // setup boot stack
    mov $(stack+BOOT_STACK_SIZE), %esp

    // reset eflags
    pushl $0
    popf

    // do i386 initialization using info given by bootloader
    pushl %esi
    call init_x86

    // jump to machine-independent main function (and never return)
    call main

    hlt

    .data

    // boot page directory - should probably be moved to .bss and zeroed on boot
    .global boot_page_dir_base
    .align 4096
boot_page_dir_base:
    .skip 1024

    /* boot gdt pseudo-descriptor - address must be congruent to 2 mod 4
    so that we get an aligned word, then an aligned double word. */
    .global boot_gdtd
    .align 4
    .word 0                                     // align to 2 mod 4
boot_gdtd:
    .word (boot_gdt_end - boot_gdt) - 1         // gdt size - 1
    .long (boot_gdt - KERNEL_BASE)              // gdt pointer

    /* boot gdt - for more information about the segment descriptors, see
    intel x86 manual */
    .global boot_gdt
    .align 8                    // align on 8-byte boundary for best performance
boot_gdt:
    .fill BOOT_CS_IDX, 8      // make null descriptors leading up to BOOT_CS
    // kernel code segment, base 0, limit 4GB, DPL 0
    .long 0b00000000000000001111111111111111
    .long 0b00000000110011111001101000000000
    // kernel data segment, base 0, limit 4GB, DPL 0
    .long 0b00000000000000001111111111111111
    .long 0b00000000110011111001001000000000
boot_gdt_end:

    // boot stack
    .comm stack, BOOT_STACK_SIZE
