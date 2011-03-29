#ifndef _SEGMENT_H
#define _SEGMENT_H

#include <compiler.h>

// boot-time GDT indices
#define BOOT_NULL_IDX   0
#define BOOT_CS_IDX     1
#define BOOT_DS_IDX     2

#define BOOT_CS (BOOT_CS_IDX*8)
#define BOOT_DS (BOOT_DS_IDX*8)

// real GDT indices
#define NULL_SEG_IDX    0
#define KERNEL_CS_IDX   1
#define KERNEL_DS_IDX   2
#define USER_CS_IDX     3
#define USER_DS_IDX     4
#define GDT_SIZE        5

#define KERNEL_CS (KERNEL_CS_IDX*8)
#define KERNEL_DS (KERNEL_DS_IDX*8)
#define USER_CS (USER_CS_IDX*8)
#define USER_DS (USER_DS_IDX*8)

#ifndef __ASM__

/* IMPORTANT!! the ordering of these bits in each byte is a compiler choice!
   currently, these structs are written the gcc way, which is least significant
   bit first (on little endian machines). note that byte ordering is the same */

// descriptor types
typedef enum descriptor_type {
  DT_NULL = 0,
  DT_TSS_AVAIL_16,
  DT_LDT,
  DT_TSS_BUSY_16,
  DT_CALL_GATE_16,
  DT_TASK_GATE,
  DT_INT_GATE_16,
  DT_TRAP_GATE_16,
  DT_TSS_AVAIL_32 = 9,
  DT_TSS_BUSY_32 = 11,
  DT_CALL_GATE_32,
  DT_INT_32 = 14,
  DT_TRAP_GATE_32,

  DT_DATA,
  DT_DATA_A,
  DT_DATA_W,
  DT_DATA_WA,
  DT_DATA_E,
  DT_DATA_EA,
  DT_DATA_EW,
  DT_DATA_EWA,
  DT_CODE,
  DT_CODE_A,
  DT_CODE_R,
  DT_CODE_RA,
  DT_CODE_C,
  DT_CODE_CA,
  DT_CODE_CR,
  DT_CODE_CRA
} descriptor_type_t;

typedef enum dpl {
  DPL_KERNEL = 0,
  DPL_USER = 3
} dpl_t;

typedef struct segment_descriptor {
  unsigned limit_low:16;        // segment limit (low 16 bits)
  unsigned base_low:24;         // segment base address (low 24 bits)
  descriptor_type_t type:5;     // descriptor type
  dpl_t dpl:2;                  // descriptor privilege level
  unsigned present:1;           // segment present in memory
  unsigned limit_high:4;        // segment limit (high 4 bits)
  unsigned :1;                  // unused
  unsigned reserved:1;          // reserved (should be 0)
  unsigned big:1;               // this is usage in DS, usage in CS is "default"
  unsigned granularity:1;       // 4kb granularity for segment limit
  unsigned base_high:8;         // segment base address (high 8 bits)
} PACK segment_descriptor_t;

typedef struct gate_descriptor {
  unsigned off_low:16;          // gate segment offset (low 16 bits)
  unsigned segment:16;          // gate segment number
  unsigned param_count:5;       // number of word-sized parameters of gate
  unsigned reserved:3;          // reserved (should be 0)
  descriptor_type_t type:5;     // descriptor type
  dpl_t dpl:2;                  // descriptor privelege level
  unsigned present:1;           // gate present in memory
  unsigned off_high:16;         // segment offset of gate (high 16 bits)
} PACK gate_descriptor_t;

typedef struct nonpresent_descriptor {
  unsigned :32;
  unsigned :15;
  unsigned present:1;
  unsigned :16;
} PACK nonpresent_descriptor_t;

typedef union descriptor {
  segment_descriptor_t segment;
  gate_descriptor_t gate;
  nonpresent_descriptor_t nonpresent;
} descriptor_t;

// dont use bitfield here, or PACK will not align 'base' properly
typedef struct pseudo_segment_descriptor {
  unsigned short limit;         // descriptor limit
  unsigned long base;           // descriptor base address
} PACK pseudo_segment_descriptor_t;

#endif /* __ASM__ */

#endif /* _SEGMENT_H */
