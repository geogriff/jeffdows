#include <stddef.h>
#include <mem/page.h>
#include <mem/pmem.h>

page_t *pmem_pages = NULL;
int pmem_page_count = 0;

pmem_segment_t pmem_segments[MAX_PMEM_SEGMENTS];
int pmem_segment_count = 0;

page_t *pmem_get_page(phys_addr_t phys_addr) {
  for (pmem_segment_t *seg = &pmem_segments[0]; 
       seg < &pmem_segments[pmem_segment_count]; seg++)
    if (phys_addr >= seg->start && phys_addr < seg->start + seg->limit)
      return &seg->pages[(phys_addr - seg->start) / PAGE_SIZE];

  return NULL;
}

pmem_segment_t *pmem_get_seg(phys_addr_t phys_addr) {
  for (pmem_segment_t *seg = &pmem_segments[0]; 
       seg < &pmem_segments[pmem_segment_count]; seg++)
    if (phys_addr >= seg->start && phys_addr < seg->start + seg->limit)
      return seg;

  return NULL;
}

void pmem_free(page_t *page) {
  page_t *buddy = pmem_get_page(page->phys_addr ^ PAGE_SIZE);
  pmem_segment_t *seg = pmem_get_seg(page->phys_addr);
  while (buddy != NULL && 
         buddy->phys_addr >= seg->start &&
         buddy->phys_addr < seg->start + seg->limit &&
         buddy->order == page->order) {
    // we want first page struct of the area 
    if (page->phys_addr & (PAGE_SIZE << page->order)) {
      page_t *temp = page;
      page = buddy;
      buddy = temp;
    }
    buddy->order = PAGE_ORDER_INUSE;
    page->order++;

    buddy = pmem_get_page(page->phys_addr ^ (PAGE_SIZE << page->order));
  }
}

page_t *pmem_alloc() {
  
}
