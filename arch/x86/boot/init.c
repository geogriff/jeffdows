#include <core/printf.h>
#include "multiboot.h"

void multiboot_init(unsigned long magic, unsigned long addr) {
  multiboot_info_t *multiboot_info = (multiboot_info_t *) addr;

  puts("Booting OS...");

  if (magic != MULTIBOOT_EAX_MAGIC)
    return;
}
