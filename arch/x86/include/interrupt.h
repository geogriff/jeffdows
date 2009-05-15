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
  INT_BREAKPOINT = 3,
  INT_OVERFLOW = 4,
  INT_BOUND_RANGE = 5,
  INT_INVALID_OPCODE = 6,
  INT_NO_MATH = 7,
  INT_DOUBLE_FAULT = 8,
  INT_INVALID_TSS = 10,
  INT_NO_SEGMENT = 11,
  INT_SS_FAULT = 12,
  INT_GP_FAULT = 13,
  INT_PAGE_FAULT = 14,
  INT_MATH_FAULT = 16,
  INT_ALIGN_CHECK = 17,
  INT_MACHINE_CHECK = 18,
  INT_SIMD_FAULT = 19,
  INT_EXTERNAL = 32,
  INT_SYSCALL = 32,
  INT_LAST = 255
} interrupt_vector_t;
#endif /* __ASM__ */

#ifndef __ASM__

#define IDT_SIZE (INT_LAST+1)

typedef struct trapped_state {
  long eax;
  long ebx;
  long ecx;
  long edx;
  long ebp;
  long esi;
  long edi;
  long ds;
  long es;
  long fs;
  long gs;
  // members provided by cpu always
  long eip;
  long cs;
  long eflags;
  // members provided by cpu only on mode switch
  long esp;
  long ss;
} trapped_state_t;

typedef struct trap_error_code {
  unsigned external:1;      // external event caused this interrupt
  unsigned idt:1;           // segment is in the idt
  unsigned local:1;         // segment is in an ldt
  unsigned segment_idx:13;  // segment index
  unsigned :16;             // reserved
} trap_error_code_t;

typedef void *isr_entry_t;

#endif /* __ASM__ */

#endif /* _INTERRUPT_H */
