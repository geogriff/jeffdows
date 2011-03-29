#ifndef _PAGE_H
#define _PAGE_H

// base virtual address to kernel pages (mapped to physical address 0x00000000) 
#define KERNEL_BASE 0xC0000000

#define PA(x) ((void *) (KERNEL_BASE + (x)))

#define PAGE_SIZE 4096
#define PAGE_FLOOR(x) ((x) & ~(PAGE_SIZE - 1))
#define PAGE_CEIL(x) (PAGE_FLOOR((x) + (PAGE_SIZE - 1)))

#ifndef __ASM__

#include <types.h>
#include <mem/segment.h>

/* IMPORTANT!! the ordering of these bits in each byte is a compiler choice!
   currently, these structs are written the gcc way, which is least significant
   bit first (on little endian machines). note that byte ordering is the same */

typedef struct pde_bigpage {
  unsigned present:1;           // page present in memory (should be 1)
  unsigned writable:1;          // writable
  unsigned user:1;              // user accessable
  unsigned write_through:1;     // enable write-through caching of this page
  unsigned cache_disable:1;     // disable caching of this pde
  unsigned accessed:1;          // page has been read from
  unsigned dirty:1;             // page has been written to
  unsigned bigpage:1;           // pde points to a 4mb page (should be 1)
  unsigned global:1;            // global pde (cache even when pagedir changes)
  unsigned :3;                  // unused
  unsigned pat:1;               // page attribute table bit
  unsigned reserved:9;          // reserved (should be 0)
  unsigned base_addr:10;        // base address of page (high 10 bits)
} PACK pde_bigpage_t;

typedef struct pde_pagetable {
  unsigned present:1;           // page table present in memory (should be 1)
  unsigned writable:1;          // writable
  unsigned user:1;              // user accessable
  unsigned write_through:1;     // enable write-through caching of page table
  unsigned cache_disable:1;     // disable caching of page table
  unsigned accessed:1;          // page table has been read from
  unsigned :1;                  // unused
  unsigned bigpage:1;           // pde points to a 4mb page (should be 0)
  unsigned :4;                  // unused
  unsigned base_addr:20;        // base address of page table (high 20 bits)
} PACK pde_pagetable_t;

typedef struct pde_nonpresent {
  unsigned present:1;       // page/page table present in memory (should be 0)
  unsigned :31;             // unused
} PACK pde_nonpresent_t;

typedef union pde {
    pde_bigpage_t bigpage;
    pde_pagetable_t pagetable;
    pde_nonpresent_t nonpresent;
} pde_t;

typedef struct pte_present {
  unsigned present:1;           // page present in memory (should be 1)
  unsigned writable:1;          // writable
  unsigned user:1;              // user accessable
  unsigned write_through:1;     // enable write-through caching of page
  unsigned cache_disable:1;     // disable caching of this page
  unsigned accessed:1;          // page has been read from
  unsigned dirty:1;             // page has been written to
  unsigned pat:1;               // page attribute table bit
  unsigned global:1;            // global pte (cache even when pagedir changes)
  unsigned :3;                  // unused
  unsigned base_addr:20;
} PACK pte_present_t;

typedef struct pte_nonpresent {
  unsigned present:1;           // page present in memory (should be 0)
  unsigned :31;                 // unused
} PACK pte_nonpresent_t;

typedef union pte {
    pte_present_t present;
    pte_nonpresent_t nonpresent;
} pte_t;

extern pde_t page_dir[1024];

#endif /* __ASM__ */

#endif /* _PAGE_H */
