#include <init/init.h>
#include <mem/page.h>
#include <mem/pmem.h>

void init_pmem_segment(phys_mmap_t *mmap, page_t *page_data) {
  pmem_segment_t *pmem_segment = &pmem_segments[pmem_segment_count++];
  pmem_segment->start = mmap->start; 
  pmem_segment->limit = mmap->limit;
  pmem_segment->pages = page_data;
}

void init_pmem_page(phys_addr_t phys_addr) {
  page_t *page = pmem_get_page(phys_addr);
  page->phys_addr = phys_addr;
  page->order = PAGE_ORDER_INUSE;
}
