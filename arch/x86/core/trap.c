#include <interrupt.h>
#include <mem/segment.h>
#include <core/printf.h>

void trap_handler(interrupt_vector_t vector, trap_error_code_t error, 
                  trapped_state_t *state) {
  if (state->cs == USER_CS) { // handle trap from user mode process
    // send signal, kill process, etc..
  } else { // handle trap from anywhere else
    // fix the problem, kill kernel, etc..
  }
}

void syscall_handler(trapped_state_t *state) {
  // handle syscall...
}
