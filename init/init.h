#ifndef _INIT_INIT_H
#define _INIT_INIT_H

#include <init/mem.h>

typedef struct boot_info {
  phys_mmap_t *phys_mmap;
} boot_info_t;

extern boot_info_t boot_info;

void init();

#endif /* _INIT_INIT_H */
