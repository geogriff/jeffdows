#ifndef _INIT_MEM_H
#define _INIT_MEM_H

typedef struct phys_mmap {
  phys_addr_t base_addr;
  size_t limit;
} phys_mmap_t;

#endif
