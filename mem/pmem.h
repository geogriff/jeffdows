#ifndef _MEM_PMEM_H
#define _MEM_PMEM_H

#include <types.h>

#define MAX_PMEM_SEGMENTS 128

typedef struct phys_mmap {
  phys_addr_t start;
  size_t limit;
} phys_mmap_t;

typedef struct page {
  phys_addr_t phys_addr;
} page_t;

typedef struct pmem_segment {
  phys_addr_t start;
  size_t limit;
  page_t *pages;
} pmem_segment_t;

extern page_t *pmem_pages;
extern int pmem_page_count;

extern pmem_segment_t pmem_segments[MAX_PMEM_SEGMENTS];
extern int pmem_segment_count;

page_t *pmem_get_page(phys_addr_t phys_addr);

#endif /* _MEM_PMEM_H */
