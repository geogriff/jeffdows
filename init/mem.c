#include <page.h>

void init_mem(phys_mmap_t *phys_mmap) {
  phys_addr_t pmem_pages = NULL;
  phys_addr_t vmem_pages = NULL;

  for (phys_mmap_t *mmap = &phys_mmap[0]; !(mmap->limit == 0 && 
                                            mmap->base_addr == 0); mmap++) {
    // align mmap to page boundaries
    mmap->base_addr = PAGE_CEIL(mmap->base_addr);
    mmap->limit = PAGE_FLOOR(mmap->limit);

    // increment total number of pages available to system
    page_count += mmap->limit / PAGE_SIZE;
  }

  // calculate number of pages needed for pmem data

  for (phys_mmap_t *mmap = &phys_mmap[0]; !(mmap->limit == 0 && 
                                            mmap->base_addr == 0); mmap++) {
    // find a suitable spot for the pmem allocator data
    if (mmap->limit >= pmem_page_count * PAGE_SIZE) {
      pmem_pages = mmap->base_addr;
      mmap->base_addr += pmem_page_count * PAGE_SIZE;
      mmap->limit -= pmem_page_count * PAGE_SIZE;
    }

    // find a suitable spot for the page_t structures
    if (mmap->limit >= page_count * sizeof(page_t)) {
      vmem_pages = mmap->base_addr;
      mmap->base_addr += page_count * sizeof(page_t);
      mmap->limit -= page_count * sizeof(page_t);
      break;
    }
  }

  if (pmem_pages == NULL || vmem_pages == NULL) {
    //panic
  }

  // initialize physical memory allocator
  init_pmem(pmem_pages);

  // initialize virtual memory allocator
  init_vmem(vmem_pages);
}
n
