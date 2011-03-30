#ifndef _MEM_PMEM_H
#define _MEM_PMEM_H

#include <types.h>
#include <stddef.h>

#define MAX_PMEM_SEGMENTS 128
#define MAX_PAGE_ORDER 12

#define PMEM_FREELIST_COUNT (MAX_PAGE_ORDER)

#define PAGE_ORDER_INUSE (MAX_PAGE_ORDER+1)

typedef struct page {
  phys_addr_t phys_addr;
  int order;
  struct page *next_free;
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

void freelist_remove(page_t **freelist, page_t *page);
void freelist_push(page_t **freelist, page_t *page);
page_t *freelist_pop(page_t **freelist);
page_t *pmem_get_page(phys_addr_t phys_addr);
pmem_segment_t *pmem_get_seg(phys_addr_t phys_addr);
void pmem_free(page_t *page, int order);
page_t *pmem_alloc(int order);

#endif /* _MEM_PMEM_H */
