#include <types.h>
#include <stddef.h>

void *memset(void *s, int c, size_t n) {
  void *ss = s;
  for (; n > 0; n--, s++)
    *((char *) s) = c;
  return ss;
}
