#define __ASM__
#include <interrupt.h>

    .global trap_divide_error_entry
trap_divide_error_entry:
    .type trap_divide_error_entry, STT_FUNC
    push $INT_DIVIDE_ERROR
    jmp trap_handler_entry

    .global trap_nmi_entry
trap_nmi_entry:
    .type trap_nmi_entry, STT_FUNC
    push $INT_NMI
    jmp trap_handler_entry

    .global trap_breakpoint_entry
trap_breakpoint_entry:
    .type trap_breakpoint_entry, STT_FUNC
    push $INT_BREAKPOINT
    jmp trap_handler_entry

    .global trap_overflow_entry
trap_overflow_entry:
    .type trap_overflow_entry, STT_FUNC
    push $INT_OVERFLOW
    jmp trap_handler_entry

    .global trap_bound_range_entry
trap_bound_range_entry:
    .type trap_bound_range_entry, STT_FUNC
    push $INT_BOUND_RANGE
    jmp trap_handler_entry

    .global trap_invalid_opcode_entry
trap_invalid_opcode_entry:
    .type trap_invalid_opcode_entry, STT_FUNC
    push $INT_INVALID_OPCODE
    jmp trap_handler_entry

    .global trap_no_math_entry
trap_no_math_entry:
    .type trap_no_math_entry, STT_FUNC
    push $INT_NO_MATH
    jmp trap_handler_entry

    .global trap_double_fault_entry
trap_double_fault_entry:
    .type trap_double_fault_entry, STT_FUNC
    push $INT_DOUBLE_FAULT
    jmp trap_handler_entry

    .global trap_invalid_tss_entry
trap_invalid_tss_entry:
    .type trap_invalid_tss_entry, STT_FUNC
    push $INT_INVALID_TSS
    jmp trap_handler_entry

    .global trap_no_segment_entry
trap_no_segment_entry:
    .type trap_no_segment_entry, STT_FUNC
    push $INT_NO_SEGMENT
    jmp trap_handler_entry

    .global trap_ss_fault_entry
trap_ss_fault_entry:
    .type trap_ss_fault_entry, STT_FUNC
    push $INT_SS_FAULT
    jmp trap_handler_entry

    .global trap_gp_fault_entry
trap_gp_fault_entry:
    .type trap_gp_fault_entry, STT_FUNC
    push $INT_GP_FAULT
    jmp trap_handler_entry

    .global trap_page_fault_entry
trap_page_fault_entry:
    .type trap_page_fault_entry, STT_FUNC
    push $INT_PAGE_FAULT
    jmp trap_handler_entry

    .global trap_math_fault_entry
trap_math_fault_entry:
    .type trap_math_fault_entry, STT_FUNC
    push $INT_MATH_FAULT
    jmp trap_handler_entry

    .global trap_align_check_entry
trap_align_check_entry:
    .type trap_align_check_entry, STT_FUNC
    push $INT_ALIGN_CHECK
    jmp trap_handler_entry

trap_handler_entry:
    // set up trapped_state struct
    push %es
    push %ds
    push %edi
    push %esi
    push %ebp
    push %edx
    push %ecx
    push %ebx
    push %eax
    /* push pointer to our struct, so compiler doesn't optimize out the changes
       to the struct in trap_handler */
    push %esp
    // get error code, replace with gs, push error code in front
    mov 44(%esp), %eax
    mov %gs, 44(%esp)
    push %eax
    // get interrupt vector, replace with fs, push vector in front
    mov 44(%esp), %eax
    mov %fs, 44(%esp)
    push %eax

    // call generic trap handler
    call trap_handler

    /* remove interrupt vector, error code, and trapped_state struct pointer
       from stack */
    add $12, %esp
    // restore machine state to our trapped_state struct
    pop %eax
    pop %ebx
    pop %ecx
    pop %edx
    pop %ebp
    pop %esi
    pop %edi
    pop %ds
    pop %es
    pop %fs
    pop %gs

    iret

    .global syscall_handler_entry
syscall_handler_entry:
    .type syscall_handler_entry, STT_FUNC
    // set up trapped_state struct
    push %gs
    push %fs
    push %es
    push %ds
    push %edi
    push %esi
    push %ebp
    push %edx
    push %ecx
    push %ebx
    push %eax
    /* push pointer to our struct, so compiler doesn't optimize out the changes
       to the struct in trap_handler */
    push %esp

    // call generic trap handler
    call syscall_handler

    // trapped_state struct pointer from stack
    add $4, %esp
    // restore machine state to our trapped_state struct
    pop %eax
    pop %ebx
    pop %ecx
    pop %edx
    pop %ebp
    pop %esi
    pop %edi
    pop %ds
    pop %es
    pop %fs
    pop %gs

    iret
