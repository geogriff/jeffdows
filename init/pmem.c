#include <mem/page.h>

int page_count;

void init_pmem(phys_mmap_t *phys_mmap, phys_addr_t pmem_pages) {
  void *pmem_data = PA(pmem_pages);
  // pmem bitmap sits at beginning of pmem_data
  char *pmem_bitmap = pmem_pages;

  // mark all pages as used in bitmap
  memset(pmem_bitmap, 0, page_count / 8);

  for (phys_mmap_t *mmap = &phys_mmap[0]; !(mmap->limit == 0 && 
                                            mmap->base_addr == 0); mmap++) {
    for (phys_addr_t pa = mmap->base_addr; pa < mmap->base_addr + mmap->limit;
         pa += PAGE_SIZE) {
      // verify that page is available for general use
      if (PA(pa) > &end && 
          pa < pmem_pages || pa > pmem_pages + pmem_page_count * PAGE_SIZE) {
        // free available page
        pmem_free_page(PA(pa));
      }
    }
  }
}
