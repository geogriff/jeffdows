#ifndef _INIT_INIT_H
#define _INIT_INIT_H

#include <init/mem.h>
#include <mem/pmem.h>

#define MAX_MMAP (MAX_PMEM_SEGMENTS)

typedef struct boot_info {
  phys_mmap_t phys_mmap[MAX_MMAP];
  unsigned int phys_mmap_count;
} boot_info_t;

extern boot_info_t boot_info;

void init();

#endif /* _INIT_INIT_H */
