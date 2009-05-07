#include <mem/segment.h>
#include <init/asm.h>
#include <core/trap_entry.h>
#include <interrupt.h>

descriptor_t idt[IDT_SIZE];

void set_isr(interrupt_vector_t vector, descriptor_type_t type, 
             isr_entry_t entry) {
  gate_descriptor_t gate = {
    .off_high = ((unsigned int) entry) >> 16,
    .present = 1,
    .dpl = 3,
    .type = type,
    .reserved = 0,
    .param_count = 0,
    .segment = KERNEL_CS,
    .off_low = (((unsigned int) entry) << 16) >> 16
  };
  idt[vector].gate = gate;
}

void clear_isr(interrupt_vector_t vector) {
  nonpresent_descriptor_t nonpresent_gate = {
    .present = 0
  };
  idt[vector].nonpresent = nonpresent_gate;
}

void init_interrupts() {
  // clear out idt
  for (interrupt_vector_t v = INT_FIRST; v <= INT_LAST; v++)
    clear_isr(v);

  // setup processor traps
  set_isr(INT_DIVIDE_ERROR, SDT_TRAP_GATE_32, &trap_divide_error_entry);
  set_isr(INT_NMI, SDT_TRAP_GATE_32, &trap_nmi_entry);
  set_isr(INT_BREAKPOINT, SDT_TRAP_GATE_32, &trap_breakpoint_entry);
  set_isr(INT_OVERFLOW, SDT_TRAP_GATE_32, &trap_overflow_entry);
  set_isr(INT_BOUND_RANGE, SDT_TRAP_GATE_32, &trap_bound_range_entry);
  set_isr(INT_INVALID_OPCODE, SDT_TRAP_GATE_32, &trap_invalid_opcode_entry);
  set_isr(INT_NO_MATH, SDT_TRAP_GATE_32, &trap_no_math_entry);
  set_isr(INT_DOUBLE_FAULT, SDT_TRAP_GATE_32, &trap_double_fault_entry);
  set_isr(INT_INVALID_TSS, SDT_TRAP_GATE_32, &trap_invalid_tss_entry);
  set_isr(INT_NO_SEGMENT, SDT_TRAP_GATE_32, &trap_no_segment_entry);
  set_isr(INT_SS_FAULT, SDT_TRAP_GATE_32, &trap_ss_fault_entry);
  set_isr(INT_GP_FAULT, SDT_TRAP_GATE_32, &trap_gp_fault_entry);
  set_isr(INT_PAGE_FAULT, SDT_TRAP_GATE_32, &trap_page_fault_entry);
  set_isr(INT_MATH_FAULT, SDT_TRAP_GATE_32, &trap_math_fault_entry);
  set_isr(INT_ALIGN_CHECK, SDT_TRAP_GATE_32, &trap_align_check_entry);
  //set_isr(INT_MACHINE_CHECK, SDT_TRAP_GATE_32, &isr_machine_check_entry);
  //set_isr(INT_SIMD_FAULT, SDT_TRAP_GATE_32, &isr_simd_fault_entry);

  // setup syscall trap
  set_isr(INT_SYSCALL, SDT_TRAP_GATE_32, &syscall_handler_entry);

  pseudo_segment_descriptor_t idtd = {
    (sizeof(idt) - 1),
    (unsigned int) &idt
  };
  set_idt(&idtd);
}
