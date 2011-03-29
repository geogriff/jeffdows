#include <core/printf.h>
#include <mem/kmalloc.h>

#include "init.h"

void main() {
  puts("booting OS................\n");
  init();

  char *bufs[1000];
  while(1) {
    for (int i = 0; i < 1000; i++) {
      bufs[i] = kmalloc(sizeof(char) * (i + 1));
      for (int j = 0; j < i; j++) {
	bufs[i][j] = 'a' + (j % ('z' - 'a' + 1));
      }
      bufs[i][i] = 0;
    }
    for (int i = 0; i < 1000; i++) {
      puts(bufs[i]);
      kfree(bufs[i]);
    }
  }

  while(1) {
  }
}
