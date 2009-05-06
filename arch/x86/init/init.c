#include <core/printf.h>
#include <mem/segment.h>
#include <mem/page.h>
#include <init/asm.h>
#include <init/interrupt.h>
#include "init.h"

segment_descriptor_t gdt[] ALIGN(8) = {
  { // null descriptor
    .base_high = 0x00,      // high 8 bits of segment base address
    .granularity = 0b0,     // granularity flag
    .big = 0b0,             // big flag
    .reserved = 0b0,        // reserved bit (must be 0)
    .limit_high = 0x0,      // high 4 bits of segment limit
    .present = 0b0,         // present flag
    .dpl = 0b00,            // descriptor privelege level
    .type = SDT_NULL,       // descriptor type
    .base_low = 0x000000,   // low 24 bits of segment base address
    .limit_low = 0x0000     // low 16 bits of segment limit
  },
  { // kernel code descriptor
    .base_high = 0x00,      // high 8 bits of segment base address
    .granularity = 0b1,     // granularity flag
    .big = 0b1,             // big/default flag
    .reserved = 0b0,        // reserved bit (must be 0)
    .limit_high = 0xF,      // high 4 bits of segment limit
    .present = 0b1,         // present flag
    .dpl = 0b00,            // descriptor privelege level
    .type = SDT_CODE_RA,    // descriptor type
    .base_low = 0x000000,   // low 24 bits of segment base address
    .limit_low = 0xFFFF     // low 16 bits of segment limit
  },
  { // kernel data descriptor
    .base_high = 0x00,      // high 8 bits of segment base address
    .granularity = 0b1,     // granularity flag
    .big = 0b1,             // big/default flag
    .reserved = 0b0,        // reserved bit (must be 0)
    .limit_high = 0xF,      // high 4 bits of segment limit
    .present = 0b1,         // present flag
    .dpl = 0b00,            // descriptor privelege level
    .type = SDT_DATA_WA,    // descriptor type
    .base_low = 0x000000,   // low 24 bits of segment base address
    .limit_low = 0xFFFF     // low 16 bits of segment limit
  },
  { // user code descriptor
    .base_high = 0x00,      // high 8 bits of segment base address
    .granularity = 0b1,     // granularity flag
    .big = 0b1,             // big/default flag
    .reserved = 0b0,        // reserved bit (must be 0)
    .limit_high = 0xF,      // high 4 bits of segment limit
    .present = 0b1,         // present flag
    .dpl = 0b11,            // descriptor privelege level
    .type = SDT_CODE_RA,    // descriptor type
    .base_low = 0x000000,   // low 24 bits of segment base address
    .limit_low = 0xFFFF     // low 16 bits of segment limit
  },
  { // user data descriptor
    .base_high = 0x00,      // high 8 bits of segment base address
    .granularity = 0b1,     // granularity flag
    .big = 0b1,             // big/default flag
    .reserved = 0b0,        // reserved bit (must be 0)
    .limit_high = 0xF,      // high 4 bits of segment limit
    .present = 0b1,         // present flag
    .dpl = 0b11,            // descriptor privelege level
    .type = SDT_DATA_WA,    // descriptor type
    .base_low = 0x000000,   // low 24 bits of segment base address
    .limit_low = 0xFFFF     // low 16 bits of segment limit
  }
};

void init_x86(boot_info_t *boot_info) {
  // set up gdt
  pseudo_segment_descriptor_t gdtd = {
    .limit = (sizeof(gdt) - 1),
    .base = (unsigned) &gdt
  };
  set_gdt(&gdtd);

  // set up nonpresent user pages in page dir
  pde_nonpresent_t nonpresent_page = {
    .present = 0       // present
  };
  for (int i = 0; i < (KERNEL_BASE>>22); i++)
    page_dir[i].nonpresent = nonpresent_page;

  // set up 4mb kernel code pages in page dir
  pde_bigpage_t kernel_page = {
    .base_addr = 0,
    .reserved = 0,
    .pat = 0,
    .global = 1,
    .bigpage = 1,
    .dirty = 0,
    .accessed = 0,
    .cache_disable = 0,
    .write_through = 0,
    .user = 0,
    .writable = 1,
    .present = 1
  };
  for (; kernel_page.base_addr <= (((unsigned int) &_end) - KERNEL_BASE) >> 22;
       kernel_page.base_addr++)
    page_dir[kernel_page.base_addr + (KERNEL_BASE>>22)].bigpage = kernel_page;

  // clear rest of kernel pages
  for (int i = (((unsigned int) &_end) >> 22) + 1; 
       i < 1024; i++)
    page_dir[i].nonpresent = nonpresent_page;

  init_interrupts();
}
