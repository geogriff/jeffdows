#ifndef _INIT_ASM_H
#define _INIT_ASM_H

#include <mem/segment.h>

extern void *_end;

void set_gdt(pseudo_segment_descriptor_t *gdtd);
void set_idt(pseudo_segment_descriptor_t *idtd);

#endif /* _INIT_ASM_H */
