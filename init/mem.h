#ifndef _INIT_MEM_H
#define _INIT_MEM_H

#include <types.h>
#include <stddef.h>

typedef struct phys_mmap {
  phys_addr_t start;
  size_t limit;
} phys_mmap_t;

void init_mem();

#endif
