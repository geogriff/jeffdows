#ifndef _MEM_KMALLOC_H
#define _MEM_KMALLOC_H

#include <stddef.h>

void *kmalloc(size_t size);
void kfree(void *buf);

#endif
