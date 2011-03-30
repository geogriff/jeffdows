#include <core/printf.h>
#include <mem/kmalloc.h>
#include <stddef.h>
#include <stdbool.h>

#include "init.h"

struct kmalloc_test_buf {
  struct kmalloc_test_buf *next;
  char text[];
};

void main() {
  puts("booting OS................\n");
  init();

  for (size_t size = 16; size <= 2048; size *= 2) {
    struct kmalloc_test_buf *bufs = NULL;
    while (true) {
      struct kmalloc_test_buf *buf = kmalloc(size);
      if (buf == NULL) {
	puts("allocation failed!");
	break;
      }
      buf->next = bufs;
      bufs = buf;
      
      int text_size = (size - sizeof(struct kmalloc_test_buf)) / sizeof(char);
      for (int j = 0; j < text_size - 1; j++) {
	buf->text[j] = 'a' + (j % ('z' - 'a' + 1));
      }
      buf->text[text_size - 1] = 0;
    }
    // free all buffers
    while (bufs != NULL) {
      struct kmalloc_test_buf *buf = bufs;
      bufs = bufs->next;
      kfree(buf);
    }
  }
}
