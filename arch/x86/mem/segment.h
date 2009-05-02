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

typedef struct segment_descriptor {
  unsigned limit_low:16;        // segment limit (low 16 bits)
  unsigned base_low:24;         // segment base address (low 24 bits)
  unsigned type:5;              // descriptor type
  unsigned dpl:2;               // descriptor privilege level
  unsigned present:1;           // segment present in memory
  unsigned limit_high:4;        // segment limit (high 4 bits)
  unsigned :1;                  // unused
  unsigned reserved:1;          // reserved (should be 0)
  unsigned big:1;               // this is usage in DS, usage in CS is "default"
  unsigned granularity:1;       // 4kb granularity for segment limit
  unsigned base_high:8;         // segment base address (high 8 bits)
} PACK segment_descriptor_t;

// descriptor types
#define SDT_NULL            0b00000
#define SDT_TSS_AVAIL_16    0b00001
#define SDT_LDT             0b00010
#define SDT_TSS_BUSY_16     0b00011
#define SDT_CALL_GATE_16    0b00100
#define SDT_TASK_GATE       0b00101
#define SDT_INT_GATE_16     0b00110
#define SDT_TRAP_GATE_16    0b00111
#define SDT_TSS_AVAIL_32    0b01001
#define SDT_TSS_BUSY_32     0b01011
#define SDT_CALL_GATE_32    0b01100
#define SDT_INT_32          0b01110
#define SDT_TRAP_GATE_32    0b01111

#define SDT_DATA            0b10000
#define SDT_DATA_A          0b10001
#define SDT_DATA_W          0b10010
#define SDT_DATA_WA         0b10011
#define SDT_DATA_E          0b10100
#define SDT_DATA_EA         0b10101
#define SDT_DATA_EW         0b10110
#define SDT_DATA_EWA        0b10111
#define SDT_CODE            0b11000
#define SDT_CODE_A          0b11001
#define SDT_CODE_R          0b11010
#define SDT_CODE_RA         0b11011
#define SDT_CODE_C          0b11100
#define SDT_CODE_CA         0b11101
#define SDT_CODE_CR         0b11110
#define SDT_CODE_CRA        0b11111

typedef struct gate_descriptor {
  unsigned off_low:16;          // gate segment offset (low 16 bits)
  unsigned segment:16;          // gate segment number
  unsigned param_count:5;       // number of word-sized parameters of gate
  unsigned reserved:3;          // reserved (should be 0)
  unsigned type:5;              // descriptor type
  unsigned dpl:2;               // descriptor privelege level
  unsigned present:1;           // gate present in memory
  unsigned off_high:16;         // segment offset of gate (high 16 bits)
} PACK gate_descriptor_t;

typedef struct pseudo_segment_descriptor {
  unsigned short limit;         // descriptor limit
  unsigned long base;           // descriptor base address
} PACK pseudo_segment_descriptor_t;

#endif /* __ASM__ */

#endif /* _SEGMENT_H */
