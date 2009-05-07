#ifndef _TRAP_H
#define _TRAP_H

#include <interrupt.h>

void trap_handler(interrupt_vector_t vector, trap_error_code_t error, 
                  trapped_state_t *state);

#endif /* _TRAP_H */
