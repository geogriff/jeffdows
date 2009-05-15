#ifndef _INIT_ASM_H
#define _INIT_ASM_H

#include <mem/segment.h>

#define EFLAGS_IF (1<<9)

#ifndef __ASM__

extern void *_end;

void set_gdt(pseudo_segment_descriptor_t *gdtd);
void set_idt(pseudo_segment_descriptor_t *idtd);

void enable_interrupts();
void disable_interrupts();

#endif

#endif /* _INIT_ASM_H */
