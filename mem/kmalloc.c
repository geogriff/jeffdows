#include <mem/page.h>
#include <mem/pmem.h>
#include <stddef.h>
#include <stdint.h>

struct buffer {
  struct buffer *next_free;
  char buf[];
};

#define BUFFER_SIZE(order) (sizeof(struct buffer) + ((2 << order) >> 1))
#define BUF_BUFFER(buf) ((struct buffer *) \
			 ((void *) buf - sizeof(struct buffer)))
#define BUFFER_SLAB(buf) ((struct slab *) PAGE_FLOOR((uintptr_t) buf))

struct slab {
  struct slab *next_free;
  struct buffer *buffer_freelist;
  int order;
  unsigned int kmalloc_times;
  struct buffer first_buffer[];
};

#define SLAB_ORDER_MIN 4
#define SLAB_ORDER_MAX 11

struct slab *slab_freelists[] = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

struct slab *slab_alloc(int order) {
  // allocate a new page for this slab
  // XXX dont allocate highmem page? (on 32-bit machines)
  page_t *page = pmem_alloc();
  struct slab *slab = PA(page->phys_addr);
  // initialize the slab
  slab->order = order;
  slab->kmalloc_times = 0;
  // add as many buffers to freelist as will fit in the page
  slab->buffer_freelist = slab->first_buffer;
  for (struct buffer *buffer = slab->first_buffer; 
       ((void *) buffer) + BUFFER_SIZE(order) < ((void *) slab) + PAGE_SIZE;
       buffer = buffer->next_free) {
    buffer->next_free = ((void *) buffer + BUFFER_SIZE(order));
  }
  return slab;
}

void *kmalloc(size_t size) {
  int order = (size / 2);
  // XXX disable interrupts
  struct slab *slab = slab_freelists[order];
  if (slab == NULL) {
    // allocate a new slab
    // XXX enable interrupts again during slab_alloc
    slab = slab_alloc(order);
    // add slab to global freelist
    slab->next_free = slab_freelists[order];
    slab_freelists[order] = slab;
  }
  // allocate a buffer from the slab
  struct buffer *buffer = slab->buffer_freelist;
  slab->buffer_freelist = buffer->next_free;

  // remove slab from freelist if it is now full
  if (slab->buffer_freelist == NULL) {
    slab_freelists[order] = slab->next_free;
    slab->next_free = NULL;
  }

  // set buffer's next_free pointer to NULL to catch double frees
  buffer->next_free = NULL;
  return buffer->buf;
}

void kfree(void *buf) {
  // XXX disable interrupts
  struct buffer *buffer = BUF_BUFFER(buf);
  struct slab *slab = BUFFER_SLAB(buffer);

  if (buffer->next_free != NULL) {
    // double free
    // XXX panic?
    return;
  }

  // add buffer to slab's buffer freelist
  buffer->next_free = slab->buffer_freelist;
  slab->buffer_freelist = buffer;

  // add slab to freelist if it was previously full
  // XXX add to end of freelist to improve performance?
  if (buffer->next_free == NULL) {
    slab->next_free = slab_freelists[slab->order];
    slab_freelists[slab->order] = slab;
  }

  // XXX deallocate slab if it becomes empty
}
