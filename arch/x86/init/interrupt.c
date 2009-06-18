#include <mem/segment.h>
#include <init/asm.h>
#include <core/trap_entry.h>
#include <interrupt.h>

descriptor_t idt[IDT_SIZE];

void set_isr(interrupt_vector_t vector, descriptor_type_t type, dpl_t dpl,
             isr_entry_t entry) {
  gate_descriptor_t gate = {
    .off_high = ((unsigned int) entry) >> 16,
    .present = 1,
    .dpl = dpl,
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
  set_isr(INT_DIVIDE_ERROR, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_divide_error_entry);
  set_isr(INT_NMI, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_nmi_entry);
  set_isr(INT_BREAKPOINT, DT_TRAP_GATE_32, DPL_USER, 
          &trap_breakpoint_entry);
  set_isr(INT_OVERFLOW, DT_TRAP_GATE_32, DPL_USER, 
          &trap_overflow_entry);
  set_isr(INT_BOUND_RANGE, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_bound_range_entry);
  set_isr(INT_INVALID_OPCODE, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_invalid_opcode_entry);
  set_isr(INT_NO_MATH, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_no_math_entry);
  set_isr(INT_DOUBLE_FAULT, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_double_fault_entry);
  set_isr(INT_INVALID_TSS, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_invalid_tss_entry);
  set_isr(INT_NO_SEGMENT, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_no_segment_entry);
  set_isr(INT_SS_FAULT, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_ss_fault_entry);
  set_isr(INT_GP_FAULT, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_gp_fault_entry);
  set_isr(INT_PAGE_FAULT, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_page_fault_entry);
  set_isr(INT_MATH_FAULT, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_math_fault_entry);
  set_isr(INT_ALIGN_CHECK, DT_TRAP_GATE_32, DPL_KERNEL, 
          &trap_align_check_entry);
  /*set_isr(INT_MACHINE_CHECK, DT_TRAP_GATE_32, DPL_KERNEL, 
          &isr_machine_check_entry); */
  /*set_isr(INT_SIMD_FAULT, DT_TRAP_GATE_32, DPL_KERNEL, 
          &isr_simd_fault_entry); */

  // setup syscall trap
  set_isr(INT_SYSCALL, DT_TRAP_GATE_32, DPL_USER, 
          &syscall_handler_entry);

  pseudo_segment_descriptor_t idtd = {
    (sizeof(idt) - 1),
    (unsigned int) &idt
  };
  set_idt(&idtd);
}
