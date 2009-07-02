#ifndef _INIT_PMEM_H
#define _INIT_PMEM_H

#include <init/mem.h>
#include <mem/pmem.h>
#include <types.h>

void init_pmem_segment(phys_mmap_t *mmap, page_t *page_data);
void init_pmem_page(phys_addr_t phys_addr);

#endif /* _INIT_PMEM_H */
