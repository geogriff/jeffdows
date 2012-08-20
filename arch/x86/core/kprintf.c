#include <stdint.h>
#include <mem/page.h>
#include "kprintf.h"

#define VGA_BASE (KERNEL_BASE+0xB8000)

static const int vga_rows=25, vga_cols=80;
static int vga_ypos=0, vga_xpos=0;

void putchar(char c) {
  if (c == '\n') {
    vga_ypos++;

    // scroll if necessary
    if (vga_ypos >= vga_rows) {
      for (vga_ypos = 0; vga_ypos < vga_rows-1; vga_ypos++)
        for (vga_xpos = 0; vga_xpos < vga_cols; vga_xpos++)
          *((uint16_t *) VGA_BASE + vga_ypos * vga_cols + vga_xpos) = 
            *((uint16_t *) VGA_BASE + (vga_ypos+1) * vga_cols + vga_xpos);
      // now clear last line
      for (vga_xpos = 0; vga_xpos < vga_cols; vga_xpos++)
        *((uint16_t *) VGA_BASE + vga_ypos * vga_cols + vga_xpos) = 0;
    }
    vga_xpos = 0;
  } else if (c != '\r') {
    *((char *) VGA_BASE + 2*(vga_ypos * vga_cols + vga_xpos)) = c;
    *((char *) VGA_BASE + 2*(vga_ypos * vga_cols + vga_xpos)+1) = 7;
    
    // increment current position and do newline if necessary
    if (++vga_xpos >= vga_cols)
      putchar('\n');
  }
}
