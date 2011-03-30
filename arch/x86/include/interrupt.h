#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#ifdef __ASM__

#define INT_FIRST 0
#define INT_DIVIDE_ERROR 0
#define INT_NMI 2
#define INT_BREAKPOINT 3
#define INT_OVERFLOW 4
#define INT_BOUND_RANGE 5
#define INT_INVALID_OPCODE 6
#define INT_NO_MATH 7
#define INT_DOUBLE_FAULT 8
#define INT_INVALID_TSS 10
#define INT_NO_SEGMENT 11
#define INT_SS_FAULT 12
#define INT_GP_FAULT 13
#define INT_PAGE_FAULT 14
#define INT_MATH_FAULT 16
#define INT_ALIGN_CHECK 17
#define INT_MACHINE_CHECK 18
#define INT_SIMD_FAULT 19
#define INT_EXTERNAL 32
#define INT_SYSCALL 32
#define INT_LAST 25

#else

typedef enum interrupt_vector {
  INT_FIRST = 0,
  INT_DIVIDE_ERROR = 0,
  INT_NMI = 2,
  INT_BREAKPOINT,
  INT_OVERFLOW,
  INT_BOUND_RANGE,
  INT_INVALID_OPCODE,
  INT_NO_MATH,
  INT_DOUBLE_FAULT,
  INT_INVALID_TSS,
  INT_NO_SEGMENT,
  INT_SS_FAULT,
  INT_GP_FAULT,
  INT_PAGE_FAULT,
  INT_MATH_FAULT = 16,
  INT_ALIGN_CHECK,
  INT_MACHINE_CHECK,
  INT_SIMD_FAULT,
  INT_EXTERNAL = 32,
  INT_SYSCALL = 32,
  INT_LAST = 255
} interrupt_vector_t;
#endif /* __ASM__ */

#ifndef __ASM__

#include <stdbool.h>
#include <stdint.h>

#define IDT_SIZE (INT_LAST+1)

typedef struct trapped_state {
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;
  uint32_t ds;
  uint32_t es;
  uint32_t fs;
  uint32_t gs;
  // members provided by cpu always
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  // members provided by cpu only on mode switch
  uint32_t esp;
  uint32_t ss;
} trapped_state_t;

typedef struct trap_error_code {
  bool external:1;          // external event caused this interrupt
  bool idt:1;               // segment is in the idt
  bool local:1;             // segment is in an ldt
  uint16_t segment_idx:13;  // segment index
  uint16_t :16;             // reserved
} trap_error_code_t;

typedef void *isr_entry_t;

#endif /* __ASM__ */

#endif /* _INTERRUPT_H */
