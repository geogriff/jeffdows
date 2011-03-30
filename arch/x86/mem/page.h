#ifndef _PAGE_H
#define _PAGE_H

// base virtual address to kernel pages (mapped to physical address 0x00000000) 
#define KERNEL_BASE 0xC0000000

#define PA(x) ((void *) (((phys_addr_t) x) + KERNEL_BASE))
#define VA(x) ((phys_addr_t) ((void *) x) - KERNEL_BASE)

#define PAGE_SIZE 4096
#define PAGE_FLOOR(x) ((x) & ~(PAGE_SIZE - 1))
#define PAGE_CEIL(x) (PAGE_FLOOR((x) + (PAGE_SIZE - 1)))

#ifndef __ASM__

#include <mem/segment.h>
#include <compiler.h>
#include <stdbool.h>
#include <stdint.h>

/* IMPORTANT!! the ordering of these bits in each byte is a compiler choice!
   currently, these structs are written the gcc way, which is least significant
   bit first (on little endian machines). note that byte ordering is the
   same */

typedef struct pde_bigpage {
  bool present:1;           // page present in memory (should be 1)
  bool writable:1;          // writable
  bool user:1;              // user accessable
  bool write_through:1;     // enable write-through caching of this page
  bool cache_disable:1;     // disable caching of this pde
  bool accessed:1;          // page has been read from
  bool dirty:1;             // page has been written to
  bool bigpage:1;           // pde points to a 4mb page (should be 1)
  bool global:1;            // global pde (cache even when pagedir changes)
  uint8_t :3;               // unused
  bool pat:1;               // page attribute table bit
  uint16_t reserved:9;      // reserved (should be 0)
  uint16_t base_addr:10;    // base address of page (high 10 bits)
} PACK pde_bigpage_t;

typedef struct pde_pagetable {
  bool present:1;           // page table present in memory (should be 1)
  bool writable:1;          // writable
  bool user:1;              // user accessable
  bool write_through:1;     // enable write-through caching of page table
  bool cache_disable:1;     // disable caching of page table
  bool accessed:1;          // page table has been read from
  bool :1;                  // unused
  bool bigpage:1;           // pde points to a 4mb page (should be 0)
  uint8_t :4;               // unused
  uint32_t base_addr:20;    // base address of page table (high 20 bits)
} PACK pde_pagetable_t;

typedef struct pde_nonpresent {
  bool present:1;       // page/page table present in memory (should be 0)
  uint32_t :31;         // unused
} PACK pde_nonpresent_t;

typedef union pde {
    pde_bigpage_t bigpage;
    pde_pagetable_t pagetable;
    pde_nonpresent_t nonpresent;
} pde_t;

typedef struct pte_present {
  bool present:1;           // page present in memory (should be 1)
  bool writable:1;          // writable
  bool user:1;              // user accessable
  bool write_through:1;     // enable write-through caching of page
  bool cache_disable:1;     // disable caching of this page
  bool accessed:1;          // page has been read from
  bool dirty:1;             // page has been written to
  bool pat:1;               // page attribute table bit
  bool global:1;            // global pte (cache even when pagedir changes)
  uint8_t :3;               // unused
  uint32_t base_addr:20;    // base address of the page
} PACK pte_present_t;

typedef struct pte_nonpresent {
  bool present:1;           // page present in memory (should be 0)
  uint32_t :31;             // unused
} PACK pte_nonpresent_t;

typedef union pte {
    pte_present_t present;
    pte_nonpresent_t nonpresent;
} pte_t;

extern pde_t page_dir[1024];

#endif /* __ASM__ */

#endif /* _PAGE_H */
