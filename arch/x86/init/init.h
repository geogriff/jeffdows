#ifndef _INIT_H
#define _INIT_H

#include <init/pmem.h>

typedef struct boot_info {
  phys_mmap_t *phys_mmap;
} boot_info_t;

#endif /* _INIT_H */
