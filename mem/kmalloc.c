#include <mem/page.h>
#include <mem/pmem.h>
#include <stddef.h>
#include <stdint.h>

typedef struct slab_buffer {
  struct slab_buffer *next_free;
  char buf[];
} slab_buffer_t;

#define SLAB_BUFFER_SIZE(order) \
  (sizeof(slab_buffer_t) + ((2 << order) >> 1))
#define BUF_SLAB_BUFFER(buf) \
  ((slab_buffer_t *) ((void *) buf - sizeof(slab_buffer_t)))
#define SLAB_BUFFER_SLAB(buf) ((slab_t *) PAGE_FLOOR((uintptr_t) buf))

typedef struct slab {
  struct slab *next_free;
  slab_buffer_t *buffer_freelist;
  int order;
  unsigned int kmalloc_times;
  slab_buffer_t first_buffer[];
} slab_t;

#define SLAB_ORDER_MIN 4
#define SLAB_ORDER_MAX 11

slab_t *slab_freelists[] = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

#define SLAB_FREELIST(order) (slab_freelists[order - SLAB_ORDER_MIN])

int get_slab_order(size_t size) {
  int order = 1;
  for (size--; size > 1; size /= 2, order++);
  return 
    order > SLAB_ORDER_MIN? (order <= SLAB_ORDER_MAX? order : -1)
    : SLAB_ORDER_MIN;
}

slab_t *slab_alloc(int order) {
  // allocate a new page for this slab
  // XXX dont allocate highmem page? (on 32-bit machines)
  page_t *page = pmem_alloc();
  if (page == NULL) {
    return NULL;
  }

  slab_t *slab = PA(page->phys_addr);
  // initialize the slab
  slab->order = order;
  slab->kmalloc_times = 0;
  // add as many buffers to freelist as will fit in the page
  slab->buffer_freelist = slab->first_buffer;
  slab_buffer_t *buffer;
  for (buffer = slab->first_buffer; 
       ((void *) buffer) + 2 * SLAB_BUFFER_SIZE(order)
	 < ((void *) slab) + PAGE_SIZE;
       buffer = buffer->next_free) {
    buffer->next_free = ((void *) buffer + SLAB_BUFFER_SIZE(order));
  }
  buffer->next_free = NULL;

  return slab;
}

void *kmalloc(size_t size) {
  int order = get_slab_order(size);
  // fail if requested size is too large
  if (order < 0) {
    return NULL;
  }

  // XXX disable interrupts
  slab_t *slab = SLAB_FREELIST(order);
  if (slab == NULL) {
    // allocate a new slab
    // XXX enable interrupts again during slab_alloc
    slab = slab_alloc(order);
    if (slab == NULL) {
      return NULL;
    }
    // add slab to global freelist
    slab->next_free = SLAB_FREELIST(order);
    SLAB_FREELIST(order) = slab;
  }
  // allocate a buffer from the slab
  slab_buffer_t *buffer = slab->buffer_freelist;
  slab->buffer_freelist = buffer->next_free;

  // remove slab from freelist if it is now full
  if (slab->buffer_freelist == NULL) {
    SLAB_FREELIST(order) = slab->next_free;
    slab->next_free = NULL;
  }

  // set buffer's next_free pointer to NULL to catch double frees
  buffer->next_free = NULL;
  return buffer->buf;
}

void kfree(void *buf) {
  // XXX disable interrupts
  slab_buffer_t *buffer = BUF_SLAB_BUFFER(buf);
  slab_t *slab = SLAB_BUFFER_SLAB(buffer);

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
    slab->next_free = SLAB_FREELIST(slab->order);
    SLAB_FREELIST(slab->order) = slab;
  }

  // XXX deallocate slab if it becomes empty
}
