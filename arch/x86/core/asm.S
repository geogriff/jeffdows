#define __ASM__
#include <mem/segment.h>

    .text
    
// void set_gdt(pseudo_segment_descriptor_t *gdtd)
    .global set_gdt
set_gdt:
    .type set_gdt, STT_FUNC

    mov 4(%esp), %eax
    lgdt (%eax)                 /* this is a serializing instruction, no
                                   extra prefetch clear needed */
    // set data segment registers0
    mov $KERNEL_DS, %eax
    mov %eax, %ds
    mov %eax, %es
    mov %eax, %fs
    mov %eax, %gs

    // set code segment register (and return)
    ljmp $KERNEL_CS, $set_gdt_cs
set_gdt_cs:
    ret

// void set_idt(pseudo_segment_descriptor_t *idtd)
    .global set_idt
set_idt:
    .type set_idt, STT_FUNC

    mov 4(%esp), %eax
    lidt (%eax)

    ret

// void enable_interrupts()
    .global enable_interrupts
enable_interrupts:
    .type enable_interrupts, STT_FUNC

    sti

    ret

// void disable_interrupts()
    .global disable_interrupts
disable_interrupts:
    .type disable_interrupts, STT_FUNC

    cli

    ret
