#include <stddef.h>
#include <mem/page.h>
#include <mem/pmem.h>

page_t *pmem_pages;
int pmem_page_count = 0;

pmem_segment_t pmem_segments[MAX_PMEM_SEGMENTS];
int pmem_segment_count = 0;

page_t *pmem_get_page(phys_addr_t phys_addr) {
  for (pmem_segment_t *seg = &pmem_segments[0]; 
       seg < &pmem_segments[pmem_segment_count]; seg++)
    if (phys_addr > seg->start && phys_addr < seg->start + seg->limit)
      return &seg->pages[(phys_addr - seg->start) / PAGE_SIZE];

  return NULL;
}
