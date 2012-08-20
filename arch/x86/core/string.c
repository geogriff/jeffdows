#include <types.h>
#include <stddef.h>

void *memset(void *s, int c, size_t n) {
  void *ss = s;
  for (; n > 0; n--, s++)
    *((char *) s) = c;
  return ss;
}

size_t strlen(const char *s) {
  for (size_t s_idx = 0;; s_idx++) {
    if (s[s_idx] == 0) {
      return s_idx;
    }
  }
}
size_t strnlen(const char *s, size_t maxlen) {
  for (size_t s_idx = 0; s_idx < maxlen; s_idx++) {
    if (s[s_idx] == 0) {
      return s_idx;
    }
  }
  return maxlen;
}
