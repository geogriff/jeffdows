#include <init/init.h>
#include <init/pmem.h>
#include <mem/page.h>
#include <mem/pmem.h>
#include <core/asm.h>
#include <stddef.h>
#include <string.h>

void init_mem() {
  phys_addr_t pmem_data_addr;

  for (phys_mmap_t *mmap = &boot_info.phys_mmap[0];
       mmap < &boot_info.phys_mmap[boot_info.phys_mmap_count]; mmap++) {
    // align mmap to page boundaries
    mmap->start = PAGE_CEIL(mmap->start);
    mmap->limit = PAGE_FLOOR(mmap->limit);

    // increment total number of pages available to system
    pmem_page_count += mmap->limit / PAGE_SIZE;
  }

  // calculate number of pages needed for pmem data
  size_t pmem_data_size = PAGE_CEIL(pmem_page_count * sizeof(page_t));

  for (phys_mmap_t *mmap = &boot_info.phys_mmap[0];
       mmap < &boot_info.phys_mmap[boot_info.phys_mmap_count]; mmap++) {
    // find a suitable spot for the pmem allocator data
    if (mmap->limit >= pmem_data_size) {
      pmem_pages = PA(mmap->start);
      mmap->start += pmem_data_size;
      mmap->limit -= pmem_data_size;
      break;
    }
  }

  if (pmem_pages == NULL) {
    //panic
  }

  // initialize page data
  memset(pmem_pages, 0, pmem_data_size);

  int pmem_page_idx = 0;
  for (phys_mmap_t *mmap = &boot_info.phys_mmap[0];
       mmap < &boot_info.phys_mmap[boot_info.phys_mmap_count]; mmap++) {
    init_pmem_segment(mmap, &pmem_pages[pmem_page_idx]);
    for (phys_addr_t phys_addr = mmap->start;
         phys_addr < mmap->start + mmap->limit; phys_addr += PAGE_SIZE) {
      init_pmem_page(phys_addr);

      // free anything after kernel code that is not used to store struct pages
      if (phys_addr > VA(&_end)
	  && !(phys_addr + PAGE_SIZE >= VA(pmem_pages)
	       && phys_addr < VA(&pmem_pages[pmem_page_count]))) {
	pmem_free(pmem_get_page(phys_addr));
      }
    }
    pmem_page_idx += mmap->limit / PAGE_SIZE;
  }
}
