#ifndef _PAGE_H
#define _PAGE_H

extern pde_t page_dir[1024];

typedef struct pde {
  union {
    pde_bigpage_t bigpage;
    pde_pagetable_t pagetable;
  };
} pde_t;

typedef struct pde_bigpage {
  unsigned int base_addr:10;
  unsigned int reserved:9;
  unsigned int pat:1;
  unsigned int available:3;
  unsigned int global:1;
  unsigned int bigpage:1;
  unsigned int dirty:1;
  unsigned int accessed:1;
  unsigned int cache_disable:1;
  unsigned int write_through:1;
  unsigned int user:1;
  unsigned int writable:1;
  unsigned int present:1;
} pde_bigpage_t;

typedef struct pde_pagetable {
  unsigned int base_addr:20;
  unsigned int available1:3;
  unsigned int global:1;
  unsigned int bigpage:1;
  unsigned int available2:1;
  unsigned int accessed:1;
  unsigned int cache_disable:1;
  unsigned int write_through:1;
  unsigned int user:1;
  unsigned int writable:1;
  unsigned int present:1;
} pde_pagetable_t;

typedef struct pte {
  unsigned int base_addr:20;
  unsigned int available:3;
  unsigned int pat:1;
  unsigned int dirty:1;
  unsigned int accessed:1;
  unsigned int cache_disable:1;
  unsigned int write_through:1;
  unsigned int user:1;
  unsigned int writable:1;
  unsigned int present:1;
} pte_t;

#endif /* _PAGE_H */
