void init_mem() {
  phys_mmap_t *last_mmap;
  phys_mmap_t *boot_mmap = NULL;
  /* align phys_mmap entries to page boundaries, find a region to hold data for
     the physical memory allocator, and calculate page count */
  {
    phys_mmap_t *mmap;
    for (mmap = &phys_mmap[0]; mmap->limit != 0; mmap++) {
      mmap->base_addr = PAGE_ALIGN_UP(mmap->base_addr);
      mmap->limit = PAGE_ALIGN_DOWN(mmap->limit);
      if (mmap->limit >= boot_page_count * PAGE_SIZE)
        boot_mmap = mmap;
      page_count += mmap->limit / PAGE_SIZE;
    }
    last_mmap = mmap;
  }

  if (boot_mmap == NULL) {
    // panic
  }

  init_pmem(boot_mmap->base_addr);
  boot_mmap->base_addr += boot_page_count * PAGE_SIZE;
  boot_mmap->limit -= boot_page_count * PAGE_SIZE;
}
