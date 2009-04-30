#include <core/printf.h>
#include <mem/segment.h>
#include <mem/asm.h>
#include "init.h"

segment_descriptor_t gdt[] = {
  { // null descriptor
    0x0000,     // low 16 bits of segment limit
    0x000000,   // low 24 bits of segment base address
    SDT_NULL,   // descriptor type
    0b00,       // descriptor privelege level
    0b0,        // present flag
    0x0,        // high 4 bits of segment limit
    0b0,        // reserved bit (must be 0)
    0b0,        // big flag
    0b0,        // granularity flag
    0x00        // high 8 bits of segment base address
  },
  { // kernel code descriptor
    0xFFFF,     // low 16 bits of segment limit
    0x000000,   // low 24 bits of segment base address
    SDT_CODE_RA,// descriptor type
    0b00,       // descriptor privelege level
    0b1,        // present flag
    0xF,        // high 4 bits of segment limit
    0b0,        // reserved bit (must be 0)
    0b1,        // big/default flag
    0b1,        // granularity flag
    0x00        // high 8 bits of segment base address
  },
  { // kernel data descriptor
    0xFFFF,     // low 16 bits of segment limit
    0x000000,   // low 24 bits of segment base address
    SDT_DATA_WA,// descriptor type
    0b00,       // descriptor privelege level
    0b1,        // present flag
    0xF,        // high 4 bits of segment limit
    0b0,        // reserved bit (must be 0)
    0b1,        // big/default flag
    0b1,        // granularity flag
    0x00        // high 8 bits of segment base address
  },
  { // user code descriptor
    0xFFFF,     // low 16 bits of segment limit
    0x000000,   // low 24 bits of segment base address
    SDT_CODE_RA,// descriptor type
    0b11,       // descriptor privelege level
    0b1,        // present flag
    0xF,        // high 4 bits of segment limit
    0b0,        // reserved bit (must be 0)
    0b1,        // big/default flag
    0b1,        // granularity flag
    0x00        // high 8 bits of segment base address
  },
  { // user data descriptor
    0xFFFF,     // low 16 bits of segment limit
    0x000000,   // low 24 bits of segment base address
    SDT_DATA_WA,// descriptor type
    0b11,       // descriptor privelege level
    0b1,        // present flag
    0xF,        // high 4 bits of segment limit
    0b0,        // reserved bit (must be 0)
    0b1,        // big/default flag
    0b1,        // granularity flag
    0x00        // high 8 bits of segment base address
  }
};

void init_x86(boot_info_t *boot_info) {
  pseudo_segment_descriptor_t gdtd = {
    (sizeof(gdt) - 1),
    (unsigned) &gdt
  };
  set_gdt(&gdtd);
}
