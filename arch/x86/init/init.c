#include <core/printf.h>
#include <mem/segment.h>
#include <mem/page.h>
#include <core/asm.h>
#include <init/init.h>
#include <init/interrupt.h>
#include <interrupt.h>
#include <boot/multiboot.h>

segment_descriptor_t gdt[] ALIGN(8) = {
  { // null descriptor
    .base_high = 0,         // high 8 bits of segment base address
    .granularity = 0,       // granularity flag
    .big = 0,               // big flag
    .reserved = 0,          // reserved bit (must be 0)
    .limit_high = 0,        // high 4 bits of segment limit
    .present = 0,           // present flag
    .dpl = 0,               // descriptor privelege level
    .type = DT_NULL,        // descriptor type
    .base_low = 0,          // low 24 bits of segment base address
    .limit_low = 0          // low 16 bits of segment limit
  },
  { // kernel code descriptor
    .base_high = 0,         // high 8 bits of segment base address
    .granularity = 1,       // granularity flag
    .big = 1,               // big/default flag
    .reserved = 0,          // reserved bit (must be 0)
    .limit_high = 0xF,      // high 4 bits of segment limit
    .present = 1,           // present flag
    .dpl = DPL_KERNEL,      // descriptor privelege level
    .type = DT_CODE_RA,     // descriptor type
    .base_low = 0,          // low 24 bits of segment base address
    .limit_low = 0xFFFF     // low 16 bits of segment limit
  },
  { // kernel data descriptor
    .base_high = 0,         // high 8 bits of segment base address
    .granularity = 1,       // granularity flag
    .big = 1,               // big/default flag
    .reserved = 0,          // reserved bit (must be 0)
    .limit_high = 0xF,      // high 4 bits of segment limit
    .present = 1,           // present flag
    .dpl = DPL_KERNEL,      // descriptor privelege level
    .type = DT_DATA_WA,     // descriptor type
    .base_low = 0,          // low 24 bits of segment base address
    .limit_low = 0xFFFF     // low 16 bits of segment limit
  },
  { // user code descriptor
    .base_high = 0,         // high 8 bits of segment base address
    .granularity = 1,       // granularity flag
    .big = 1,               // big/default flag
    .reserved = 0,          // reserved bit (must be 0)
    .limit_high = 0xF,      // high 4 bits of segment limit
    .present = 1,           // present flag
    .dpl = DPL_USER,        // descriptor privelege level
    .type = DT_CODE_RA,     // descriptor type
    .base_low = 0,          // low 24 bits of segment base address
    .limit_low = 0xFFFF     // low 16 bits of segment limit
  },
  { // user data descriptor
    .base_high = 0,         // high 8 bits of segment base address
    .granularity = 1,       // granularity flag
    .big = 1,               // big/default flag
    .reserved = 0,          // reserved bit (must be 0)
    .limit_high = 0xF,      // high 4 bits of segment limit
    .present = 1,           // present flag
    .dpl = DPL_USER,        // descriptor privelege level
    .type = DT_DATA_WA,     // descriptor type
    .base_low = 0,          // low 24 bits of segment base address
    .limit_low = 0xFFFF     // low 16 bits of segment limit
  }
};

void init_x86(multiboot_info_t *multiboot_info) {
  // get boot info from multiboot struct
  if (multiboot_info->flags & MULTIBOOT_FLAG_MMAP) {
    phys_mmap_t *phys_mmap = &boot_info.phys_mmap[0];
    for (mmap_info_t *multiboot_mmap = 
           (mmap_info_t *) multiboot_info->mmap_addr;
         (void *) multiboot_mmap < (void *) (multiboot_info->mmap_addr + 
                                             multiboot_info->mmap_length);
         multiboot_mmap++) {
      if (phys_mmap >= &boot_info.phys_mmap[MAX_MMAP]) {
        //panic
      }

      if (multiboot_mmap->type == 1) {
        phys_mmap->start = multiboot_mmap->base_addr_low;
        phys_mmap->limit = multiboot_mmap->length_low;
        phys_mmap++;
        boot_info.phys_mmap_count++;
      }

    }
  } else {
    // panic
  }

  // set up gdt
  pseudo_segment_descriptor_t gdtd = {
    .limit = (sizeof(gdt) - 1),
    .base = (uint32_t) &gdt
  };
  set_gdt(&gdtd);

  // set up nonpresent user pages in page dir
  pde_nonpresent_t nonpresent_page = {
    .present = 0       // present
  };
  for (uint_fast16_t i = 0; i < (KERNEL_BASE>>22); i++)
    page_dir[i].nonpresent = nonpresent_page;

  // map first 1GB of physical memory to KERNEL_BASE
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
  for (uint_fast16_t i = 0; i < 1024; i++, kernel_page.base_addr++)
    page_dir[kernel_page.base_addr + (KERNEL_BASE>>22)].bigpage = kernel_page;

  init_interrupts();
}
