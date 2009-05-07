#ifndef _TRAP_ENTRY_H
#define _TRAP_ENTRY_H

void trap_divide_error_entry();
void trap_nmi_entry();
void trap_breakpoint_entry();
void trap_overflow_entry();
void trap_bound_range_entry();
void trap_invalid_opcode_entry();
void trap_no_math_entry();
void trap_double_fault_entry();
void trap_invalid_tss_entry();
void trap_no_segment_entry();
void trap_ss_fault_entry();
void trap_gp_fault_entry();
void trap_page_fault_entry();
void trap_math_fault_entry();
void trap_align_check_entry();

#endif /* _TRAP_ENTRY_H */
