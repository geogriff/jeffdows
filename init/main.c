#include <core/printf.h>
#include <mem/kmalloc.h>
#include <stddef.h>
#include <stdbool.h>

#include "init.h"

void main() {
  puts("booting OS................\n");
  init();

  bool alloc_failed = false;
  for (int limit = 1458; !alloc_failed; limit++) {
    char *bufs[limit];
    for (int i = 0; i < limit; i++) {
      bufs[i] = kmalloc(sizeof(char) * (i + 1));
      if (bufs[i] == NULL) {
	puts("allbreak ocation failed!");
	alloc_failed = true;
	break;
      }

      for (int j = 0; j < i; j++) {
	bufs[i][j] = 'a' + (j % ('z' - 'a' + 1));
      }
      bufs[i][i] = 0;
    }
    for (int i = 0; i < limit; i++) {
      if (bufs[i] == NULL) {
	break;
      }
      kfree(bufs[i]);
    }
  }
}
