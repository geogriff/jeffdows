#include <stddef.h>
#include <mem/pmem.h>

page_t *pmem_pages;
int pmem_page_count = 0;

pmem_segment_t pmem_segments[MAX_PMEM_SEGMENTS];
int pmem_segment_count = 0;

page_t *pmem_get_page(phys_addr_t phys_addr) {
  return NULL;
}
