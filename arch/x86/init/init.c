#include <core/printf.h>
#include <mem/segment.h>
#include <mem/page.h>
#include <mem/asm.h>
#include "init.h"

segment_descriptor_t gdt[] ALIGN(8) = {
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
  // set up gdt
  pseudo_segment_descriptor_t gdtd = {
    (sizeof(gdt) - 1),
    (unsigned) &gdt
  };
  set_gdt(&gdtd);

  // set up nonpresent user pages in page dir
  pde_nonpresent_t nonpresent_page = {
    0       // present
  };
  for (int i = 0; i < (KERNEL_BASE>>22); i++)
    page_dir[i].nonpresent = nonpresent_page;

  // set up 4mb kernel code pages in page dir
  pde_bigpage_t kernel_page = {
    1,      // present
    1,      // writable
    0,      // user
    0,      // write_through
    0,      // cache_disable
    0,      // accessed
    0,      // dirty
    1,      // bigpage
    1,      // global
    0,      // pat
    0,      // reserved
    0       // base_addr
  };
  for (; kernel_page.base_addr <= (((unsigned int) &_end) - KERNEL_BASE) >> 22;
       kernel_page.base_addr++)
    page_dir[kernel_page.base_addr + (KERNEL_BASE>>22)].bigpage = kernel_page;

  // clear rest of kernel pages
  for (int i = (((unsigned int) &_end) >> 22) + 1; 
       i < 1024; i++)
    page_dir[i].nonpresent = nonpresent_page;
}
