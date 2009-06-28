#include <init/pmem.h>
#include <mem/page.h>
#include <mem/pmem.h>
#include <stddef.h>
#include <string.h>


void init_mem(phys_mmap_t *phys_mmap) {
  phys_addr_t pmem_data_addr = 0;

  for (phys_mmap_t *mmap = &phys_mmap[0]; !(mmap->limit == 0 && 
                                            mmap->start == 0); mmap++) {
    // align mmap to page boundaries
    mmap->start = PAGE_CEIL(mmap->start);
    mmap->limit = PAGE_FLOOR(mmap->limit);

    // increment total number of pages available to system
    pmem_page_count += mmap->limit / PAGE_SIZE;
  }

  // calculate number of pages needed for pmem data
  size_t pmem_data_size = PAGE_CEIL(pmem_page_count * sizeof(page_t));

  for (phys_mmap_t *mmap = &phys_mmap[0]; !(mmap->limit == 0 && 
                                            mmap->start == 0); mmap++) {
    // find a suitable spot for the pmem allocator data
    if (pmem_data_addr == 0 && mmap->limit >= pmem_data_size) {
      pmem_data_addr = mmap->start;
      mmap->start += pmem_data_size;
      mmap->limit -= pmem_data_size;
    }
  }

  if (pmem_data_addr == 0) {
    //panic
  }

  // initialize page data
  pmem_pages = PA(pmem_data_addr);
  memset(PA(pmem_pages), 0, pmem_data_size);

  int pmem_page_idx = 0;
  for (phys_mmap_t *mmap = &phys_mmap[0]; !(mmap->limit == 0 && 
                                            mmap->start == 0); mmap++) {
    init_pmem_segment(mmap, &pmem_pages[pmem_page_idx]);
    for (phys_addr_t phys_addr = mmap->start;
         phys_addr < mmap->start + mmap->limit; phys_addr += PAGE_SIZE) {
      init_pmem_page(phys_addr);
    }
    pmem_page_idx += mmap->limit / PAGE_SIZE;
  }
}
