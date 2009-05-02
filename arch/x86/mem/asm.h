#ifndef _MEM_ASM_H
#define _MEM_ASM_H

#include <mem/segment.h>

extern void *_end;

void set_gdt(pseudo_segment_descriptor_t *gdtd);

#endif /* _MEM_ASM_H */
