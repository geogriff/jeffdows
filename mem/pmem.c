#include <stddef.h>
#include <mem/page.h>
#include <mem/pmem.h>

page_t *pmem_pages = NULL;
int pmem_page_count = 0;

pmem_segment_t pmem_segments[MAX_PMEM_SEGMENTS];
int pmem_segment_count = 0;

page_t *freelists[PMEM_FREELIST_COUNT];

void freelist_remove(page_t **freelist, page_t *page) {
  page_t *cur_page = *freelist;
  page_t **prev_ptr = freelist;
  while (cur_page != NULL) {
    if (cur_page == page) {
      *prev_ptr = cur_page->next_free;
      return;
    }
    prev_ptr = &cur_page->next_free;
    cur_page = cur_page->next_free;
  }
}

void freelist_push(page_t **freelist, page_t *page) {
    page->next_free = *freelist;
    *freelist = page;
}

page_t *freelist_pop(page_t **freelist) {
  page_t *page = *freelist;
  *freelist = (*freelist)->next_free;
  return page;
}

page_t *pmem_get_page(phys_addr_t phys_addr) {
  for (pmem_segment_t *seg = &pmem_segments[0]; 
       seg < &pmem_segments[pmem_segment_count]; seg++) {
    if (phys_addr >= seg->start && phys_addr < seg->start + seg->limit) {
      if (phys_addr + PAGE_SIZE <= seg->start + seg->limit) {
	return &seg->pages[(phys_addr - seg->start) / PAGE_SIZE];
      } else {
	break;
      }
    }
  }

  return NULL;
}

pmem_segment_t *pmem_get_seg(phys_addr_t phys_addr) {
  for (pmem_segment_t *seg = &pmem_segments[0]; 
       seg < &pmem_segments[pmem_segment_count]; seg++)
    if (phys_addr >= seg->start && phys_addr < seg->start + seg->limit)
      return seg;

  return NULL;
}

void pmem_free(page_t *page, int order) {
  pmem_segment_t *seg = pmem_get_seg(page->phys_addr);

  // mark page as free and put on freelist
  page->order = order;
  freelist_push(&freelists[page->order], page);

  // merge with other pages as much as possible
  page_t *buddy;
  while (page->order < MAX_PAGE_ORDER && 
         ((buddy = pmem_get_page(page->phys_addr ^ (PAGE_SIZE << page->order)))
	  != NULL) && 
         buddy->phys_addr >= seg->start &&
         buddy->phys_addr < seg->start + seg->limit &&
         buddy->order == page->order) {
    // remove pages from freelist
    freelist_remove(&freelists[page->order], page);
    freelist_remove(&freelists[page->order], buddy);

    // we really want first page struct of the area 
    if (page->phys_addr & (PAGE_SIZE << page->order)) {
      page_t *temp = page;
      page = buddy;
      buddy = temp;
    }
    // merge the two pages
    buddy->order = PAGE_ORDER_INUSE;
    page->order++;

    // add the new area to freelist
    freelist_push(&freelists[page->order], page);
  }
}

page_t *pmem_alloc(int order) {
  int curorder = order;

  // find lowest order freelist with available pages
  while (freelists[curorder] == NULL) {
    curorder++;
    if (curorder > MAX_PAGE_ORDER) {
      return NULL;
    }
  }

  // split free areas until we have a single page
  while (curorder > order) {
    page_t *first_page = freelist_pop(&freelists[curorder]);

    curorder--;

    page_t *buddy_page = pmem_get_page(first_page->phys_addr ^ 
                                       (PAGE_SIZE << curorder));

    if (buddy_page != NULL) {
      buddy_page->order = curorder;
      freelist_push(&freelists[curorder], buddy_page);
    }
    
    first_page->order = curorder;
    freelist_push(&freelists[curorder], first_page);
  }

  // get first available page
  page_t *page = freelist_pop(&freelists[order]);
  // mark page as used
  page->order = PAGE_ORDER_INUSE;
  
  return page;
}
