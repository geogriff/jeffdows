#include <interrupt.h>
#include <core/printf.h>

void trap(interrupt_vector_t vector) {
  puts("trapped!");
}
